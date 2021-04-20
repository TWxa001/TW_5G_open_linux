#ifndef QMI_CLIENT_H
#define QMI_CLIENT_H
/******************************************************************************
  @file    qmi_client.h
  @brief   The QMI common client Header file.

  DESCRIPTION
  QMI common client routines.  All client will be build on top of these
  routines for initializing, sending messages and receiving responses/
  indications.

  INITIALIZATION AND SEQUENCING REQUIREMENTS
  qmi_client_init() needs to be called before sending or receiving of any
  service specific messages

  ---------------------------------------------------------------------------
  Copyright (c) 2011 QUALCOMM Technologies Incorporated. All Rights Reserved.
  QUALCOMM Proprietary and Confidential.
  ---------------------------------------------------------------------------
*******************************************************************************/
#include "qmi_idl_lib.h"
#include "qmi_cci_target_ext.h"

#define QMI_NO_ERR                0
#define QMI_INTERNAL_ERR          (-1)
#define QMI_SERVICE_ERR           (-2)
#define QMI_TIMEOUT_ERR           (-3)
#define QMI_EXTENDED_ERR          (-4)
#define QMI_PORT_NOT_OPEN_ERR     (-5)
#define QMI_MEMCOPY_ERROR         (-13)
#define QMI_INVALID_TXN           (-14)
#define QMI_CLIENT_ALLOC_FAILURE  (-15)

typedef struct         qmi_client_struct *qmi_client_type;
typedef int            qmi_client_error_type;
typedef unsigned int   qmi_txn_handle;

/* structure for storing address information of the servers returned by
 * qmi_client_get_service_list()
 */
typedef struct {
  unsigned int info[5];
}qmi_service_info;

typedef unsigned int qmi_service_instance;

/*============================================================================
                            CALLBACK FUNCTIONS

  Asynchronous callback function prototype.  Individual services will
  register functions of this prototype when they send an asynchronous
  message

============================================================================*/

/*=============================================================================
  CALLBACK FUNCTION qmi_client_recv_raw_msg_async_cb
=============================================================================*/
/*!
@brief
  This callback function is called by the QCCI infrastructure when a response
  is received after a request is sent using qmi_client_send_raw_msg_async()


@param[in]   user_handle        Handle used by the infrastructure to
                                identify different clients.
@param[in]   msg_id             Message ID
@param[in]   resp_buf           Pointer to the response
@param[in]   resp_buf_len       Length of the response
@param[in]   resp_cb_data       User-data
@param[in]   transp_err         Error code

*/
/*=========================================================================*/
typedef void (*qmi_client_recv_raw_msg_async_cb)
(
  qmi_client_type                user_handle,
  unsigned int                   msg_id,
  void                           *resp_buf,
  unsigned int                   resp_buf_len,
  void                           *resp_cb_data,
  qmi_client_error_type          transp_err
);

/*=============================================================================
  CALLBACK FUNCTION qmi_client_recv_msg_async_cb
=============================================================================*/
/*!
@brief
  This callback function is called by the QCCI infrastructure when a response
  is received after a request is sent using qmi_client_send_msg_async()


@param[in]   user_handle        Handle used by the infrastructure to
                                identify different clients.
@param[in]   msg_id             Message ID
@param[in]   resp_c_struct      Pointer to the response
@param[in]   resp_c_struct_len  Length of the response
@param[in]   resp_cb_data       User-data
@param[in]   transp_err         Error code

*/
/*=========================================================================*/
typedef void (*qmi_client_recv_msg_async_cb)
(
  qmi_client_type                user_handle,
  unsigned int                   msg_id,
  void                           *resp_c_struct,
  unsigned int                   resp_c_struct_len,
  void                           *resp_cb_data,
  qmi_client_error_type          transp_err
);

/*=============================================================================
  CALLBACK FUNCTION qmi_client_recv_msg_async_cb
=============================================================================*/
/*!
@brief
  This callback function is called by the QCCI infrastructure when an
  indication is received. This callback is registered at initialization.

@param[in]   user_handle        Handle used by the infrastructure to
                                identify different clients.
@param[in]   msg_id             Message ID
@param[in]   ind_buf            Pointer to the raw/undecoded indication
@param[in]   ind_buf_len        Length of the indication
@param[in]   resp_cb_data       User-data

*/
/*=========================================================================*/
typedef void (*qmi_client_ind_cb)
(
 qmi_client_type                user_handle,
 unsigned int                   msg_id,
 void                           *ind_buf,
 unsigned int                   ind_buf_len,
 void                           *ind_cb_data
);

/*=============================================================================
  CALLBACK FUNCTION qmi_client_error_cb
=============================================================================*/
/*!
@brief
  This callback function is called by the QCCI infrastructure when the service
  terminates or deregisters

@param[in]   user_handle        Handle used by the infrastructure to
                                identify different clients.
@param[in]   error              Error code
@param[in]   err_cb_data        User-data

*/
/*=========================================================================*/
typedef void (*qmi_client_error_cb)
(
 qmi_client_type user_handle,
 qmi_client_error_type error,
 void *err_cb_data
);

/*===========================================================================
  FUNCTION  qmi_client_notifier_init
===========================================================================*/
/*!
@brief

  This function is used for initializing a notifier with a service object.
  When a new service is registered supporting the service_obj, the signal
  or event object specified in os_params will be set.

@param[in]   service_obj        Service object
@param[in]   os_params          OS-specific parameters. It can be a pointer
                                to event object, or signal mask and TCB
@param[out]  user_handle        Handle used by the infrastructure to
                                identify different clients.
@return

  Sets the user_handle and returns QMI_NO_ERR if successful,
  error code if not successful

@note

  - Dependencies
    - None

  - Side Effects
    - A client is created and the signal will be set when a remote server is
      registered
*/
/*=========================================================================*/
extern qmi_client_error_type
qmi_client_notifier_init
(
 qmi_idl_service_object_type               service_obj,
 qmi_client_os_params                      *os_params,
 qmi_client_type                           *user_handle
 );

/*===========================================================================
  FUNCTION  qmi_client_init
===========================================================================*/
/*!
@brief

  This function is used for initializing a connection to a service.

@param[in]   service_info       Pointer to an entry in the service_info array
                                returned by qmi_client_get_service_list()
@param[in]   service_obj        Service object
@param[in]   qmi_client_ind_cb  Indication callback function
@param[in]   ind_cb_data        Indicatino callback user-data
@param[in]   os_params          OS-specific parameters. It can be a pointer
                                to event object, or signal mask and TCB
@param[out]  user_handle        Handle used by the infrastructure to
                                identify different clients.
@return
  Sets the user_handle and returns QMI_NO_ERR if successful,
  error code if not successful
  QMI_SERVICE_ERR if server cannot be reached

@note

  - Dependencies
    - None

  - Side Effects
    - QMI connection is opened with the service
*/
/*=========================================================================*/
extern  qmi_client_error_type
qmi_client_init
(
 qmi_service_info                          *service_info,
 qmi_idl_service_object_type               service_obj,
 qmi_client_ind_cb                         ind_cb,
 void                                      *ind_cb_data,
 qmi_client_os_params                      *os_params,
 qmi_client_type                           *user_handle
);



/*===========================================================================
  FUNCTION  qmi_client_send_raw_msg_async
===========================================================================*/
/*!
@brief
  Sends an asynchronous QMI service message on the specified connection.
  The caller is expected to encode the message before sending it through
  this function.

@param[in]   user_handle        Handle used by the infrastructure to
@param[in]   msg_id             Message ID
@param[in]   req_buf            Pointer to the request
@param[in]   req_buf_len        Length of the request
@param[in]   resp_buf           Pointer to where the response will be stored
@param[in]   resp_buf_len       Length of the response buffer
@param[in]   resp_cb            Callback function to handle the response
@param[in]   resp_cb_data       Callback user-data
@param[out]  txn_handle         Handle used to identify the transaction

@return
  QMI_NO_ERR and sets the transaction handle if function is successful,
  error code otherwise.
  QMI_SERVICE_ERR if server cannot be reached
@note

  - Dependencies
    - None

  - Side Effects
*/
/*=========================================================================*/
extern qmi_client_error_type
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
);


/*===========================================================================
  FUNCTION  qmi_client_send_msg_async
===========================================================================*/
/*!
@brief
  Sends an asynchronous QMI service message on the specified connection.
  The callback functi is expected to encode the message before sending it
  through this function.

@param[in]   user_handle        Handle used by the infrastructure to
@param[in]   msg_id             Message ID
@param[in]   req_c_struct       Pointer to the request
@param[in]   req_c_struct_len   Length of the request
@param[in]   resp_c_struct      Pointer to where the response will be stored
@param[in]   resp_c_struct_len  Length of the response buffer
@param[in]   resp_cb            Callback function to handle the response
@param[in]   resp_cb_data       Cablback user-data
@param[out]  txn_handle         Handle used to identify the transaction


@return
  QMI_NO_ERR and sets the transaction handle if function is successful,
  error code otherwise.
  QMI_SERVICE_ERR if server cannot be reached

@note

  - Dependencies
    - None

  - Side Effects
*/
/*=========================================================================*/

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
);


/*===========================================================================
  FUNCTION  qmi_client_delete_async_txn
===========================================================================*/
/*!
@brief
  Deletes an asynchronous transaction so that it will free resources
  associated with transaction

@param[in]   user_handle        Handle used by the infrastructure to
@param[in]   txn_handle         Handle used to identify the transaction as
                                returned by the send async functions
@return
   QMI_NO_ERR if successful, error code otherwise
@note

  - Dependencies
    - None

  - Side Effects
    - Async response will not be delivered

  Users should be aware of the potential race condition where an
  asynchronous response may be in the process of being handled
  by the "users_rsp_cb" callback up until this routine returns.
*/
/*=========================================================================*/
extern qmi_client_error_type
qmi_client_delete_async_txn
(
 qmi_client_type  user_handle,
 qmi_txn_handle   async_txn_handle
);


/*===========================================================================
  FUNCTION  qmi_client_send_raw_msg_sync
===========================================================================*/
/*!
@brief
  Sends a synchronous QMI service message on the specified connection.
  This function expects the user to encode the message before sending
  and decode the message after receiving.

@param[in]   user_handle        Handle used by the infrastructure to
@param[in]   msg_id             Message ID
@param[in]   req_buf            Pointer to the request
@param[in]   req_buf_len        Length of the request
@param[in]   resp_buf           Pointer to where the response will be stored
@param[in]   resp_buf_len       Length of the response buffer
@param[in]   resp_buf_recv_len  Length of the response received
@param[in]   timeout_msecs      Time-out in milliseconds. 0 = no timeout

@return
  QMI_NO_ERR and sets resp_buf_recv_len if function is successful,
  error code otherwise.
  QMI_SERVICE_ERR if server cannot be reached

@note
  - Dependencies
    - None

  - Side Effects
    - None
*/
/*=========================================================================*/
extern qmi_client_error_type
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
);


/*===========================================================================
  FUNCTION  qmi_client_send_msg_sync
===========================================================================*/
/*!
@brief
  Sends a synchronous QMI service message on the specified connection.
  This function provides the encoding/decoding functionality and
  user gets the decoded data in the response structure provided.

@param[in]   user_handle        Handle used by the infrastructure to
@param[in]   msg_id             Message ID
@param[in]   req_c_struct       Pointer to the request
@param[in]   req_c_struct_len   Length of the request
@param[in]   resp_c_struct      Pointer to where the response will be stored
@param[in]   resp_c_struct_len  Length of the response buffer
@param[in]   timeout_msecs      Time-out in milliseconds. 0 = no timeout

@return
  QMI_NO_ERR if function is successful,
  error code otherwise.
  QMI_SERVICE_ERR if server cannot be reached

@note
  - Dependencies
    - None

  - Side Effects
    - None
*/
/*=========================================================================*/
extern qmi_client_error_type
qmi_client_send_msg_sync
(
 qmi_client_type    user_handle,
 unsigned int       msg_id,
 void               *req_c_struct,
 unsigned int       req_c_struct_len,
 void               *resp_c_struct,
 unsigned int       resp_c_struct_len,
 unsigned int       timeout_msecs
);
/*===========================================================================
  FUNCTION  qmi_client_release
===========================================================================*/
/*!
@brief
  Releases the connection to a service.

@param[in]   user_handle        Handle used by the infrastructure to

@return
  QMI_NO_ERR if function is successful, error code otherwise.

@note

  - Dependencies
    - None

  - Side Effects
   - None
*/
/*=========================================================================*/
extern qmi_client_error_type
qmi_client_release
(
 qmi_client_type     user_handle
);



/*===========================================================================
  FUNCTION  qmi_client_message_encode
===========================================================================*/
/*!
@brief
  Encodes the body (TLV's) of a QMI message from the C data structure to the
  wire format.
@param[in]  user_handle   opaque handle
                          object accessor function from service header file.
@param[in]  message_type  The type of message: request, response, or indication.
@param[in]  message_id    Message ID from IDL.
@param[in]  p_src         Pointer to C structure containing message data.
@param[in]  src_len       Length (size) of p_src C structure in bytes.
@param[out] p_dst         Pointer to beginning of first TLV in message.
@param[in]  dst_len       Length of p_dst buffer in bytes.
@param[out] dst_decoded_len Pointer to the return value, the length of
                          encoded message (to be filled in as the length
                          field in the QMI header).

@retval    QMI_NO_ERR     Success
@retval    QMI_IDL_...    Error, see error codes defined in qmi.h*/
/*=========================================================================*/
extern qmi_client_error_type
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
);

/*===========================================================================
  FUNCTION  qmi_client_message_decode
===========================================================================*/
/*!
@brief
  Decodes the body (TLV's) of a QMI message body from the wire format to the
  C structure.

@param[in]  user_handle   opaque  handle
                          object accessor function from service header file.
@param[in]  message_type  The type of message: request, response, or indication.
@param[in]  message_id    Message ID from IDL.
@param[in]  p_src         Pointer to beginning of first TLV in message.
@param[in]  src_len       Length of p_src buffer in bytes.
@param[out] p_dst         Pointer to C structure for decoded data
@param[in]  dst_len       Length (size) of p_dst C structure in bytes.

@retval    QMI_NO_ERR     Success
@retval    QMI_IDL_...    Error, see error codes defined in qmi.h
*/
/*=========================================================================*/
extern qmi_client_error_type
qmi_client_message_decode
(
 qmi_client_type                         user_handle,
 qmi_idl_type_of_message_type            req_resp_ind,
 unsigned int                            message_id,
 const void                              *p_src,
 unsigned int                            src_len,
 void                                    *p_dst,
 unsigned int                            dst_len
);

/*===========================================================================
  FUNCTION  qmi_client_get_service_list
===========================================================================*/
/*!
@brief
   Retrieves a list of services capable of handling the service object
   @return
   QMI_NO_ERR if function is successful, error code otherwise.

@param[in]      service_obj         Service object
@param[out]     service_info_array  Array to fill
@param[in/out]  num_entries         Number of entries in the array as input
                                    Number of entires filled as output
@param[out]     num_services        Number of known services. If num_services
                                    > num_entries, a larger array may be needed

@note

  - Dependencies
    - None

  - Side Effects
   - service_info_array may be filled
*/
/*=========================================================================*/

qmi_client_error_type
qmi_client_get_service_list
(
 qmi_idl_service_object_type   service_obj,
 qmi_service_info       *service_info_array,
 unsigned int *num_entries,
 unsigned int *num_services
);

/*===========================================================================
  FUNCTION  qmi_client_get_any_service
===========================================================================*/
/*!
@brief
   Retrieves a service capable of handling the service object

@return
   QMI_NO_ERR if function is successful, error code otherwise.

@param[in]      service_obj         Service object
@param[out]     service_info        Service information

@note

  - Dependencies
    - None

  - Side Effects
   - service_info may be written
*/
/*=========================================================================*/
qmi_client_error_type
qmi_client_get_any_service
(
 qmi_idl_service_object_type service_obj,
 qmi_service_info *service_info
);

/*===========================================================================
  FUNCTION  qmi_client_get_service_instance
===========================================================================*/
/*!
@brief
   Retrieves a service with a specific instance ID capable of handling the
   service object

@return
   QMI_NO_ERR if function is successful, error code otherwise.

@param[in]      service_obj         Service object
@param[in]      instance_id         Instance ID of the service
@param[out]     service_info        Service information

@note

  - Dependencies
    - None

  - Side Effects
   - service_info may be written
*/
/*=========================================================================*/
qmi_client_error_type
qmi_client_get_service_instance
(
 qmi_idl_service_object_type service_obj,
 qmi_service_instance instance_id,
 qmi_service_info *service_info
);

/*===========================================================================
  FUNCTION  qmi_client_get_instance_id
===========================================================================*/
/*!
@brief
  Accessor function for obtaining the instance ID for a specific service_info
  entry

@param[in]   service_info       Pointer to an entry in the service_info array
@param[out]  instance_id        Instance ID of the service_info entry

@return
  Sets the user_handle and returns QMI_NO_ERR if successful,
  error code if not successful
*/
/*=========================================================================*/
qmi_client_error_type
qmi_client_get_instance_id
(
 qmi_service_info *service_info,
 qmi_service_instance *instance_id
);

/*===========================================================================
  FUNCTION  qmi_client_register_error_cb
===========================================================================*/
/*!
@brief
  Register an service error callback with the user handle

@param[in]  user_handle   opaque handle
                          object accessor function from service header file.
@param[in]  err_cb        Pointer to callback function
@param[in]  err_cb_data   User-data

@retval  QMI_NO_ERR       Success
         QMI_SERVICE_ERR  Server has gone down and the callback is invoked
         QMI_INTERNAL_ERR Invalid input parameters
*/
/*=========================================================================*/
qmi_client_error_type qmi_client_register_error_cb
(
 qmi_client_type user_handle,
 qmi_client_error_cb err_cb,
 void *err_cb_data
);

#endif /* QMI_CLIENT_H  */
