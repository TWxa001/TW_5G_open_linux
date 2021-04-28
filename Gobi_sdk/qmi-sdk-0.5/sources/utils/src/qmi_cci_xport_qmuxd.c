/******************************************************************************
  ---------------------------------------------------------------------------
  Copyright (c) 2011-2012 QUALCOMM Technologies Incorporated. All Rights Reserved.
  QUALCOMM Proprietary and Confidential.
  ---------------------------------------------------------------------------
*******************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "qmi_cci_target.h"
#include "qmi_client.h"
#include "qmi_idl_lib.h"
#include "qmi_cci_common.h"
#include "qmi_fw_debug.h"
#include "qmi_client_instance_defs.h"

// IOCTL to generate a client ID for this service type
#define IOCTL_QMI_GET_SERVICE_FILE 0x8BE0 + 1

// IOCTL to get the VIDPID of the device
#define IOCTL_QMI_GET_DEVICE_VIDPID 0x8BE0 + 2

// IOCTL to get the MEID of the device
#define IOCTL_QMI_GET_DEVICE_MEID 0x8BE0 + 3

// IOCTL to get the qmi version of the device
#define IOCTL_QMI_GET_QMI_VERSION 0x8BE0 + 4

struct xport_handle
{
  qmi_cci_client_type *clnt;
  qmi_connection_id_type conn_id;
  unsigned char service_id;
  int client_id;
  uint32_t max_rx_len;
  uint32_t client_id_valid;
  LINK(struct xport_handle, link);
};
int bthread = 0;
pthread_t tid;
/* File descriptor sets */
fd_set master_fd_set, read_fd_set;
int pipefd[2];
/* maximum file descriptor number */
int max_fd = -1;
/* Track all the active QMUX connections in a list because the Rx callback does
 * not have a user-data parameter. Argh!
 */
static char* qmidev[] ={
	"/dev/qcqmi0",
	"/dev/qcqmi1",
	"/dev/qcqmi2",
	"/dev/qcqmi3",
	"/dev/qcqmi4",
	"/dev/qcqmi5",
	"/dev/qcqmi6",
	"/dev/qcqmi7",
};

static qmi_cci_lock_type client_list_lock;
static LIST(struct xport_handle, client_list);
static int qmux_client_instances;
static qmi_cci_lock_type instances_count_lock = PTHREAD_MUTEX_INITIALIZER;

static void *reader_thread(void *handle)
{
  struct xport_handle *xp = (struct xport_handle *)handle;
  unsigned char *buf;
  int rx_len;
  int rc;

  buf = malloc(xp->max_rx_len);
  if(!buf)
  {
    printf("Unable to allocate read buffer\n");
    return NULL;
  }

  while(1)
  {
    read_fd_set = master_fd_set;
    QMI_CCI_LOGD("QMUXD: select %d\n", max_fd);
    rc = select(max_fd+1, &read_fd_set, NULL, NULL, NULL);
    QMI_CCI_LOGD("QMUXD: got data\n");
    
    if(rc < 0)
    {
        break;
    }
    else
    {
        if(FD_ISSET(pipefd[0], &read_fd_set))
        {
            while(read(pipefd[0], buf, 1) > 0);
            continue;
        }
        LOCK(&client_list_lock);
        xp = LIST_HEAD(client_list);
        while(xp)
        {
          if(xp->client_id_valid && FD_ISSET(xp->client_id, &read_fd_set))
          {
            QMI_CCI_LOGD("QMUXD: %s delivering broadcast message <%d:%d:%d>\n",
                          __func__, xp->conn_id, xp->service_id, xp->client_id);
            if((rx_len=read(xp->client_id, buf, xp->max_rx_len)) > 0)
            {
                qmi_cci_xport_recv(xp->clnt, &xp->client_id, buf, rx_len);
            }
            
          }
          xp = xp->link.next;
        }
        UNLOCK(&client_list_lock);
    }
  }

  free(buf);
  return NULL;
}

static int qmuxd_ref_count_up(void)
{
  int rc;
  LOCK(&instances_count_lock);
  if(qmux_client_instances == 0)
  {
      FD_ZERO (&master_fd_set);
    FD_ZERO (&read_fd_set);
  }
  qmux_client_instances++;
  UNLOCK(&instances_count_lock);
  return 0;
}

static void qmuxd_ref_count_down(void)
{
  LOCK(&instances_count_lock);
  if(qmux_client_instances > 0)
  {
    qmux_client_instances--;
  }
  UNLOCK(&instances_count_lock);
}

static void *xport_open
(
 void *xport_data,
 qmi_cci_client_type *clnt,
 uint32_t service_id,
 uint32_t version,
 void *addr,
 uint32_t max_rx_len
 )
{
  int rc;
  int opts;

  struct xport_handle *xp = calloc(sizeof(struct xport_handle), 1);
  if(!xp)
  {
    QMI_FW_LOGE("QMUXD: Failed to allocate xport_handle\n");
    return NULL;
  }

  xp->clnt = clnt;

  /* conn_id, i.e. QMI_CONN_ID_RMNET_0, is passed in when xport is registered */
  xp->conn_id = ((qmi_connection_id_type)xport_data - QMI_CLIENT_QMUX_BASE_INSTANCE);
  xp->service_id = service_id;
  xp->max_rx_len = max_rx_len;
  xp->client_id = 0;
  xp->client_id_valid = 0;

  QMI_CCI_LOGD("QMUXD: %s[%d] for service_id=%d max_rx_len=%d\n",
                __func__, xp->conn_id, xp->service_id, max_rx_len);
  if(qmuxd_ref_count_up())
  {
    free(xp);
    return NULL;
  }

  if (addr)
  {
    xp->client_id = open(qmidev[xp->conn_id], O_RDWR);
    if(xp->client_id < 0)
    {
	QMI_FW_LOGE("QMUXD: open %s failed\n",qmidev[xp->conn_id]);
        free(xp);
        return NULL;
    }
    opts = fcntl(xp->client_id,F_GETFL);
    if (opts < 0)
    {
	QMI_FW_LOGE("QMUXD: fcntl GETFL failed\n");
        close(xp->client_id);
        free(xp);
        return NULL;
    }
    opts = (opts | O_NONBLOCK);
    if (fcntl(xp->client_id,F_SETFL,opts) < 0)
    {
	QMI_FW_LOGE("QMUXD: fcntl SETFL failed\n");
        close(xp->client_id);
        free(xp);
        return;
    }
    rc = ioctl(xp->client_id, IOCTL_QMI_GET_SERVICE_FILE, service_id);
    if(rc != 0)
    {
      QMI_FW_LOGE("QMUXD: WARNING qmi_qmux_if_alloc_service_client failed!"
                  " service_id = %d, conn_id = %d, rc=%d\n",
                   xp->service_id, xp->conn_id, rc);
      qmuxd_ref_count_down();
      close(xp->client_id);
      free(xp);
      return NULL;
    }
    /* Add the new fd to the master fd set */
    FD_SET (xp->client_id,&master_fd_set);
    if (xp->client_id > max_fd)
    {
        max_fd = xp->client_id;
    }
    if(bthread == 0)
    {
        if(pipe((int*)pipefd) < 0)
        {
            perror("pipe()");
            close(xp->client_id);
            free(xp);
            return NULL;
        }
        opts = fcntl(pipefd[0],F_GETFL);
        if (opts < 0)
        {
		QMI_FW_LOGE("QMUXD: fcntl GETFL failed\n");
            close(pipefd[0]);
            close(pipefd[1]);
            close(xp->client_id);
            free(xp);
            return NULL;
        }
        opts = (opts | O_NONBLOCK);
        if (fcntl(pipefd[0],F_SETFL,opts) < 0)
        {
		QMI_FW_LOGE("QMUXD: fcntl SETFL failed\n");
            close(pipefd[0]);
            close(pipefd[1]);
            close(xp->client_id);
            free(xp);
            return NULL;
        }
    
        opts = fcntl(pipefd[1],F_GETFL);
        if (opts < 0)
        {
		QMI_FW_LOGE("QMUXD: fcntl GETFL failed\n");
            close(pipefd[0]);
            close(pipefd[1]);
            close(xp->client_id);
            free(xp);
            return NULL;
        }
        opts = (opts | O_NONBLOCK);
        if (fcntl(pipefd[1],F_SETFL,opts) < 0)
        {
		QMI_FW_LOGE("QMUXD: fcntl SETFL failed\n");
            close(pipefd[0]);
            close(pipefd[1]);
            close(xp->client_id);
            free(xp);
            return NULL;
        }
        FD_SET(pipefd[0], &master_fd_set );
        if (pipefd[0] > max_fd)
        {
            max_fd = pipefd[0];
        }

        /* create reader thread */
        if(pthread_create(&tid, NULL, reader_thread, xp))
        {
            perror("pthread_create()");
            close(xp->client_id);
            free(xp);
            return NULL;
        }
        bthread = 1;
    }

    xp->client_id_valid = 1;
    write(pipefd[1], &bthread, 1);
  }

  LOCK(&client_list_lock);
  LIST_ADD(client_list, xp, link);
  UNLOCK(&client_list_lock);
  QMI_CCI_LOGD("QMUXD: %s[%d] successful for service_id=%d max_rx_len=%d\n",
                __func__, xp->conn_id, xp->service_id, max_rx_len);
  return xp;
}

static qmi_client_error_type xport_send
(
 void *handle,
 void *addr,
 uint8_t *buf,
 uint32_t len
 )
{
  struct xport_handle *xp = (struct xport_handle *)handle;
  int rc;

  QMI_CCI_LOGD("QMUXD: %s[%d -> %d]: %d bytes on conn_id=%d\n",
                __func__, xp->client_id, xp->service_id, len, xp->conn_id);
  if (!xp->client_id_valid)
  {
    QMI_FW_LOGE("QMUXD: %s on an invalid client id\n", __func__);
    return QMI_SERVICE_ERR;
  }

  if(len == write(xp->client_id, buf, len))
  {
    rc = QMI_NO_ERR;
  }
  else
  {
    rc = QMI_SERVICE_ERR;
  }
  return rc;
}

static void xport_close
(
 void *handle
 )
{
  struct xport_handle *xp = (struct xport_handle *)handle;

  QMI_CCI_LOGD("QMUXD: %s[%d] service_id=%d, conn_id=%d\n",
                __func__, xp->conn_id, xp->service_id, xp->client_id);


  qmuxd_ref_count_down();

  LOCK(&client_list_lock);

  LIST_REMOVE(client_list, xp, link);
  FD_CLR (xp->client_id, &master_fd_set);
  /* Find new max_fd */
    if(xp->client_id == max_fd)
    {
        max_fd = 0;
        xp = LIST_HEAD(client_list);
        while(xp)
        {
            if(xp->client_id > max_fd)
            {
                FD_SET (xp->client_id, &master_fd_set);
            }
          xp = xp->link.next;
        }
    }
  UNLOCK(&client_list_lock);
    xp->client_id_valid = 0;
    close(xp->client_id);
    xp->client_id = 0;
  QMI_CCI_LOGD("QMUXD: %s[%d] complete, service_id=%d, conn_id=%d\n",
                __func__, xp->conn_id, xp->service_id, xp->client_id);
  qmi_cci_xport_closed(xp->clnt);
  free(xp);
}


static uint32_t xport_lookup
(
 void *xport_data,
 uint8_t xport_num,
 uint32_t service_id,
 uint32_t version,
 uint32_t *num_entries,
 qmi_cci_service_info *service_info
)
{
  qmi_connection_id_type conn_id = ((qmi_connection_id_type)xport_data - QMI_CLIENT_QMUX_BASE_INSTANCE);
    if(access(qmidev[conn_id], F_OK))
    {
        return 0;
    }
    if (num_entries && service_info)
    {
      service_info[0].xport = xport_num;
      service_info[0].version = version;
      service_info[0].instance = (qmi_client_qmux_instance_type)xport_data;
      service_info[0].reserved = 0;
      *num_entries = 1;
    }
    QMI_CCI_LOGD("QMUXD: Service_id=%d found over conn_id=%d\n",
                  service_id, conn_id);
    return 1;
}

static uint32_t xport_addr_len
(
 void
 )
{
  return 0;
}

qmi_cci_xport_ops_type qmuxd_ops =
{
  xport_open,
  xport_send,
  xport_close,
  xport_lookup,
  xport_addr_len
};
