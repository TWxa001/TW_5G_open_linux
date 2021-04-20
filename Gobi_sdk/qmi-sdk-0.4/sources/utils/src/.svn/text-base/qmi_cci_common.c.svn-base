/******************************************************************************
  @file    qmi_cci_common.c
  @brief   The QMI common client interface common module

  DESCRIPTION
  QMI common client routines.  All client will be build on top of these
  routines for initializing, sending messages and receiving responses/
  indications.

  INITIALIZATION AND SEQUENCING REQUIREMENTS
  qmi_client_init() needs to be called before sending or receiving of any
  service specific messages

  ---------------------------------------------------------------------------
  Copyright (c) 2011-2012 QUALCOMM Technologies Incorporated. All Rights Reserved.
  QUALCOMM Proprietary and Confidential.
  ---------------------------------------------------------------------------
*******************************************************************************/
#include "qmi_client.h"
#include "qmi_idl_lib.h"
#include "qmi_cci_target.h"
#include "qmi_cci_common.h"
#include "qmi_fw_debug.h"
#include <string.h>

#define QCCI_MIN(a, b) ((a) > (b) ? (b) : (a))

struct qmi_cci_xport_tbl_s
{
  qmi_cci_xport_ops_type *ops;
  void *xport_data;
};

static struct qmi_cci_xport_tbl_s xport_tbl[MAX_XPORTS];

static unsigned int num_xports;

/*===========================================================================
  FUNCTION  remove_txn
===========================================================================*/
/*!
@brief

  Find, remove a transaction from client's transaction list and free the txn

@return

  QMI_NO_ERR if found
  QMI_INVALID_TXN otherwise

@note
*/
/*=========================================================================*/
static qmi_client_error_type remove_txn(qmi_cci_client_type *clnt, qmi_cci_txn_type *txn)
{
  qmi_cci_txn_type *temp_txn;

  LOCK(&clnt->txn_list_lock);
  temp_txn = LIST_HEAD(clnt->txn_list);
  while(temp_txn)
  {
    if(temp_txn == txn)
    {
      LIST_REMOVE(clnt->txn_list, txn, link);
      FREE(txn);
      break;
    }
    temp_txn = temp_txn->link.next;
  }
  UNLOCK(&clnt->txn_list_lock);

  return (temp_txn ? QMI_NO_ERR : QMI_INVALID_TXN);
}

/*===========================================================================
  FUNCTION  get_dest_addr
===========================================================================*/
/*!
@brief

  Gets the destination address of the service.

@return

  QMI_NO_ERR if server exists and is valid (addr will be populated with the
             correct address)
  QMI_INTERNAL_ERR - Invalid parameters
  QMI_SERVICE_ERR - Address to the service is not valid (Service does not
  exist)

@note
*/
/*=========================================================================*/
static qmi_client_error_type get_dest_addr(qmi_cci_client_type *clnt, uint8_t *addr)
{
  if(!clnt || !addr)
    return QMI_INTERNAL_ERR;
  LOCK(&clnt->server_addr_valid_lock);
  if(!clnt->server_addr_valid)
  {
    UNLOCK(&clnt->server_addr_valid_lock);
    return QMI_SERVICE_ERR;
  }
  memcpy(addr, clnt->server_addr, MAX_ADDR_LEN);
  UNLOCK(&clnt->server_addr_valid_lock);
  return QMI_NO_ERR;
}

/*===========================================================================
  FUNCTION  qmi_client_get_txn
===========================================================================*/
/*!
@brief

  Create a transaction and return the handle

@return
  qmi_client_error_type

@note
  The transaction is added to the client's outstand transaction list
*/
/*=========================================================================*/
static qmi_client_error_type qmi_client_get_txn
(
 qmi_cci_client_type              *clnt,
 qmi_cci_txn_enum_type             type,
 unsigned int                      msg_id,
 void                              *resp_buf,
 unsigned int                      resp_buf_len,
 qmi_client_recv_raw_msg_async_cb  raw_async_rx_cb,
 qmi_client_recv_msg_async_cb      msg_async_rx_cb,
 void                              *rx_cb_data,
 qmi_cci_txn_type                  **txn_handle
 )
{
  qmi_cci_txn_type *txn;

  txn = CALLOC(1, sizeof(qmi_cci_txn_type));
  if(!txn)
    return QMI_INTERNAL_ERR;

  txn->type = type;
  txn->msg_id = msg_id;
  txn->txn_id = clnt->next_txn_id++;
  txn->raw_async_rx_cb = raw_async_rx_cb;
  txn->msg_async_rx_cb = msg_async_rx_cb;
  txn->rx_cb_data = rx_cb_data;
  txn->rx_buf = resp_buf;
  txn->rx_buf_len = resp_buf_len;

  LOCK(&clnt->txn_list_lock);
  LIST_ADD(clnt->txn_list, txn, link);
  UNLOCK(&clnt->txn_list_lock);

  *txn_handle = txn;
  return QMI_NO_ERR;
}

/*===========================================================================
  FUNCTION  encode_and_send
===========================================================================*/
/*!
@brief

  Encode and send a message to the client

@return
  qmi_client_error_type

@note
*/
/*=========================================================================*/
static qmi_client_error_type encode_and_send
(
 qmi_cci_client_type *clnt,
 qmi_idl_type_of_message_type msg_type,
 uint16_t txn_id,
 uint16_t msg_id,
 void *c_struct,
 int c_struct_len,
 void *dest_addr
)
{
  int rc;
  uint32_t max_msg_len;
  uint32_t out_len, idl_c_struct_len;
  unsigned char *msg;
  uint8_t cntl_flag;

  rc = qmi_idl_get_message_c_struct_len(clnt->service_obj, msg_type, msg_id,
      &idl_c_struct_len);
  if(rc != QMI_IDL_LIB_NO_ERR)
      return rc;

  if(c_struct_len != idl_c_struct_len)
    return QMI_INTERNAL_ERR;

  if(c_struct && c_struct_len)
  {
    rc = qmi_idl_get_max_message_len(clnt->service_obj, msg_type, msg_id,
                &max_msg_len);
    if(rc != QMI_IDL_LIB_NO_ERR)
      return rc;

    msg = MALLOC(max_msg_len + QMI_HEADER_SIZE);
    if(!msg)
      return QMI_INTERNAL_ERR;

    rc = qmi_idl_message_encode(
        clnt->service_obj,
        msg_type,
        msg_id,
        c_struct,
        c_struct_len,
        msg + QMI_HEADER_SIZE,
        max_msg_len,
        (uint32_t *)&out_len);

    if(rc != QMI_IDL_LIB_NO_ERR)
    {
      FREE(msg);
      return rc;
    }
  }
  else
  {
    /* Empty message */
    out_len = 0;
    msg = MALLOC(QMI_HEADER_SIZE);
    if(!msg)
      return QMI_INTERNAL_ERR;
  }

  switch(msg_type)
  {
    case QMI_IDL_INDICATION:
      cntl_flag = QMI_INDICATION_CONTROL_FLAG;
      break;
    case QMI_IDL_RESPONSE:
      cntl_flag = QMI_RESPONSE_CONTROL_FLAG;
      break;
    default:
      cntl_flag = QMI_REQUEST_CONTROL_FLAG;
      break;
  }

  /* fill in header */
  encode_header(msg, cntl_flag, txn_id, msg_id, (uint16_t)out_len);

  QMI_CCI_OS_LOG_TX(msg);

  out_len += QMI_HEADER_SIZE;

  rc = clnt->xport->ops->send(clnt->xport->handle, dest_addr, msg,
      out_len);

  FREE(msg);

  return (rc == QMI_NO_ERR ? QMI_NO_ERR : QMI_SERVICE_ERR);
}

/*===========================================================================
  FUNCTION  handle_txn_error
===========================================================================*/
/*!
@brief
  Handle transaction error base on its type and set return code to error

@note
  Transaction is freed in the async case. The thread waiting on a sync response
  will free the transation after waking up.
*/
/*=========================================================================*/
static void handle_txn_error
(
 qmi_cci_client_type *clnt,
 qmi_cci_txn_type *txn,
 int error
 )
{
  txn->rc = error;

  switch(txn->type)
  {
    case TXN_SYNC_MSG:
    case TXN_SYNC_RAW:
      /* txn freed by the waiting function */
      QMI_CCI_OS_SIGNAL_SET(&clnt->signal);
      break;

    case TXN_ASYNC_MSG:
      if(txn->msg_async_rx_cb)
      {
        txn->msg_async_rx_cb((qmi_client_type)clnt, txn->msg_id, txn->rx_buf,
            0, txn->rx_cb_data, txn->rc);
      }
      FREE(txn);
      break;

    case TXN_ASYNC_RAW:
      if(txn->raw_async_rx_cb)
        txn->raw_async_rx_cb((qmi_client_type)clnt, txn->msg_id, txn->rx_buf,
            0, txn->rx_cb_data, txn->rc);
      FREE(txn);
      break;
    default:
      break;
  }
}
/*===========================================================================
                                EXPORTED FUNCTIONS
===========================================================================*/
void qmi_cci_xport_closed
(
 qmi_cci_client_type *clnt
 )
{
  uint32_t ref;

  if(!clnt)
    return;

  /* decrement and obtain ref count */
  LOCK(&clnt->xport_ref_lock);
  ref = --clnt->xport_ref_cnt;
  UNLOCK(&clnt->xport_ref_lock);

  /* signal thread waiting on qmi_client_release() that it can
     fully free the client structure */
  if(!ref)
  {
    QMI_CCI_OS_SIGNAL_SET(&clnt->signal);
  }
}

/*===========================================================================
  FUNCTION  qmi_cci_xport_event_new_server
===========================================================================*/
void qmi_cci_xport_event_new_server
(
 qmi_cci_client_type *clnt,
 void *addr
 )
{
  if(!clnt || !clnt->ext_signal)
    return;

  QMI_CCI_OS_SIGNAL_SET(clnt->ext_signal);
}

/*===========================================================================
  FUNCTION  qmi_cci_xport_event_remove_server
===========================================================================*/
void qmi_cci_xport_event_remove_server
(
 qmi_cci_client_type *clnt,
 void *addr
 )
{
  qmi_cci_txn_type *txn;
  qmi_client_error_cb err_cb;
  void *err_cb_data;

  if(!clnt)
    return;

  /* signal notifier of the event */
  if(clnt->ext_signal)
  {
    QMI_CCI_OS_SIGNAL_SET(clnt->ext_signal);
  }

  /* check to see if it's the server we're talking to */
  if(memcmp(addr, clnt->server_addr, clnt->xport->addr_len))
    return;

  /* zero out the server's address so no future transaction can occur */
  LOCK(&clnt->server_addr_valid_lock);
  clnt->server_addr_valid = 0;
  memset(clnt->server_addr, 0, clnt->xport->addr_len);
  UNLOCK(&clnt->server_addr_valid_lock);

  /* remove txns from client's list and handle error */
  LOCK(&clnt->txn_list_lock);
  txn = LIST_HEAD(clnt->txn_list);
  while(txn)
  {
    qmi_cci_txn_type *to_free = txn;
    txn = txn->link.next;
    LIST_REMOVE(clnt->txn_list, to_free, link);

    /* handle transaction error base on its type */
    handle_txn_error(clnt, to_free, QMI_SERVICE_ERR);
  }
  UNLOCK(&clnt->txn_list_lock);

  /* notify error callback */
  LOCK(&clnt->err_cb_lock);
  err_cb = clnt->err_cb;
  err_cb_data = clnt->err_cb_data;

  /* set error pending only if no callback is registered */
  if(!err_cb)
    clnt->err_pending = 1;

  UNLOCK(&clnt->err_cb_lock);

  if(err_cb)
  {
    err_cb((qmi_client_type)clnt, QMI_SERVICE_ERR, err_cb_data);
  }
}


/*===========================================================================
  FUNCTION  qmi_cci_xport_recv
===========================================================================*/
qmi_client_error_type qmi_cci_xport_recv
(
 qmi_cci_client_type *clnt,
 void *addr,
 uint8_t *buf,
 uint32_t len
 )
{
  uint8_t cntl_flag;
  uint16_t txn_id, msg_id, msg_len;
  qmi_cci_txn_type *txn;

  if(!clnt || len < QMI_HEADER_SIZE)
    return QMI_INTERNAL_ERR;

  QMI_CCI_OS_LOG_RX(buf);

  /* decode message header and find the transaction */
  decode_header(buf, &cntl_flag, &txn_id, &msg_id, &msg_len);
  buf += QMI_HEADER_SIZE;
  len -= QMI_HEADER_SIZE;

  ASSERT(msg_len == len);

  if(cntl_flag != QMI_RESPONSE_CONTROL_FLAG &&
      cntl_flag != QMI_INDICATION_CONTROL_FLAG)
    return QMI_INTERNAL_ERR;

  /* process indication */
  if(cntl_flag == QMI_INDICATION_CONTROL_FLAG)
  {
    if(clnt->ind_cb)
    {
      clnt->ind_cb((qmi_client_type)clnt, msg_id, msg_len ? buf : NULL, msg_len,
          clnt->ind_cb_data);
    }
    return QMI_NO_ERR;
  }

  /* process transaction */
  LOCK(&clnt->txn_list_lock);
  txn = LIST_HEAD(clnt->txn_list);
  while(txn)
  {
    if(txn->txn_id == txn_id)
    {
      LIST_REMOVE(clnt->txn_list, txn, link);
      break;
    }
    txn = txn->link.next;
  }

  UNLOCK(&clnt->txn_list_lock);

  /* transaction not found */
  if(!txn)
    return QMI_INTERNAL_ERR;

  /* mismatched msg_id, something went wrong - bail */
  if(txn->msg_id != msg_id)
  {
    handle_txn_error(clnt, txn, QMI_INVALID_TXN);
    return QMI_INTERNAL_ERR;
  }

  switch(txn->type)
  {
    case TXN_SYNC_MSG:
      txn->rc = qmi_idl_message_decode(
          clnt->service_obj,
          QMI_IDL_RESPONSE,
          msg_id,
          buf,
          len,
          txn->rx_buf,
          txn->rx_buf_len
          );
      QMI_CCI_OS_SIGNAL_SET(&clnt->signal);
      break;

    case TXN_SYNC_RAW:
      memcpy(txn->rx_buf, buf, QCCI_MIN(len, txn->rx_buf_len));
      txn->reply_len = QCCI_MIN(len, txn->rx_buf_len);
      txn->rc = QMI_NO_ERR;
      QMI_CCI_OS_SIGNAL_SET(&clnt->signal);
      break;

    case TXN_ASYNC_MSG:
      if(txn->msg_async_rx_cb)
      {
        txn->rc = qmi_idl_message_decode(
            clnt->service_obj,
            QMI_IDL_RESPONSE,
            msg_id,
            buf,
            len,
            txn->rx_buf,
            txn->rx_buf_len
            );
        txn->msg_async_rx_cb((qmi_client_type)clnt, msg_id, txn->rx_buf,
            txn->rx_buf_len, txn->rx_cb_data, txn->rc);
      }
      FREE(txn);
      break;

    case TXN_ASYNC_RAW:
      memcpy(txn->rx_buf, buf, QCCI_MIN(len, txn->rx_buf_len));
      if(txn->raw_async_rx_cb)
        txn->raw_async_rx_cb((qmi_client_type)clnt, msg_id, txn->rx_buf,
            QCCI_MIN(len, txn->rx_buf_len), txn->rx_cb_data, QMI_NO_ERR);
      FREE(txn);
      break;
    default:
      break;
  }

  return QMI_NO_ERR;
}

/*===========================================================================
  FUNCTION  qmi_cci_xport_start
===========================================================================*/
void qmi_cci_xport_start
(
 qmi_cci_xport_ops_type *ops,
 void *xport_data
 )
{
  int i;
  for (i = 0; i < num_xports; i++)
  {
    if ((xport_tbl[i].ops == ops) && (xport_tbl[i].xport_data == xport_data))
      return;
  }

  if(num_xports < MAX_XPORTS)
  {
    xport_tbl[num_xports].ops = ops;
    xport_tbl[num_xports].xport_data = xport_data;
    num_xports++;
  }
}

/*===========================================================================
  FUNCTION  qmi_cci_xport_stop
===========================================================================*/
void qmi_cci_xport_stop
(
 qmi_cci_xport_ops_type *ops,
 void *xport_data
 )
{
  int i;
  for (i = 0; i < MAX_XPORTS; i++)
  {
    if ((xport_tbl[i].ops == ops) && (xport_tbl[i].xport_data == xport_data))
    {
      xport_tbl[i].ops = NULL;
      xport_tbl[i].xport_data = NULL;
      num_xports--;
    }
  }
}

/*===========================================================================
  FUNCTION  qmi_client_notifier_init
===========================================================================*/
qmi_client_error_type
qmi_client_notifier_init
(
 qmi_idl_service_object_type               service_obj,
 qmi_client_os_params                      *os_params,
 qmi_client_type                           *user_handle
 )
{
  qmi_cci_client_type *clnt;
  uint32_t service_id, idl_version, i;
  int rc;

  if(!user_handle)
    return QMI_INTERNAL_ERR;

  *user_handle = NULL;

  if(!service_obj || !os_params)
    return QMI_INTERNAL_ERR;

  clnt = CALLOC(1, sizeof(qmi_cci_client_type));
  if(!clnt)
    return QMI_INTERNAL_ERR;

  /* Extract service id */
  rc =  qmi_idl_get_service_id(service_obj, &service_id);
  if (rc !=  QMI_IDL_LIB_NO_ERR)
    goto init_bail;

  /* Get IDL version */
  rc =  qmi_idl_get_idl_version(service_obj, &idl_version);
  if (rc !=  QMI_IDL_LIB_NO_ERR)
    goto init_bail;

  clnt->service_obj = service_obj;

  LOCK_INIT(&clnt->xport_ref_lock);
  LOCK_INIT(&clnt->err_cb_lock);
  LOCK_INIT(&clnt->txn_list_lock);
  LOCK_INIT(&clnt->server_addr_valid_lock);
  QMI_CCI_OS_EXT_SIGNAL_INIT(clnt->ext_signal, os_params);
  QMI_CCI_OS_SIGNAL_INIT(&clnt->signal, os_params);

  rc = QMI_INTERNAL_ERR;

  /* go through all the xports and find the service */
  for(i = 0; i < num_xports; i++)
  {
    /* figure out the address length of xport */
    clnt->xport[i].handle = xport_tbl[i].ops->open(xport_tbl[i].xport_data,
        clnt, service_id, idl_version, NULL, 0);

    /* unable to open xport, go to the next one */
    if(!clnt->xport[i].handle)
      continue;

    /* if server exists, signal notifier */
    if(xport_tbl[i].ops->lookup(xport_tbl[i].xport_data, (uint8_t)i, service_id,
        idl_version, NULL, NULL))
    {
      QMI_CCI_OS_SIGNAL_SET(clnt->ext_signal);
    }

    rc = QMI_NO_ERR;
    clnt->xport[i].ops = xport_tbl[i].ops;
    clnt->xport[i].addr_len = QCCI_MIN(MAX_ADDR_LEN, xport_tbl[i].ops->addr_len());

    /* increment xport reference count without lock */
    clnt->xport_ref_cnt++;
  }

  *user_handle = (qmi_client_type)clnt;

  return rc;

init_bail:
  FREE(clnt);
  return rc;
}

/*===========================================================================
  FUNCTION  qmi_client_init
===========================================================================*/
qmi_client_error_type
qmi_client_init
(
 qmi_service_info                          *service_info,
 qmi_idl_service_object_type               service_obj,
 qmi_client_ind_cb                         ind_cb,
 void                                      *ind_cb_data,
 qmi_client_os_params                      *os_params,
 qmi_client_type                           *user_handle
 )
{
  qmi_cci_service_info *svc = (qmi_cci_service_info *)service_info;
  qmi_cci_client_type *clnt;
  uint32_t service_id, idl_version;
  uint32_t max_msg_len, i;
  int rc;
  unsigned int num_entries = 0, num_services;
  qmi_service_info *service_array = NULL;

  if(!svc || !user_handle)
    return QMI_INTERNAL_ERR;

  *user_handle = NULL;

  if(svc->xport >= num_xports)
    return QMI_INTERNAL_ERR;

  clnt = CALLOC(1,sizeof(qmi_cci_client_type));
  if(!clnt)
    return QMI_INTERNAL_ERR;

  /* Extract service id */
  rc =  qmi_idl_get_service_id(service_obj, &service_id);
  if (rc !=  QMI_IDL_LIB_NO_ERR)
    goto init_bail;

  /* Get IDL version */
  rc =  qmi_idl_get_idl_version(service_obj, &idl_version);
  if (rc !=  QMI_IDL_LIB_NO_ERR)
    goto init_bail;

  /* Get max message length */
  rc = qmi_idl_get_max_service_len(service_obj, &max_msg_len);
  if(rc != QMI_IDL_LIB_NO_ERR)
    goto init_bail;

  clnt->next_txn_id = 1;
  clnt->service_obj = service_obj;
  clnt->ind_cb = ind_cb;
  clnt->ind_cb_data = ind_cb_data;

  LOCK_INIT(&clnt->xport_ref_lock);
  LOCK_INIT(&clnt->err_cb_lock);
  LOCK_INIT(&clnt->txn_list_lock);
  LOCK_INIT(&clnt->server_addr_valid_lock);
  QMI_CCI_OS_SIGNAL_INIT(&clnt->signal, os_params);

  /* figure out the address length of xport */
  clnt->xport->addr_len = QCCI_MIN(MAX_ADDR_LEN, xport_tbl[svc->xport].ops->addr_len());

  /* fill in server address */
  clnt->server_addr_valid = 1;
  memcpy(clnt->server_addr, svc->addr, clnt->xport->addr_len);

  clnt->xport->ops = xport_tbl[svc->xport].ops;
  clnt->xport->handle = xport_tbl[svc->xport].ops->open(xport_tbl[svc->xport].xport_data, clnt, 
      service_id, idl_version, clnt->server_addr, max_msg_len);

  /* unable to open xport, bail */
  if(!clnt->xport->handle)
  {
    rc = QMI_INTERNAL_ERR;
    goto init_bail;
  }

  /* redo lookup to make sure the server actually exits */
  while(1)
  {
    rc = qmi_client_get_service_list(service_obj, service_array, &num_entries,
        &num_services);

    if(rc != QMI_NO_ERR)
    {
      if(service_array)
      {
        FREE(service_array);
        service_array = NULL;
      }
      break;
    }

    if(num_entries == num_services)
      break;

    if(service_array)
      FREE(service_array);

    service_array = MALLOC(sizeof(qmi_service_info) * num_services);
    if(!service_array)
    {
      rc = QMI_INTERNAL_ERR;
      goto init_close_bail;
    }

    num_entries = num_services;
  }

  if(service_array)
  {
    for(i = 0; i < num_entries; i++)
    {
      qmi_cci_service_info *s = (qmi_cci_service_info *)&service_array[i];
      if(!memcmp(s->addr, svc->addr, clnt->xport->addr_len))
      {
        /* increment xport reference count without lock */
        clnt->xport_ref_cnt++;

  *user_handle = (qmi_client_type)clnt;
        FREE(service_array);
        return QMI_NO_ERR;
      }
    }
    FREE(service_array);
  }

  rc = QMI_SERVICE_ERR;

init_close_bail:
  /* no need to free clnt because it's done by the CLOSED event callback */
  xport_tbl[svc->xport].ops->close(clnt->xport->handle);
  return rc;

init_bail:
  FREE(clnt);
  return rc;
}

/*===========================================================================
  FUNCTION  qmi_client_send_raw_msg_async
===========================================================================*/
qmi_client_error_type
qmi_client_send_raw_msg_async
(
 qmi_client_type                   user_handle,
 unsigned int                      msg_id,
 void                              *req_buf,
 unsigned int                      req_buf_len,
 void                              *resp_buf,
 unsigned int                      resp_buf_len,
 qmi_client_recv_raw_msg_async_cb  resp_cb,
 void                              *resp_cb_data,
 qmi_txn_handle                    *txn_handle
 )
{
  qmi_cci_client_type *clnt = (qmi_cci_client_type *)user_handle;
  qmi_cci_txn_type *txn = NULL;
  uint32_t c_struct_len;
  unsigned char *buf;
  uint8_t dest_addr[MAX_ADDR_LEN];
  int rc;

  if(!clnt || !resp_buf || !txn_handle)
    return QMI_INTERNAL_ERR;

  *txn_handle = (qmi_txn_handle)NULL;

  if(!clnt->xport->ops || !clnt->xport->handle)
    return QMI_INTERNAL_ERR;

  rc = get_dest_addr(clnt, dest_addr);
  if(rc != QMI_NO_ERR)
    return rc;

  /* allocate output buffer */
  buf = MALLOC(req_buf_len + QMI_HEADER_SIZE);
  if(!buf)
    return QMI_INTERNAL_ERR;

  rc = qmi_client_get_txn(clnt, TXN_ASYNC_RAW, msg_id, resp_buf, resp_buf_len,
      resp_cb, NULL, resp_cb_data, &txn);

  if(rc != QMI_NO_ERR)
  {
    FREE(buf);
    return QMI_INTERNAL_ERR;
  }

  /* encode header and copy payload */
  encode_header(buf, QMI_REQUEST_CONTROL_FLAG, (uint16_t)txn->txn_id, 
      (uint16_t)msg_id, (uint16_t)req_buf_len);

  QMI_CCI_OS_LOG_TX(buf);

  if(req_buf)
    memcpy(buf + QMI_HEADER_SIZE, req_buf, req_buf_len);

  /* send message and return */
  rc = clnt->xport->ops->send(clnt->xport->handle, dest_addr, buf,
      req_buf_len + QMI_HEADER_SIZE);

  FREE(buf);

  if(rc != QMI_NO_ERR)
  {
    (void)remove_txn(clnt, txn);
    txn = NULL;
  }

  *txn_handle = (qmi_txn_handle)txn;
  return rc;
}

/*===========================================================================
  FUNCTION  qmi_client_send_msg_async
===========================================================================*/
qmi_client_error_type
qmi_client_send_msg_async
(
    qmi_client_type                 user_handle,
    unsigned int                    msg_id,
    void                            *req_c_struct,
    unsigned int                    req_c_struct_len,
    void                            *resp_c_struct,
    unsigned int                    resp_c_struct_len,
    qmi_client_recv_msg_async_cb    resp_cb,
    void                            *resp_cb_data,
    qmi_txn_handle                  *txn_handle
)
{
  qmi_cci_client_type *clnt = (qmi_cci_client_type *)user_handle;
  qmi_cci_txn_type *txn = NULL;
  int rc;
  uint8_t dest_addr[MAX_ADDR_LEN];

  if(!clnt || !resp_c_struct || !txn_handle)
    return QMI_INTERNAL_ERR;

  *txn_handle = (qmi_txn_handle)NULL;

  if(!clnt->xport->ops || !clnt->xport->handle)
    return QMI_INTERNAL_ERR;

  rc = get_dest_addr(clnt, dest_addr);
  if(rc != QMI_NO_ERR)
    return rc;

  rc = qmi_client_get_txn(clnt, TXN_ASYNC_MSG, msg_id, resp_c_struct,
      resp_c_struct_len, NULL, resp_cb, resp_cb_data, &txn);

  if(rc != QMI_NO_ERR)
    return QMI_INTERNAL_ERR;

  /* encode message */
  rc = encode_and_send(clnt, QMI_IDL_REQUEST, (uint16_t)txn->txn_id, 
      (uint16_t)msg_id, req_c_struct, req_c_struct_len, dest_addr);

  if(rc != QMI_NO_ERR)
  {
    (void)remove_txn(clnt, txn);
    txn = NULL;
  }

  *txn_handle = (qmi_txn_handle)txn;
  return rc;
}


/*===========================================================================
  FUNCTION  qmi_client_delete_async_txn
===========================================================================*/
qmi_client_error_type
qmi_client_delete_async_txn
(
  qmi_client_type  user_handle,
  qmi_txn_handle   async_txn_handle
)
{
  qmi_cci_client_type *clnt = (qmi_cci_client_type *)user_handle;
  qmi_cci_txn_type *txn;

  if(!clnt || !async_txn_handle)
    return QMI_INTERNAL_ERR;

  /* lock list and look for txn */
  LOCK(&clnt->txn_list_lock);
  txn = LIST_HEAD(clnt->txn_list);
  while(txn)
  {
    if(txn == (qmi_cci_txn_type *)async_txn_handle)
    {
      LIST_REMOVE(clnt->txn_list, txn, link);
      UNLOCK(&clnt->txn_list_lock);
      FREE(txn);
      return QMI_NO_ERR;
    }
    txn = txn->link.next;
  }
  UNLOCK(&clnt->txn_list_lock);

  return QMI_INVALID_TXN;
}


/*===========================================================================
  FUNCTION  qmi_client_send_raw_msg_sync
===========================================================================*/
qmi_client_error_type
qmi_client_send_raw_msg_sync
(
  qmi_client_type           user_handle,
  unsigned int              msg_id,
  void                      *req_buf,
  unsigned int              req_buf_len,
  void                      *resp_buf,
  unsigned int              resp_buf_len,
  unsigned int              *resp_buf_recv_len,
  unsigned int              timeout_msecs
)
{
  unsigned char *buf;
  qmi_cci_client_type *clnt = (qmi_cci_client_type *)user_handle;
  qmi_cci_txn_type *txn = NULL;
  int rc;
  uint8_t dest_addr[MAX_ADDR_LEN];

  if(!clnt || !req_buf || !resp_buf || !resp_buf_recv_len)
    return QMI_INTERNAL_ERR;

  if(!clnt->xport->ops || !clnt->xport->handle)
    return QMI_INTERNAL_ERR;

  rc = get_dest_addr(clnt, dest_addr);
  if(rc != QMI_NO_ERR)
    return rc;

  /* allocate output buffer */
  buf = MALLOC(req_buf_len + QMI_HEADER_SIZE);
  if(!buf)
    return QMI_INTERNAL_ERR;

  rc = qmi_client_get_txn(clnt, TXN_SYNC_RAW, msg_id, resp_buf, resp_buf_len,
      NULL, NULL, NULL, &txn);

  if(rc != QMI_NO_ERR)
  {
    FREE(buf);
    return QMI_INTERNAL_ERR;
  }

  /* encode header and copy payload */
  encode_header(buf, QMI_REQUEST_CONTROL_FLAG, (uint16_t)txn->txn_id, 
      (uint16_t)msg_id, (uint16_t)req_buf_len);
  QMI_CCI_OS_LOG_TX(buf);
  memcpy(buf + QMI_HEADER_SIZE, req_buf, req_buf_len);

  /* clear signal */
  QMI_CCI_OS_SIGNAL_CLEAR(&clnt->signal);
  txn->rc = QMI_TIMEOUT_ERR;

  /* send message */
  rc = clnt->xport->ops->send(clnt->xport->handle, dest_addr, buf,
      req_buf_len + QMI_HEADER_SIZE);

  FREE(buf);

  if(rc != QMI_NO_ERR)
  {
    /* if txn not found, we need to assume our copy of txn is valid */
    if(remove_txn(clnt, txn) != QMI_NO_ERR)
      FREE(txn);
    return QMI_SERVICE_ERR;
  }

  /* wait */
  QMI_CCI_OS_SIGNAL_WAIT(&clnt->signal, timeout_msecs);

  if(QMI_CCI_OS_SIGNAL_TIMED_OUT(&clnt->signal))
  {
    if(remove_txn(clnt, txn) != QMI_NO_ERR)
      FREE(txn);
    return QMI_TIMEOUT_ERR;
  }

  if(txn->rc == QMI_TIMEOUT_ERR)
  {
    /* we shouldn't be here, but we probably got here from an unexpected wakeup 
     */
    if(remove_txn(clnt, txn) != QMI_NO_ERR)
      FREE(txn);
    return QMI_INTERNAL_ERR;
  }

  *resp_buf_recv_len = txn->reply_len;

  rc = txn->rc;
  FREE(txn);

  return rc;
}

/*===========================================================================
  FUNCTION  qmi_client_send_msg_sync
===========================================================================*/
qmi_client_error_type
qmi_client_send_msg_sync
(
    qmi_client_type    user_handle,
    unsigned int       msg_id,
    void               *req_c_struct,
    unsigned int       req_c_struct_len,
    void               *resp_c_struct,
    unsigned int       resp_c_struct_len,
    unsigned int       timeout_msecs
)
{
  qmi_cci_client_type *clnt = (qmi_cci_client_type *)user_handle;
  qmi_cci_txn_type *txn = NULL;
  int rc;
  uint8_t dest_addr[MAX_ADDR_LEN];

  if(!clnt || !resp_c_struct)
    return QMI_INTERNAL_ERR;

  if(!clnt->xport->ops || !clnt->xport->handle)
    return QMI_INTERNAL_ERR;

  rc = get_dest_addr(clnt, dest_addr);
  if(rc != QMI_NO_ERR)
    return rc;

  rc = qmi_client_get_txn(clnt, TXN_SYNC_MSG, msg_id, resp_c_struct,
      resp_c_struct_len, NULL, NULL, NULL, &txn);

  if(rc != QMI_NO_ERR)
    return QMI_INTERNAL_ERR;

  /* clear signal */
  QMI_CCI_OS_SIGNAL_CLEAR(&clnt->signal);
  txn->rc = QMI_TIMEOUT_ERR;

  /* encode message */
  rc = encode_and_send(clnt, QMI_IDL_REQUEST, (uint16_t)txn->txn_id, 
      (uint16_t)msg_id, req_c_struct, req_c_struct_len, dest_addr);

  if(rc != QMI_NO_ERR)
  {
    /* if txn not found, we need to assume our copy of txn is valid */
    if(remove_txn(clnt, txn) != QMI_NO_ERR)
      FREE(txn);
    return rc;
  }

  /* wait */
  QMI_CCI_OS_SIGNAL_WAIT(&clnt->signal, timeout_msecs);

  if(QMI_CCI_OS_SIGNAL_TIMED_OUT(&clnt->signal))
  {
    if(remove_txn(clnt, txn) != QMI_NO_ERR)
      FREE(txn);
    return QMI_TIMEOUT_ERR;
  }

  if(txn->rc == QMI_TIMEOUT_ERR)
  {
    /* we shouldn't be here, but we probably got here from an unexpected wakeup
     */
    if(remove_txn(clnt, txn) != QMI_NO_ERR)
      FREE(txn);
    return QMI_INTERNAL_ERR;
  }

  rc = txn->rc;
  FREE(txn);

  return rc;
}

/*===========================================================================
  FUNCTION  qmi_client_release
===========================================================================*/
qmi_client_error_type
qmi_client_release
(
  qmi_client_type     user_handle
)
{
  qmi_cci_client_type *clnt = (qmi_cci_client_type *)user_handle;
  qmi_cci_txn_type *txn;
  uint32_t i;

  if(!clnt)
    return QMI_INTERNAL_ERR;

  /* free txns */
  LOCK(&clnt->txn_list_lock);
  txn = LIST_HEAD(clnt->txn_list);
  while(txn)
  {
    qmi_cci_txn_type *to_free = txn;
    txn = txn->link.next;
    FREE(to_free);
  }
  UNLOCK(&clnt->txn_list_lock);

  QMI_CCI_OS_SIGNAL_CLEAR(&clnt->signal);

  /* close xport without freeing the data structure until qmi_cci_xport_closed
   * is called to prevent callbacks from happening after freeing the client
   */
  for(i = 0; i < num_xports; i++)
    if(clnt->xport[i].ops && clnt->xport[i].handle)
      clnt->xport[i].ops->close(clnt->xport[i].handle);

  QMI_CCI_OS_SIGNAL_WAIT(&clnt->signal, 0);

  QMI_CCI_OS_SIGNAL_DEINIT(&clnt->signal);
  if(clnt->ext_signal)
  {
    QMI_CCI_OS_SIGNAL_DEINIT(clnt->ext_signal);
  }

  LOCK_DEINIT(&clnt->xport_ref_lock);
  LOCK_DEINIT(&clnt->err_cb_lock);
  LOCK_DEINIT(&clnt->txn_list_lock);
  LOCK_DEINIT(&clnt->server_addr_valid_lock);
  FREE(clnt);
  return QMI_NO_ERR;
}



/*===========================================================================
  FUNCTION  qmi_client_message_encode
===========================================================================*/
qmi_client_error_type
qmi_client_message_encode
(
    qmi_client_type                      user_handle,
    qmi_idl_type_of_message_type         req_resp_ind,
    unsigned int                         message_id,
    const void                           *p_src,
    unsigned int                         src_len,
    void                                 *p_dst,
    unsigned int                         dst_len,
    unsigned int                         *dst_encoded_len
)
{
  if(!user_handle)
    return QMI_INTERNAL_ERR;

  return (
      qmi_idl_message_encode(
        ((qmi_cci_client_type *)user_handle)->service_obj,
        req_resp_ind,
        (uint16_t)message_id,
        p_src,
        src_len,
        p_dst,
        dst_len,
        (uint32_t*)dst_encoded_len)
      );
}

/*===========================================================================
  FUNCTION  qmi_client_message_decode
===========================================================================*/
qmi_client_error_type
qmi_client_message_decode
(
    qmi_client_type                         user_handle,
    qmi_idl_type_of_message_type            req_resp_ind,
    unsigned int                            message_id,
    const void                              *p_src,
    unsigned int                            src_len,
    void                                    *p_dst,
    unsigned int                            dst_len
)
{
  if(!user_handle)
    return QMI_INTERNAL_ERR;

  return (
      qmi_idl_message_decode(
        ((qmi_cci_client_type *)user_handle)->service_obj,
        req_resp_ind,
        (uint16_t)message_id,
        p_src,
        src_len,
        p_dst,
        dst_len)
      );
}

/*===========================================================================
  FUNCTION  qmi_client_get_service_list
===========================================================================*/
qmi_client_error_type
qmi_client_get_service_list
(
 qmi_idl_service_object_type   service_obj,
 qmi_service_info       *service_info_array,
 unsigned int *num_entries,
 unsigned int *num_services
 )
{
  qmi_cci_service_info *svc = (qmi_cci_service_info *)service_info_array;
  unsigned int i, service_id, idl_version, to_fill = 0, filled = 0;
  int rc;

  if(!num_services)
    return QMI_INTERNAL_ERR;

  *num_services = 0;

  if(num_entries && *num_entries && svc)
  {
    to_fill = *num_entries;
    *num_entries = 0;
  }

  /* Extract service id */
  rc =  qmi_idl_get_service_id(service_obj, &service_id);
  if (rc !=  QMI_IDL_LIB_NO_ERR)
    return QMI_INTERNAL_ERR;

  /* Get IDL version */
  rc =  qmi_idl_get_idl_version(service_obj, &idl_version);
  if (rc !=  QMI_IDL_LIB_NO_ERR)
    return QMI_INTERNAL_ERR;

  /* go through all the xports and find the service */
  for(i = 0; i < num_xports; i++)
  {
    /* look up server address */
    unsigned int entries = to_fill - filled;
    (*num_services) += xport_tbl[i].ops->lookup(xport_tbl[i].xport_data, 
        (uint8_t)i, service_id, idl_version, entries ? &entries : NULL, 
        entries ? &svc[filled] : NULL);
    filled += entries;
  }

  if (num_entries)
    (*num_entries) = filled;

  return *num_services ? QMI_NO_ERR : QMI_SERVICE_ERR;
}

/*===========================================================================
  FUNCTION  qmi_client_get_any_service
===========================================================================*/
qmi_client_error_type
qmi_client_get_any_service
(
 qmi_idl_service_object_type service_obj,
 qmi_service_info *service_info
 )
{
  unsigned int num_entries = 1, num_services;

  if(!service_info)
    return QMI_INTERNAL_ERR;

  return qmi_client_get_service_list(service_obj, service_info, &num_entries,
      &num_services);
}

/*===========================================================================
  FUNCTION  qmi_client_get_service_instance
===========================================================================*/
qmi_client_error_type
qmi_client_get_service_instance
(
 qmi_idl_service_object_type service_obj,
 qmi_service_instance instance_id,
 qmi_service_info *service_info
 )
{
  unsigned int num_entries, num_services, i;
  qmi_client_error_type rc;
  qmi_service_info *service_array;

  if(!service_info)
    return QMI_INTERNAL_ERR;

  rc = qmi_client_get_service_list(service_obj, NULL, NULL, &num_services);
  if(rc != QMI_NO_ERR)
    return rc;

  service_array = MALLOC(sizeof(qmi_service_info) * num_services);
  if(!service_array)
    return QMI_INTERNAL_ERR;

  num_entries = num_services;
  rc = qmi_client_get_service_list(service_obj, service_array, &num_entries,
      &num_services);
  if(rc != QMI_NO_ERR)
    goto free_bail;

  rc = QMI_SERVICE_ERR;
  for(i = 0; i < num_entries; i++)
  {
    qmi_cci_service_info *svc = (qmi_cci_service_info *)&service_array[i];
    if(svc->instance == instance_id)
    {
      memcpy(service_info, svc, sizeof(qmi_service_info));
      rc = QMI_NO_ERR;
      break;
    }
  }

free_bail:
  FREE(service_array);
  return rc;
}

/*===========================================================================
  FUNCTION  qmi_client_get_instance_id
===========================================================================*/
qmi_client_error_type
qmi_client_get_instance_id
(
 qmi_service_info *service_info,
 qmi_service_instance *instance_id
 )
{
  qmi_cci_service_info *svc = (qmi_cci_service_info *)service_info;

  if(!svc || !instance_id)
    return QMI_INTERNAL_ERR;

  *instance_id = svc->instance;
  return QMI_NO_ERR;
}

/*===========================================================================
  FUNCTION  qmi_client_register_error_cb
===========================================================================*/
qmi_client_error_type qmi_client_register_error_cb
(
 qmi_client_type user_handle,
 qmi_client_error_cb err_cb,
 void *err_cb_data
 )
{
  qmi_cci_client_type *clnt = (qmi_cci_client_type *)user_handle;
  unsigned int err_pending;

  if(!clnt || !err_cb)
    return QMI_INTERNAL_ERR;


  LOCK(&clnt->err_cb_lock);
  clnt->err_cb = err_cb;
  clnt->err_cb_data = err_cb_data;
  err_pending = clnt->err_pending;
  UNLOCK(&clnt->err_cb_lock);

  if(err_pending)
  {
    err_cb((qmi_client_type)clnt, QMI_SERVICE_ERR, err_cb_data);
    return QMI_SERVICE_ERR;
  }

  return QMI_NO_ERR;
}

/*==========================================================================
  FUNCTION  qmi_client_get_async_txn_id
===========================================================================*/
/*!
@brief

  Gets a transaction id from the transaction handle

@return
  qmi_client_error_type

@note
  This API is added to support the legacy messages that needs access to the
  transcation ID.
*/
/*=========================================================================*/
qmi_client_error_type
qmi_client_get_async_txn_id
(
 qmi_client_type  user_handle,
 qmi_txn_handle   async_txn_handle,
 uint32_t *txn_id
)
{
  qmi_cci_txn_type *i;
  qmi_cci_client_type *clnt = (qmi_cci_client_type *)user_handle;
  qmi_cci_txn_type *txn = (qmi_cci_txn_type *)async_txn_handle;

  if (!txn_id || !clnt || !txn )
    return QMI_INTERNAL_ERR;

  *txn_id = 0;
  LOCK(&clnt->txn_list_lock);
  i = LIST_HEAD(clnt->txn_list);
  while(i)
  {
    if (i == txn)
    {
      *txn_id = i->txn_id;
      break;
    }
    i = i->link.next;
  }
  UNLOCK(&clnt->txn_list_lock);
  return ((i == NULL ) ? QMI_INVALID_TXN : QMI_NO_ERR);
}
