#ifndef COMMON_SERVICE_H
#define COMMON_SERVICE_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                        C O M M O N _ V 0 1  . H

GENERAL DESCRIPTION
  This is the public header file which defines the common service Data structures.

 Copyright (c) 2011 QUALCOMM Technologies Incorporated.  All Rights Reserved.
 QUALCOMM Proprietary and Confidential.

 *====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
 *THIS IS AN AUTO GENERATED FILE. DO NOT ALTER IN ANY WAY
 *====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* This file was generated with Tool version 2.5
   It was generated on: Tue Apr 12 2011
   From IDL File: common_v01.idl */

/** @defgroup common_qmi_consts Constant values defined in the IDL */
/** @defgroup common_qmi_msg_ids Constant values for QMI message IDs */
/** @defgroup common_qmi_enums Enumerated types used in QMI messages */
/** @defgroup common_qmi_messages Structures sent as QMI messages */
/** @defgroup common_qmi_aggregates Aggregate types used in QMI messages */
/** @defgroup common_qmi_accessor Accessor for QMI service object */
/** @defgroup common_qmi_version Constant values for versioning information */

#include <stdint.h>
#include "qmi_idl_lib.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * This header file defines the types and structures that were defined in
 * common. It contains the constant values defined, enums, structures,
 * messages, and service message IDs (in that order) Structures that were
 * defined in the IDL as messages contain mandatory elements, optional
 * elements, a combination of mandatory and optional elements (mandatory
 * always come before optionals in the structure), or nothing (null message)
 *
 * An optional element in a message is preceded by a uint8_t value that must be
 * set to true if the element is going to be included. When decoding a received
 * message, the uint8_t values will be set to true or false by the decode
 * routine, and should be checked before accessing the values that they
 * correspond to.
 *
 * Variable sized arrays are defined as static sized arrays with an unsigned
 * integer (32 bit) preceding it that must be set to the number of elements
 * in the array that are valid. For Example:
 *
 * uint32_t test_opaque_len;
 * uint8_t test_opaque[16];
 *
 * If only 4 elements are added to test_opaque[] then test_opaque_len must be
 * set to 4 before sending the message.  When decoding, the _len value is set
 * by the decode routine and should be checked so that the correct number of
 * elements in the array will be accessed.
 */
/** @addtogroup common_qmi_version
    @{
 */
/** Major Version Number of the IDL used to generate this file */
#define COMMON_V01_IDL_MAJOR_VERS 0x01
/** Revision Number of the IDL used to generate this file */
#define COMMON_V01_IDL_MINOR_VERS 0x02
/** Major Version Number of the qmi_idl_compiler used to generate this file */
#define COMMON_V01_IDL_TOOL_VERS 0x02

/**
    @}
  */

/** @addtogroup common_qmi_enums
    @{
  */
typedef enum {
  QMI_RESULT_TYPE_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  QMI_RESULT_SUCCESS_V01 = 0, 
  QMI_RESULT_FAILURE_V01 = 1, 
  QMI_RESULT_TYPE_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}qmi_result_type_v01;
/**
    @}
  */

/** @addtogroup common_qmi_enums
    @{
  */
typedef enum {
  QMI_ERROR_TYPE_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  QMI_ERR_NONE_V01 = 0x0000,
  QMI_ERR_MALFORMED_MSG_V01 = 0x0001,
  QMI_ERR_NO_MEMORY_V01 = 0x0002,
  QMI_ERR_INTERNAL_V01 = 0x0003,
  QMI_ERR_ABORTED_V01 = 0x0004,
  QMI_ERR_CLIENT_IDS_EXHAUSTED_V01 = 0x0005,
  QMI_ERR_UNABORTABLE_TRANSACTION_V01 = 0x0006,
  QMI_ERR_INVALID_CLIENT_ID_V01 = 0x0007,
  QMI_ERR_NO_THRESHOLDS_V01 = 0x0008,
  QMI_ERR_INVALID_HANDLE_V01 = 0x0009,
  QMI_ERR_INVALID_PROFILE_V01 = 0x000A,
  QMI_ERR_INVALID_PINID_V01 = 0x000B,
  QMI_ERR_INCORRECT_PIN_V01 = 0x000C,
  QMI_ERR_NO_NETWORK_FOUND_V01 = 0x000D,
  QMI_ERR_CALL_FAILED_V01 = 0x000E,
  QMI_ERR_OUT_OF_CALL_V01 = 0x000F,
  QMI_ERR_NOT_PROVISIONED_V01 = 0x0010,
  QMI_ERR_MISSING_ARG_V01 = 0x0011,
  QMI_ERR_ARG_TOO_LONG_V01 = 0x0013,
  QMI_ERR_INVALID_TX_ID_V01 = 0x0016,
  QMI_ERR_DEVICE_IN_USE_V01 = 0x0017,
  QMI_ERR_OP_NETWORK_UNSUPPORTED_V01 = 0x0018,
  QMI_ERR_OP_DEVICE_UNSUPPORTED_V01 = 0x0019,
  QMI_ERR_NO_EFFECT_V01 = 0x001A,
  QMI_ERR_NO_FREE_PROFILE_V01 = 0x001B,
  QMI_ERR_INVALID_PDP_TYPE_V01 = 0x001C,
  QMI_ERR_INVALID_TECH_PREF_V01 = 0x001D,
  QMI_ERR_INVALID_PROFILE_TYPE_V01 = 0x001E,
  QMI_ERR_INVALID_SERVICE_TYPE_V01 = 0x001F,
  QMI_ERR_INVALID_REGISTER_ACTION_V01 = 0x0020,
  QMI_ERR_INVALID_PS_ATTACH_ACTION_V01 = 0x0021,
  QMI_ERR_AUTHENTICATION_FAILED_V01 = 0x0022,
  QMI_ERR_PIN_BLOCKED_V01 = 0x0023,
  QMI_ERR_PIN_PERM_BLOCKED_V01 = 0x0024,
  QMI_ERR_SIM_NOT_INITIALIZED_V01 = 0x0025,
  QMI_ERR_MAX_QOS_REQUESTS_IN_USE_V01 = 0x0026,
  QMI_ERR_INCORRECT_FLOW_FILTER_V01 = 0x0027,
  QMI_ERR_NETWORK_QOS_UNAWARE_V01 = 0x0028,
  QMI_ERR_INVALID_ID_V01 = 0x0029,
  QMI_ERR_INVALID_QOS_ID_V01 = 0x0029,
  QMI_ERR_REQUESTED_NUM_UNSUPPORTED_V01 = 0x002A,
  QMI_ERR_INTERFACE_NOT_FOUND_V01 = 0x002B,
  QMI_ERR_FLOW_SUSPENDED_V01 = 0x002C,
  QMI_ERR_INVALID_DATA_FORMAT_V01 = 0x002D,
  QMI_ERR_GENERAL_V01 = 0x002E,
  QMI_ERR_UNKNOWN_V01 = 0x002F,
  QMI_ERR_INVALID_ARG_V01 = 0x0030,
  QMI_ERR_INVALID_INDEX_V01 = 0x0031,
  QMI_ERR_NO_ENTRY_V01 = 0x0032,
  QMI_ERR_DEVICE_STORAGE_FULL_V01 = 0x0033,
  QMI_ERR_DEVICE_NOT_READY_V01 = 0x0034,
  QMI_ERR_NETWORK_NOT_READY_V01 = 0x0035,
  QMI_ERR_CAUSE_CODE_V01 = 0x0036,
  QMI_ERR_MESSAGE_NOT_SENT_V01 = 0x0037,
  QMI_ERR_MESSAGE_DELIVERY_FAILURE_V01 = 0x0038,
  QMI_ERR_INVALID_MESSAGE_ID_V01 = 0x0039,
  QMI_ERR_ENCODING_V01 = 0x003A,
  QMI_ERR_AUTHENTICATION_LOCK_V01 = 0x003B,
  QMI_ERR_INVALID_TRANSITION_V01 = 0x003C,
  QMI_ERR_NOT_A_MCAST_IFACE_V01 = 0x003D,
  QMI_ERR_MAX_MCAST_REQUESTS_IN_USE_V01 = 0x003E,
  QMI_ERR_INVALID_MCAST_HANDLE_V01 = 0x003F,
  QMI_ERR_INVALID_IP_FAMILY_PREF_V01 = 0x0040,
  QMI_ERR_SESSION_INACTIVE_V01 = 0x0041,
  QMI_ERR_SESSION_INVALID_V01 = 0x0042,
  QMI_ERR_SESSION_OWNERSHIP_V01 = 0x0043,
  QMI_ERR_INSUFFICIENT_RESOURCES_V01 = 0x0044,
  QMI_ERR_DISABLED_V01 = 0x0045,
  QMI_ERR_INVALID_OPERATION_V01 = 0x0046,
  QMI_ERR_INVALID_QMI_CMD_V01 = 0x0047,
  QMI_ERR_TPDU_TYPE_V01 = 0x0048,
  QMI_ERR_SMSC_ADDR_V01 = 0x0049,
  QMI_ERR_INFO_UNAVAILABLE_V01 = 0x004A,
  QMI_ERR_SEGMENT_TOO_LONG_V01 = 0x004B,
  QMI_ERR_SEGMENT_ORDER_V01 = 0x004C,
  QMI_ERR_BUNDLING_NOT_SUPPORTED_V01 = 0x004D,
  QMI_ERR_OP_PARTIAL_FAILURE_V01 = 0x004E,
  QMI_ERR_POLICY_MISMATCH_V01 = 0x004F,
  QMI_ERR_SIM_FILE_NOT_FOUND_V01 = 0x0050,
  QMI_ERR_EXTENDED_INTERNAL_V01 = 0x0051,
  QMI_ERR_ACCESS_DENIED_V01 = 0x0052,
  QMI_ERR_HARDWARE_RESTRICTED_V01 = 0x0053,
  QMI_ERR_ACK_NOT_SENT_V01 = 0x0054,
  QMI_ERR_INJECT_TIMEOUT_V01 = 0x0055,
  QMI_ERR_INCOMPATIBLE_STATE_V01 = 0x005A,
  QMI_ERR_FDN_RESTRICT_V01 = 0x005B,
  QMI_ERR_SUPS_FAILURE_CAUSE_V01 = 0x005C,
  QMI_ERR_NO_RADIO_V01 = 0x005D,
  QMI_ERR_NOT_SUPPORTED_V01 = 0x005E,
  QMI_ERR_NO_SUBSCRIPTION_V01 = 0x005F,
  QMI_ERR_CARD_CALL_CONTROL_FAILED_V01 = 0x0060,
  QMI_ERR_NETWORK_ABORTED_V01 = 0x0061,
  QMI_ERR_MSG_BLOCKED_V01 = 0x0062,
  QMI_ERR_INVALID_SESSION_TYPE_V01 = 0x0064,
  QMI_ERR_INVALID_PB_TYPE_V01 = 0x0065,
  QMI_ERR_NO_SIM_V01 = 0x0066,
  QMI_ERR_PB_NOT_READY_V01 = 0x0067,
  QMI_ERR_PIN_RESTRICTION_V01 = 0x0068,
  QMI_ERR_PIN2_RESTRICTION_V01 = 0x0069,
  QMI_ERR_PUK_RESTRICTION_V01 = 0x006A,
  QMI_ERR_PUK2_RESTRICTION_V01 = 0x006B,
  QMI_ERR_PB_ACCESS_RESTRICTED_V01 = 0x006C,
  QMI_ERR_PB_DELETE_IN_PROG_V01 = 0x006D,
  QMI_ERR_MESSAGE_DELIVERY_FAILURE_IMS_V01 = 0x006E,
  QMI_ERR_PB_TEXT_TOO_LONG_V01 = 0x006E,
  QMI_ERR_PB_NUMBER_TOO_LONG_V01 = 0x006F,
  QMI_ERR_PB_HIDDEN_KEY_RESTRICTION_V01 = 0x0070,
  QMI_ERROR_TYPE_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}qmi_error_type_v01;
/**
    @}
  */

/** @addtogroup common_qmi_aggregates
    @{
  */
typedef struct {

  qmi_result_type_v01 result;
  /**<   Result code
 	                           - QMI_RESULT_SUCCESS
 	                           - QMI_RESULT_FAILURE
                           */

  qmi_error_type_v01 error;
  /**<   Error code. Possible error code values are
                               described in the error codes section of each
                               message definition.
                           */
}qmi_response_type_v01;	/* Type */
/**
    @}
 */

/*Extern Definition of Type Table Object*/
/*THIS IS AN INTERNAL OBJECT AND SHOULD ONLY*/
/*BE ACCESSED BY AUTOGENERATED FILES*/
extern const qmi_idl_type_table_object common_qmi_idl_type_table_object_v01;

/* XXX: BW Compatibility Defines DO NOT USE. These will be removed shortly */
#define QMI_RESULT_TYPE_MIN_ENUM_VAL  QMI_RESULT_TYPE_MIN_ENUM_VAL_V01
#define QMI_RESULT_SUCCESS  QMI_RESULT_SUCCESS_V01
#define QMI_RESULT_FAILURE  QMI_RESULT_FAILURE_V01
#define QMI_RESULT_TYPE_MAX_ENUM_VAL  QMI_RESULT_TYPE_MAX_ENUM_VAL_V01
#define QMI_ERR_UNABORTABLE_TRANSACTION QMI_ERR_UNABORTABLE_TRANSACTION_V01
#define QMI_ERR_ABORTED QMI_ERR_ABORTED_V01
#define QMI_ERR_INVALID_CLIENT_ID QMI_ERR_INVALID_CLIENT_ID_V01
#define QMI_ERR_NO_EFFECT QMI_ERR_NO_EFFECT_V01
#define QMI_ERR_INVALID_PDP_TYPE QMI_ERR_INVALID_PDP_TYPE_V01
#define QMI_ERR_OUT_OF_CALL QMI_ERR_OUT_OF_CALL_V01
#define QMI_ERR_NONE QMI_ERR_NONE_V01
#define QMI_ERR_NO_FREE_PROFILE QMI_ERR_NO_FREE_PROFILE_V01
#define QMI_ERR_OP_DEVICE_UNSUPPORTED QMI_ERR_OP_DEVICE_UNSUPPORTED_V01
#define QMI_ERR_INVALID_HANDLE QMI_ERR_INVALID_HANDLE_V01
#define QMI_ERR_NO_NETWORK_FOUND QMI_ERR_NO_NETWORK_FOUND_V01
#define QMI_ERR_ARG_TOO_LONG QMI_ERR_ARG_TOO_LONG_V01
#define QMI_ERR_INTERNAL QMI_ERR_INTERNAL_V01
#define QMI_ERR_CLIENT_IDS_EXHAUSTED QMI_ERR_CLIENT_IDS_EXHAUSTED_V01
#define QMI_ERR_OP_NETWORK_UNSUPPORTED QMI_ERR_OP_NETWORK_UNSUPPORTED_V01
#define QMI_ERR_INVALID_PROFILE QMI_ERR_INVALID_PROFILE_V01
#define QMI_ERR_INVALID_TX_ID QMI_ERR_INVALID_TX_ID_V01
#define QMI_ERR_MISSING_ARG QMI_ERR_MISSING_ARG_V01
#define QMI_ERR_NOT_PROVISIONED QMI_ERR_NOT_PROVISIONED_V01
#define QMI_ERR_DEVICE_IN_USE QMI_ERR_DEVICE_IN_USE_V01
#define QMI_ERR_MALFORMED_MSG QMI_ERR_MALFORMED_MSG_V01
#define QMI_ERR_INVALID_TECH_PREF QMI_ERR_INVALID_TECH_PREF_V01
#define QMI_ERR_NO_MEMORY QMI_ERR_NO_MEMORY_V01
typedef qmi_error_type_v01 qmi_error_type;
typedef qmi_result_type_v01 qmi_result_type;
typedef qmi_response_type_v01 qmi_response_type;


#ifdef __cplusplus
}
#endif
#endif

