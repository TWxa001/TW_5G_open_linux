/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                        C O M M O N _ V 0 1  . C

GENERAL DESCRIPTION
  This is the file which defines the common service Data structures.

 Copyright (c) 2011 QUALCOMM Technologies Incorporated.  All Rights Reserved.
 QUALCOMM Proprietary and Confidential.

 *====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
 *THIS IS AN AUTO GENERATED FILE. DO NOT ALTER IN ANY WAY
 *====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* This file was generated with Tool version 2.5
   It was generated on: Tue Apr 12 2011
   From IDL File: common_v01.idl */

#include <stdint.h>
#include "qmi_idl_lib_internal.h"
#include "common_v01.h"

/*Type Definitions*/
static const uint8_t qmi_response_type_data_v01[] = {
  QMI_IDL_2_BYTE_ENUM,
  QMI_IDL_OFFSET8(qmi_response_type_v01, result),

  QMI_IDL_2_BYTE_ENUM,
  QMI_IDL_OFFSET8(qmi_response_type_v01, error),

  QMI_IDL_FLAG_END_VALUE
};

/*Message Definitions*/
/* Type Table */
static const qmi_idl_type_table_entry  common_type_table_v01[] = {
  {sizeof(qmi_response_type_v01), qmi_response_type_data_v01}
};

/* Message Table */
/* No Messages Defined in IDL */

/* Predefine the Type Table Object */
const qmi_idl_type_table_object common_qmi_idl_type_table_object_v01;

/*Referenced Tables Array*/
static const qmi_idl_type_table_object *common_qmi_idl_type_table_object_referenced_tables_v01[] =
{&common_qmi_idl_type_table_object_v01};

/*Type Table Object*/
const qmi_idl_type_table_object common_qmi_idl_type_table_object_v01 = {
  sizeof(common_type_table_v01)/sizeof(qmi_idl_type_table_entry ),
  0,
  1,
  common_type_table_v01,
  NULL,
  common_qmi_idl_type_table_object_referenced_tables_v01
};

