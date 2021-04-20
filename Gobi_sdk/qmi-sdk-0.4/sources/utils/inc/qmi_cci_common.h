/******************************************************************************
  ---------------------------------------------------------------------------
  Copyright (c) 2011-2012 QUALCOMM Technologies Incorporated. All Rights Reserved.
  QUALCOMM Proprietary and Confidential.
  ---------------------------------------------------------------------------
*******************************************************************************/

#ifndef QMI_CCI_COMMON_H
#define QMI_CCI_COMMON_H

#include "qmi_common.h"

#define MAX_XPORTS 4

struct qmi_cci_txn_s;
struct qmi_cci_xport_ops_s;
typedef struct qmi_cci_client_s
{
  /* service object */
  qmi_idl_service_object_type service_obj;

  /* xport data */
  struct
  {
    struct qmi_cci_xport_ops_s *ops;
    void *handle;
    uint32_t addr_len;
  } xport[MAX_XPORTS];

  /* xport reference count */
  qmi_cci_lock_type xport_ref_lock;
  uint32_t xport_ref_cnt;

  /* indication callback */
  qmi_client_ind_cb ind_cb;
  void *ind_cb_data;

  /* error callback */
  qmi_cci_lock_type err_cb_lock;
  qmi_client_error_cb err_cb;
  void *err_cb_data;
  uint32_t err_pending;

  /* server address */
  uint8_t server_addr[MAX_ADDR_LEN];

  /* Flag indicating that the server address is valid */
  uint8_t server_addr_valid;
  qmi_cci_lock_type server_addr_valid_lock;

  /* list of outstanding transactions */
  qmi_cci_lock_type txn_list_lock;
  LIST(struct qmi_cci_txn_s, txn_list);

  /* next txn_id */
  uint16_t next_txn_id;

  /* OS-defined data such as signal/event */
  QMI_CCI_OS_SIGNAL signal;

  /* pointer to external signal, if provided */
  QMI_CCI_OS_SIGNAL *ext_signal;

} qmi_cci_client_type;

typedef enum
{
  TXN_SYNC_MSG,
  TXN_SYNC_RAW,
  TXN_ASYNC_MSG,
  TXN_ASYNC_RAW
} qmi_cci_txn_enum_type;

typedef struct qmi_cci_txn_s
{
  /* links to prev and next txns */
  LINK(struct qmi_cci_txn_s, link);

  /* type of txn */
  qmi_cci_txn_enum_type type;

  /* txn and msg ids */
  uint32_t txn_id;
  uint32_t msg_id;

  /* raw and message async rx cb */
  qmi_client_recv_raw_msg_async_cb raw_async_rx_cb;
  qmi_client_recv_msg_async_cb     msg_async_rx_cb;
  void                            *rx_cb_data;

  uint8_t *rx_buf;
  uint32_t rx_buf_len;
  uint32_t reply_len;

  /* return code */
  int32_t rc;
} qmi_cci_txn_type;

typedef struct
{
  uint8_t xport;
  uint8_t version;
  uint8_t instance;
  uint8_t reserved;
  uint8_t addr[MAX_ADDR_LEN];
} qmi_cci_service_info;

/*=============================================================================
     Transport abstraction prototypes - infrastructure to xport down-calls
=============================================================================*/

/*=============================================================================
  CALLBACK FUNCTION qmi_cci_open_fn_type
=============================================================================*/
/*!
@brief
  This callback function is called by the QCCI infrastructure to open a new
  transport

@param[in]   xport_data         Opaque parameter to the xport (e.g. port ID)
@param[in]   clnt               Pointer to infrastructure's client struct.
                                Can be treated as opaque, but prototyped for
                                ease of debugging.
@param[in]   service_id         Service ID
@param[in]   version            Version of the service
@param[in]   addr               Address of the server
@param[in]   max_rx_len         Maximum length of messages that can be
                                received. Used by xport to allocate a buffer
                                if the underlyin transport cannot pass the
                                message through a callback.

@retval      Opaque handle to the transport. NULL on failure.

*/
/*=========================================================================*/
typedef void *(*qmi_cci_open_fn_type)
  (
   void *xport_data,
   qmi_cci_client_type *clnt,
   uint32_t service_id,
   uint32_t version,
   void *addr,
   uint32_t max_rx_len
  );

/*=============================================================================
  CALLBACK FUNCTION qmi_cci_send_fn_type
=============================================================================*/
/*!
@brief
  This callback function is called by the QCCI infrastructure to send data to
  a server.

@param[in]   handle             Opaque handle returned by the open call
@param[in]   addr               Opaque address sent to the infrastructure
                                through the connect or recv calls.
@param[in]   msg                Pointer to message to be sent
@param[in]   msg_len            Length of the message

@retval      QMI_NO_ERR         Success

*/
/*=========================================================================*/
typedef qmi_client_error_type (*qmi_cci_send_fn_type)
  (
   void *handle,
   void *addr,
   uint8_t *msg,
   uint32_t msg_len
  );

/*=============================================================================
  CALLBACK FUNCTION qmi_cci_close_fn_type
=============================================================================*/
/*!
@brief
  This callback function is called by the QCCI infrastructure to close the
  transport usually when the service is unregistered. It is crucial that the
  xport synchronize the deallocation of memory and its callback functions so
  the callbacks do not occur after the data associated with the xport has been
  deallocated.

@param[in]   handle              Opaque handle returned by the open call

*/
/*=========================================================================*/
typedef void (*qmi_cci_close_fn_type)
  (
   void *handle
  );

/*=============================================================================
  CALLBACK FUNCTION qmi_cci_open_fn_type
=============================================================================*/
/*!
@brief
  This callback function is called by the QCCI infrastructure to open a new
  transport

@param[in]      xport_data         Opaque data associated with the transport
@param[in]      xport_num          Framework assigned enumeration of the xport
@param[in]   service_id         Service ID
@param[in]   version            Version of the service
@param[in/out]  num_entries        Number of entries in the array and number
                                   of entries filled.
@param[out]  service_list       Linked list of server records

@retval         Total number of servers found

*/
/*=========================================================================*/
typedef uint32_t (*qmi_cci_lookup_fn_type)
  (
   void    *xport_data,
   uint8_t  xport_num,
   uint32_t service_id,
   uint32_t version,
   uint32_t *num_entries,
   qmi_cci_service_info *service_info
  );

/*=============================================================================
  CALLBACK FUNCTION qmi_cci_addr_len_fn_type
=============================================================================*/
/*!
@brief
  This callback function is called by the QCCI infrastructure to retrieve the
  length of the (destination) address of the xport.

@retval         Length of address

*/
/*=========================================================================*/
typedef uint32_t (*qmi_cci_addr_len_fn_type)
  (
   void
  );

/* xport ops table type */
typedef struct qmi_cci_xport_ops_s
{
  qmi_cci_open_fn_type open;
  qmi_cci_send_fn_type send;
  qmi_cci_close_fn_type close;
  qmi_cci_lookup_fn_type lookup;
  qmi_cci_addr_len_fn_type addr_len;
} qmi_cci_xport_ops_type;

/*=============================================================================
                        Xport to infrastructure up-calls
=============================================================================*/
/*=============================================================================
  FUNCTION qmi_cci_xport_start
=============================================================================*/
/*!
@brief
  This function is used to register a transport with the infrastructure

@param[in]   ops                Pointer to transport operations table
@param[in]   xport_data         Opaque data associated with the transport,
                                such as port ID or other parameters.
*/
/*=========================================================================*/
void qmi_cci_xport_start
(
 qmi_cci_xport_ops_type *ops,
 void *xport_data
 );

/*=============================================================================
  FUNCTION qmi_cci_xport_recv
=============================================================================*/
/*!
@brief
  This function is used by the transport to signal the infrastructure to
  process the incoming message, one at a time.

@param[in]   clnt               Pointer to infrastructure's client struct
@param[in]   addr               Pointer to source address.
@param[in]   buf                Pointer to message to be received
@param[in]   len                Length of the message

@retval      QMI_CSI_NO_ERR     Success
*/
/*=========================================================================*/
qmi_client_error_type qmi_cci_xport_recv
(
 qmi_cci_client_type *clnt,
 void *addr,
 uint8_t *buf,
 uint32_t len
 );

/*=============================================================================
  FUNCTION qmi_cci_xport_closed
=============================================================================*/
/*!
@brief
  This function is used by the transport to signal the infrastructure after
  the transport is fully closed so the infrastructure can free up client's
  data structure.

@param[in]   clnt               Pointer to infrastructure's client struct

*/
/*=========================================================================*/
void qmi_cci_xport_closed
(
 qmi_cci_client_type *clnt
 );

/*=============================================================================
  FUNCTION qmi_cci_xport_event_new_server
=============================================================================*/
/*!
@brief
  This function is used by the transport to signal the infrastructure of a new
  server registration event. Client can query lookup for the new server

@param[in]   clnt               Pointer to infrastructure's client struct
@param[in]   addr               Pointer to source address.

*/
/*=========================================================================*/
void qmi_cci_xport_event_new_server
(
 qmi_cci_client_type *clnt,
 void *addr
 );

/*=============================================================================
  FUNCTION qmi_cci_xport_event_remove_server
=============================================================================*/
/*!
@brief
  This function is used by the transport to signal the infrastructure of a
  server unregistration event.

@param[in]   clnt               Pointer to infrastructure's client struct
@param[in]   addr               Pointer to source address.

*/
/*=========================================================================*/
void qmi_cci_xport_event_remove_server
(
 qmi_cci_client_type *clnt,
 void *addr
 );


#endif
