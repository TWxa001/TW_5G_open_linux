/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                        U S E R _ I D E N T I T Y _ M O D U L E _ V 0 1  . C

GENERAL DESCRIPTION
  This is the file which defines the uim service Data structures.

  Copyright (c) 2012 QUALCOMM Technologies Incorporated.  All Rights Reserved. 
 QUALCOMM Proprietary and Confidential.

  $Header: //source/qcom/qct/interfaces/qmi/uim/main/latest/src/user_identity_module_v01.c#12 $
 *====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====* 
 *THIS IS AN AUTO GENERATED FILE. DO NOT ALTER IN ANY WAY 
 *====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* This file was generated with Tool version 5.3 
   It requires encode/decode library version 4 or later
   It was generated on: Mon May 14 2012
   From IDL File: user_identity_module_v01.idl */

#include "stdint.h"
#include "qmi_idl_lib_internal.h"
#include "user_identity_module_v01.h"
#include "common_v01.h"


/*Type Definitions*/
static const uint8_t uim_session_information_type_data_v01[] = {
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_session_information_type_v01, session_type),

  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN | QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_session_information_type_v01, aid),
  QMI_UIM_AID_MAX_V01,
  QMI_IDL_OFFSET8(uim_session_information_type_v01, aid) - QMI_IDL_OFFSET8(uim_session_information_type_v01, aid_len),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t uim_file_id_type_data_v01[] = {
  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(uim_file_id_type_v01, file_id),

  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN | QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_file_id_type_v01, path),
  QMI_UIM_PATH_MAX_V01,
  QMI_IDL_OFFSET8(uim_file_id_type_v01, path) - QMI_IDL_OFFSET8(uim_file_id_type_v01, path_len),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t uim_read_transparent_type_data_v01[] = {
  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(uim_read_transparent_type_v01, offset),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(uim_read_transparent_type_v01, length),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t uim_card_result_type_data_v01[] = {
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_card_result_type_v01, sw1),

  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_card_result_type_v01, sw2),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t uim_read_transparent_result_type_data_v01[] = {
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN | QMI_IDL_FLAGS_SZ_IS_16 | QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_read_transparent_result_type_v01, content),
  ((QMI_UIM_CONTENT_TRANSPARENT_MAX_V01) & 0xFF), ((QMI_UIM_CONTENT_TRANSPARENT_MAX_V01) >> 8),
  QMI_IDL_OFFSET8(uim_read_transparent_result_type_v01, content) - QMI_IDL_OFFSET8(uim_read_transparent_result_type_v01, content_len),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t uim_read_record_type_data_v01[] = {
  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(uim_read_record_type_v01, record),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(uim_read_record_type_v01, length),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t uim_read_record_result_type_data_v01[] = {
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN | QMI_IDL_FLAGS_SZ_IS_16 | QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_read_record_result_type_v01, content),
  ((QMI_UIM_CONTENT_RECORD_MAX_V01) & 0xFF), ((QMI_UIM_CONTENT_RECORD_MAX_V01) >> 8),
  QMI_IDL_OFFSET8(uim_read_record_result_type_v01, content) - QMI_IDL_OFFSET8(uim_read_record_result_type_v01, content_len),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t uim_read_additional_record_result_type_data_v01[] = {
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN | QMI_IDL_FLAGS_SZ_IS_16 | QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_read_additional_record_result_type_v01, additional_record),
  ((QMI_UIM_CONTENT_ADDITIONAL_RECORD_MAX_V01) & 0xFF), ((QMI_UIM_CONTENT_ADDITIONAL_RECORD_MAX_V01) >> 8),
  QMI_IDL_OFFSET8(uim_read_additional_record_result_type_v01, additional_record) - QMI_IDL_OFFSET8(uim_read_additional_record_result_type_v01, additional_record_len),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t uim_write_transparent_type_data_v01[] = {
  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(uim_write_transparent_type_v01, offset),

  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN | QMI_IDL_FLAGS_SZ_IS_16 | QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_write_transparent_type_v01, data),
  ((QMI_UIM_CONTENT_TRANSPARENT_MAX_V01) & 0xFF), ((QMI_UIM_CONTENT_TRANSPARENT_MAX_V01) >> 8),
  QMI_IDL_OFFSET8(uim_write_transparent_type_v01, data) - QMI_IDL_OFFSET8(uim_write_transparent_type_v01, data_len),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t uim_write_record_type_data_v01[] = {
  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(uim_write_record_type_v01, record),

  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN | QMI_IDL_FLAGS_SZ_IS_16 | QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_write_record_type_v01, data),
  ((QMI_UIM_CONTENT_RECORD_MAX_V01) & 0xFF), ((QMI_UIM_CONTENT_RECORD_MAX_V01) >> 8),
  QMI_IDL_OFFSET8(uim_write_record_type_v01, data) - QMI_IDL_OFFSET8(uim_write_record_type_v01, data_len),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t uim_file_attributes_type_data_v01[] = {
  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(uim_file_attributes_type_v01, file_size),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(uim_file_attributes_type_v01, file_id),

  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_file_attributes_type_v01, file_type),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(uim_file_attributes_type_v01, rec_size),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(uim_file_attributes_type_v01, rec_count),

  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_file_attributes_type_v01, sec_read),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(uim_file_attributes_type_v01, sec_read_mask),

  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_file_attributes_type_v01, sec_write),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(uim_file_attributes_type_v01, sec_write_mask),

  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_file_attributes_type_v01, sec_increase),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(uim_file_attributes_type_v01, sec_increase_mask),

  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_file_attributes_type_v01, sec_deactivate),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(uim_file_attributes_type_v01, sec_deactivate_mask),

  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_file_attributes_type_v01, sec_activate),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(uim_file_attributes_type_v01, sec_activate_mask),

  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN | QMI_IDL_FLAGS_SZ_IS_16 | QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_file_attributes_type_v01, raw_value),
  ((QMI_UIM_FILE_ATTR_RAW_VALUE_MAX_V01) & 0xFF), ((QMI_UIM_FILE_ATTR_RAW_VALUE_MAX_V01) >> 8),
  QMI_IDL_OFFSET8(uim_file_attributes_type_v01, raw_value) - QMI_IDL_OFFSET8(uim_file_attributes_type_v01, raw_value_len),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t uim_register_refresh_file_id_type_data_v01[] = {
  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(uim_register_refresh_file_id_type_v01, file_id),

  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN | QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_register_refresh_file_id_type_v01, path),
  QMI_UIM_PATH_MAX_V01,
  QMI_IDL_OFFSET8(uim_register_refresh_file_id_type_v01, path) - QMI_IDL_OFFSET8(uim_register_refresh_file_id_type_v01, path_len),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t uim_register_refresh_type_data_v01[] = {
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_register_refresh_type_v01, register_flag),

  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_register_refresh_type_v01, vote_for_init),

  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN | QMI_IDL_FLAGS_SZ_IS_16 |  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_register_refresh_type_v01, files),
  ((QMI_UIM_REFRESH_FILES_MAX_V01) & 0xFF), ((QMI_UIM_REFRESH_FILES_MAX_V01) >> 8),
  QMI_IDL_OFFSET8(uim_register_refresh_type_v01, files) - QMI_IDL_OFFSET8(uim_register_refresh_type_v01, files_len),
 11, 0,
  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t uim_refresh_event_type_data_v01[] = {
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_refresh_event_type_v01, stage),

  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_refresh_event_type_v01, mode),

  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_refresh_event_type_v01, session_type),

  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN | QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_refresh_event_type_v01, aid),
  QMI_UIM_AID_MAX_V01,
  QMI_IDL_OFFSET8(uim_refresh_event_type_v01, aid) - QMI_IDL_OFFSET8(uim_refresh_event_type_v01, aid_len),

  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN | QMI_IDL_FLAGS_SZ_IS_16 |  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_refresh_event_type_v01, files),
  ((QMI_UIM_REFRESH_FILES_MAX_V01) & 0xFF), ((QMI_UIM_REFRESH_FILES_MAX_V01) >> 8),
  QMI_IDL_OFFSET8(uim_refresh_event_type_v01, files) - QMI_IDL_OFFSET8(uim_refresh_event_type_v01, files_len),
 11, 0,
  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t uim_set_pin_protection_type_data_v01[] = {
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_set_pin_protection_type_v01, pin_id),

  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_set_pin_protection_type_v01, pin_operation),

  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN | QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_set_pin_protection_type_v01, pin_value),
  QMI_UIM_PIN_MAX_V01,
  QMI_IDL_OFFSET8(uim_set_pin_protection_type_v01, pin_value) - QMI_IDL_OFFSET8(uim_set_pin_protection_type_v01, pin_value_len),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t uim_pin_retries_left_type_data_v01[] = {
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_pin_retries_left_type_v01, verify_left),

  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_pin_retries_left_type_v01, unblock_left),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t uim_encrypted_pin_type_data_v01[] = {
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN | QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_encrypted_pin_type_v01, enc_pin1_value),
  QMI_UIM_ENCRYPTED_PIN_MAX_V01,
  QMI_IDL_OFFSET8(uim_encrypted_pin_type_v01, enc_pin1_value) - QMI_IDL_OFFSET8(uim_encrypted_pin_type_v01, enc_pin1_value_len),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t uim_verify_pin_type_data_v01[] = {
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_verify_pin_type_v01, pin_id),

  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN | QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_verify_pin_type_v01, pin_value),
  QMI_UIM_PIN_MAX_V01,
  QMI_IDL_OFFSET8(uim_verify_pin_type_v01, pin_value) - QMI_IDL_OFFSET8(uim_verify_pin_type_v01, pin_value_len),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t uim_unblock_pin_type_data_v01[] = {
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_unblock_pin_type_v01, pin_id),

  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN | QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_unblock_pin_type_v01, puk_value),
  QMI_UIM_PIN_MAX_V01,
  QMI_IDL_OFFSET8(uim_unblock_pin_type_v01, puk_value) - QMI_IDL_OFFSET8(uim_unblock_pin_type_v01, puk_value_len),

  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN | QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_unblock_pin_type_v01, new_pin_value),
  QMI_UIM_PIN_MAX_V01,
  QMI_IDL_OFFSET8(uim_unblock_pin_type_v01, new_pin_value) - QMI_IDL_OFFSET8(uim_unblock_pin_type_v01, new_pin_value_len),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t uim_change_pin_type_data_v01[] = {
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_change_pin_type_v01, pin_id),

  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN | QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_change_pin_type_v01, old_pin_value),
  QMI_UIM_PIN_MAX_V01,
  QMI_IDL_OFFSET8(uim_change_pin_type_v01, old_pin_value) - QMI_IDL_OFFSET8(uim_change_pin_type_v01, old_pin_value_len),

  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN | QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_change_pin_type_v01, new_pin_value),
  QMI_UIM_PIN_MAX_V01,
  QMI_IDL_OFFSET8(uim_change_pin_type_v01, new_pin_value) - QMI_IDL_OFFSET8(uim_change_pin_type_v01, new_pin_value_len),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t uim_depersonalization_type_data_v01[] = {
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_depersonalization_type_v01, feature),

  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_depersonalization_type_v01, operation),

  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN | QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_depersonalization_type_v01, ck_value),
  QMI_UIM_CK_MAX_V01,
  QMI_IDL_OFFSET8(uim_depersonalization_type_v01, ck_value) - QMI_IDL_OFFSET8(uim_depersonalization_type_v01, ck_value_len),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t uim_perso_retries_left_type_data_v01[] = {
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_perso_retries_left_type_v01, verify_left),

  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_perso_retries_left_type_v01, unblock_left),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t uim_pin_info_type_data_v01[] = {
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_pin_info_type_v01, pin_state),

  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_pin_info_type_v01, pin_retries),

  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_pin_info_type_v01, puk_retries),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t app_info_type_data_v01[] = {
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(app_info_type_v01, app_type),

  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(app_info_type_v01, app_state),

  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(app_info_type_v01, perso_state),

  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(app_info_type_v01, perso_feature),

  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(app_info_type_v01, perso_retries),

  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(app_info_type_v01, perso_unblock_retries),

  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN | QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(app_info_type_v01, aid_value),
  QMI_UIM_AID_MAX_V01,
  QMI_IDL_OFFSET8(app_info_type_v01, aid_value) - QMI_IDL_OFFSET8(app_info_type_v01, aid_value_len),

  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(app_info_type_v01, univ_pin),

   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(app_info_type_v01, pin1),
 22, 0,
   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(app_info_type_v01, pin2),
 22, 0,
  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t card_info_type_data_v01[] = {
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(card_info_type_v01, card_state),

   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(card_info_type_v01, upin),
 22, 0,
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(card_info_type_v01, error_code),

  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(card_info_type_v01, app_info),
  QMI_UIM_APPS_MAX_V01,
  QMI_IDL_OFFSET8(card_info_type_v01, app_info) - QMI_IDL_OFFSET8(card_info_type_v01, app_info_len),
 23, 0,
  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t uim_card_status_type_data_v01[] = {
  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(uim_card_status_type_v01, index_gw_pri),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(uim_card_status_type_v01, index_1x_pri),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(uim_card_status_type_v01, index_gw_sec),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(uim_card_status_type_v01, index_1x_sec),

  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_card_status_type_v01, card_info),
  QMI_UIM_CARDS_MAX_V01,
  QMI_IDL_OFFSET8(uim_card_status_type_v01, card_info) - QMI_IDL_OFFSET8(uim_card_status_type_v01, card_info_len),
 24, 0,
  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t uim_authentication_data_type_data_v01[] = {
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_authentication_data_type_v01, context),

  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN | QMI_IDL_FLAGS_SZ_IS_16 | QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_authentication_data_type_v01, data),
  ((QMI_UIM_AUTHENTICATE_DATA_MAX_V01) & 0xFF), ((QMI_UIM_AUTHENTICATE_DATA_MAX_V01) >> 8),
  QMI_IDL_OFFSET8(uim_authentication_data_type_v01, data) - QMI_IDL_OFFSET8(uim_authentication_data_type_v01, data_len),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t uim_session_change_type_data_v01[] = {
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_session_change_type_v01, session_type),

  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_session_change_type_v01, activate),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t uim_application_information_type_data_v01[] = {
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_application_information_type_v01, slot),

  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN | QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_application_information_type_v01, aid),
  QMI_UIM_AID_MAX_V01,
  QMI_IDL_OFFSET8(uim_application_information_type_v01, aid) - QMI_IDL_OFFSET8(uim_application_information_type_v01, aid_len),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t uim_personalization_status_type_data_v01[] = {
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_personalization_status_type_v01, feature),

  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_personalization_status_type_v01, verify_left),

  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_personalization_status_type_v01, unblock_left),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t uim_send_apdu_long_response_type_data_v01[] = {
  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(uim_send_apdu_long_response_type_v01, total_length),

  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(uim_send_apdu_long_response_type_v01, token),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t uim_send_apdu_long_response_chunk_type_data_v01[] = {
  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(uim_send_apdu_long_response_chunk_type_v01, token),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(uim_send_apdu_long_response_chunk_type_v01, total_length),

  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(uim_send_apdu_long_response_chunk_type_v01, offset),

  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN | QMI_IDL_FLAGS_SZ_IS_16 | QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_send_apdu_long_response_chunk_type_v01, apdu),
  ((QMI_UIM_APDU_DATA_MAX_V01) & 0xFF), ((QMI_UIM_APDU_DATA_MAX_V01) >> 8),
  QMI_IDL_OFFSET8(uim_send_apdu_long_response_chunk_type_v01, apdu) - QMI_IDL_OFFSET8(uim_send_apdu_long_response_chunk_type_v01, apdu_len),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t uim_sap_connect_type_data_v01[] = {
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_sap_connect_type_v01, connect),

  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_sap_connect_type_v01, slot),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t uim_sap_request_type_data_v01[] = {
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_sap_request_type_v01, sap_request),

  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_sap_request_type_v01, slot),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t uim_sap_indication_type_data_v01[] = {
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_sap_indication_type_v01, sap_state),

  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_sap_indication_type_v01, slot),

  QMI_IDL_FLAG_END_VALUE
};

/*Message Definitions*/
/* 
 * uim_reset_req_msg is empty
 * static const uint8_t uim_reset_req_msg_data_v01[] = {
 * };
 */
  
static const uint8_t uim_reset_resp_msg_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_reset_resp_msg_v01, resp),
  0, 1
};

static const uint8_t uim_read_transparent_req_msg_data_v01[] = {
  0x01,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_read_transparent_req_msg_v01, session_information),
  0, 0,

  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_read_transparent_req_msg_v01, file_id),
  1, 0,

  0x03,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_read_transparent_req_msg_v01, read_transparent),
  2, 0,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_read_transparent_req_msg_v01, indication_token) - QMI_IDL_OFFSET8(uim_read_transparent_req_msg_v01, indication_token_valid)),
  0x10,
  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(uim_read_transparent_req_msg_v01, indication_token),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_read_transparent_req_msg_v01, encryption) - QMI_IDL_OFFSET8(uim_read_transparent_req_msg_v01, encryption_valid)),
  0x11,
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_read_transparent_req_msg_v01, encryption)
};

static const uint8_t uim_read_transparent_resp_msg_data_v01[] = {
  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_read_transparent_resp_msg_v01, resp),
  0, 1,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_read_transparent_resp_msg_v01, card_result) - QMI_IDL_OFFSET8(uim_read_transparent_resp_msg_v01, card_result_valid)),
  0x10,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_read_transparent_resp_msg_v01, card_result),
  3, 0,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_read_transparent_resp_msg_v01, read_result) - QMI_IDL_OFFSET8(uim_read_transparent_resp_msg_v01, read_result_valid)),
  0x11,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_read_transparent_resp_msg_v01, read_result),
  4, 0,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET16RELATIVE(uim_read_transparent_resp_msg_v01, indication_token) - QMI_IDL_OFFSET16RELATIVE(uim_read_transparent_resp_msg_v01, indication_token_valid)),
  0x12,
  QMI_IDL_FLAGS_OFFSET_IS_16 | QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET16ARRAY(uim_read_transparent_resp_msg_v01, indication_token),

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET16RELATIVE(uim_read_transparent_resp_msg_v01, encryption) - QMI_IDL_OFFSET16RELATIVE(uim_read_transparent_resp_msg_v01, encryption_valid)),
  0x13,
  QMI_IDL_FLAGS_OFFSET_IS_16 | QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET16ARRAY(uim_read_transparent_resp_msg_v01, encryption),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET16RELATIVE(uim_read_transparent_resp_msg_v01, file_length) - QMI_IDL_OFFSET16RELATIVE(uim_read_transparent_resp_msg_v01, file_length_valid)),
  0x14,
  QMI_IDL_FLAGS_OFFSET_IS_16 | QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET16ARRAY(uim_read_transparent_resp_msg_v01, file_length)
};

static const uint8_t uim_read_transparent_ind_msg_data_v01[] = {
  0x01,
  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(uim_read_transparent_ind_msg_v01, indication_token),

  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_read_transparent_ind_msg_v01, resp),
  0, 1,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_read_transparent_ind_msg_v01, card_result) - QMI_IDL_OFFSET8(uim_read_transparent_ind_msg_v01, card_result_valid)),
  0x10,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_read_transparent_ind_msg_v01, card_result),
  3, 0,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_read_transparent_ind_msg_v01, read_result) - QMI_IDL_OFFSET8(uim_read_transparent_ind_msg_v01, read_result_valid)),
  0x11,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_read_transparent_ind_msg_v01, read_result),
  4, 0,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET16RELATIVE(uim_read_transparent_ind_msg_v01, encryption) - QMI_IDL_OFFSET16RELATIVE(uim_read_transparent_ind_msg_v01, encryption_valid)),
  0x12,
  QMI_IDL_FLAGS_OFFSET_IS_16 | QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET16ARRAY(uim_read_transparent_ind_msg_v01, encryption),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET16RELATIVE(uim_read_transparent_ind_msg_v01, file_length) - QMI_IDL_OFFSET16RELATIVE(uim_read_transparent_ind_msg_v01, file_length_valid)),
  0x13,
  QMI_IDL_FLAGS_OFFSET_IS_16 | QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET16ARRAY(uim_read_transparent_ind_msg_v01, file_length)
};

static const uint8_t uim_read_record_req_msg_data_v01[] = {
  0x01,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_read_record_req_msg_v01, session_information),
  0, 0,

  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_read_record_req_msg_v01, file_id),
  1, 0,

  0x03,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_read_record_req_msg_v01, read_record),
  5, 0,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_read_record_req_msg_v01, last_record) - QMI_IDL_OFFSET8(uim_read_record_req_msg_v01, last_record_valid)),
  0x10,
  QMI_IDL_GENERIC_2_BYTE,
  QMI_IDL_OFFSET8(uim_read_record_req_msg_v01, last_record),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_read_record_req_msg_v01, indication_token) - QMI_IDL_OFFSET8(uim_read_record_req_msg_v01, indication_token_valid)),
  0x11,
  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(uim_read_record_req_msg_v01, indication_token)
};

static const uint8_t uim_read_record_resp_msg_data_v01[] = {
  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_read_record_resp_msg_v01, resp),
  0, 1,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_read_record_resp_msg_v01, card_result) - QMI_IDL_OFFSET8(uim_read_record_resp_msg_v01, card_result_valid)),
  0x10,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_read_record_resp_msg_v01, card_result),
  3, 0,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_read_record_resp_msg_v01, read_result) - QMI_IDL_OFFSET8(uim_read_record_resp_msg_v01, read_result_valid)),
  0x11,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_read_record_resp_msg_v01, read_result),
  6, 0,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET16RELATIVE(uim_read_record_resp_msg_v01, additional_read_result) - QMI_IDL_OFFSET16RELATIVE(uim_read_record_resp_msg_v01, additional_read_result_valid)),
  0x12,
  QMI_IDL_FLAGS_OFFSET_IS_16 | QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET16ARRAY(uim_read_record_resp_msg_v01, additional_read_result),
  7, 0,

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET16RELATIVE(uim_read_record_resp_msg_v01, indication_token) - QMI_IDL_OFFSET16RELATIVE(uim_read_record_resp_msg_v01, indication_token_valid)),
  0x13,
  QMI_IDL_FLAGS_OFFSET_IS_16 | QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET16ARRAY(uim_read_record_resp_msg_v01, indication_token)
};

static const uint8_t uim_read_record_ind_msg_data_v01[] = {
  0x01,
  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(uim_read_record_ind_msg_v01, indication_token),

  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_read_record_ind_msg_v01, resp),
  0, 1,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_read_record_ind_msg_v01, card_result) - QMI_IDL_OFFSET8(uim_read_record_ind_msg_v01, card_result_valid)),
  0x10,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_read_record_ind_msg_v01, card_result),
  3, 0,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_read_record_ind_msg_v01, read_result) - QMI_IDL_OFFSET8(uim_read_record_ind_msg_v01, read_result_valid)),
  0x11,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_read_record_ind_msg_v01, read_result),
  6, 0,

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET16RELATIVE(uim_read_record_ind_msg_v01, additional_read_result) - QMI_IDL_OFFSET16RELATIVE(uim_read_record_ind_msg_v01, additional_read_result_valid)),
  0x12,
  QMI_IDL_FLAGS_OFFSET_IS_16 | QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET16ARRAY(uim_read_record_ind_msg_v01, additional_read_result),
  7, 0
};

static const uint8_t uim_write_transparent_req_msg_data_v01[] = {
  0x01,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_write_transparent_req_msg_v01, session_information),
  0, 0,

  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_write_transparent_req_msg_v01, file_id),
  1, 0,

  0x03,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_write_transparent_req_msg_v01, write_transparent),
  8, 0,

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET16RELATIVE(uim_write_transparent_req_msg_v01, indication_token) - QMI_IDL_OFFSET16RELATIVE(uim_write_transparent_req_msg_v01, indication_token_valid)),
  0x10,
  QMI_IDL_FLAGS_OFFSET_IS_16 | QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET16ARRAY(uim_write_transparent_req_msg_v01, indication_token)
};

static const uint8_t uim_write_transparent_resp_msg_data_v01[] = {
  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_write_transparent_resp_msg_v01, resp),
  0, 1,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_write_transparent_resp_msg_v01, card_result) - QMI_IDL_OFFSET8(uim_write_transparent_resp_msg_v01, card_result_valid)),
  0x10,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_write_transparent_resp_msg_v01, card_result),
  3, 0,

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_write_transparent_resp_msg_v01, indication_token) - QMI_IDL_OFFSET8(uim_write_transparent_resp_msg_v01, indication_token_valid)),
  0x11,
  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(uim_write_transparent_resp_msg_v01, indication_token)
};

static const uint8_t uim_write_transparent_ind_msg_data_v01[] = {
  0x01,
  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(uim_write_transparent_ind_msg_v01, indication_token),

  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_write_transparent_ind_msg_v01, resp),
  0, 1,

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_write_transparent_ind_msg_v01, card_result) - QMI_IDL_OFFSET8(uim_write_transparent_ind_msg_v01, card_result_valid)),
  0x10,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_write_transparent_ind_msg_v01, card_result),
  3, 0
};

static const uint8_t uim_write_record_req_msg_data_v01[] = {
  0x01,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_write_record_req_msg_v01, session_information),
  0, 0,

  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_write_record_req_msg_v01, file_id),
  1, 0,

  0x03,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_write_record_req_msg_v01, write_record),
  9, 0,

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET16RELATIVE(uim_write_record_req_msg_v01, indication_token) - QMI_IDL_OFFSET16RELATIVE(uim_write_record_req_msg_v01, indication_token_valid)),
  0x10,
  QMI_IDL_FLAGS_OFFSET_IS_16 | QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET16ARRAY(uim_write_record_req_msg_v01, indication_token)
};

static const uint8_t uim_write_record_resp_msg_data_v01[] = {
  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_write_record_resp_msg_v01, resp),
  0, 1,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_write_record_resp_msg_v01, card_result) - QMI_IDL_OFFSET8(uim_write_record_resp_msg_v01, card_result_valid)),
  0x10,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_write_record_resp_msg_v01, card_result),
  3, 0,

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_write_record_resp_msg_v01, indication_token) - QMI_IDL_OFFSET8(uim_write_record_resp_msg_v01, indication_token_valid)),
  0x11,
  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(uim_write_record_resp_msg_v01, indication_token)
};

static const uint8_t uim_write_record_ind_msg_data_v01[] = {
  0x01,
  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(uim_write_record_ind_msg_v01, indication_token),

  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_write_record_ind_msg_v01, resp),
  0, 1,

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_write_record_ind_msg_v01, card_result) - QMI_IDL_OFFSET8(uim_write_record_ind_msg_v01, card_result_valid)),
  0x10,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_write_record_ind_msg_v01, card_result),
  3, 0
};

static const uint8_t uim_get_file_attributes_req_msg_data_v01[] = {
  0x01,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_get_file_attributes_req_msg_v01, session_information),
  0, 0,

  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_get_file_attributes_req_msg_v01, file_id),
  1, 0,

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_get_file_attributes_req_msg_v01, indication_token) - QMI_IDL_OFFSET8(uim_get_file_attributes_req_msg_v01, indication_token_valid)),
  0x10,
  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(uim_get_file_attributes_req_msg_v01, indication_token)
};

static const uint8_t uim_get_file_attributes_resp_msg_data_v01[] = {
  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_get_file_attributes_resp_msg_v01, resp),
  0, 1,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_get_file_attributes_resp_msg_v01, card_result) - QMI_IDL_OFFSET8(uim_get_file_attributes_resp_msg_v01, card_result_valid)),
  0x10,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_get_file_attributes_resp_msg_v01, card_result),
  3, 0,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_get_file_attributes_resp_msg_v01, file_attributes) - QMI_IDL_OFFSET8(uim_get_file_attributes_resp_msg_v01, file_attributes_valid)),
  0x11,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_get_file_attributes_resp_msg_v01, file_attributes),
  10, 0,

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET16RELATIVE(uim_get_file_attributes_resp_msg_v01, indication_token) - QMI_IDL_OFFSET16RELATIVE(uim_get_file_attributes_resp_msg_v01, indication_token_valid)),
  0x12,
  QMI_IDL_FLAGS_OFFSET_IS_16 | QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET16ARRAY(uim_get_file_attributes_resp_msg_v01, indication_token)
};

static const uint8_t uim_get_file_attributes_ind_msg_data_v01[] = {
  0x01,
  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(uim_get_file_attributes_ind_msg_v01, indication_token),

  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_get_file_attributes_ind_msg_v01, resp),
  0, 1,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_get_file_attributes_ind_msg_v01, card_result) - QMI_IDL_OFFSET8(uim_get_file_attributes_ind_msg_v01, card_result_valid)),
  0x10,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_get_file_attributes_ind_msg_v01, card_result),
  3, 0,

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_get_file_attributes_ind_msg_v01, file_attributes) - QMI_IDL_OFFSET8(uim_get_file_attributes_ind_msg_v01, file_attributes_valid)),
  0x11,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_get_file_attributes_ind_msg_v01, file_attributes),
  10, 0
};

static const uint8_t uim_refresh_register_req_msg_data_v01[] = {
  0x01,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_refresh_register_req_msg_v01, session_information),
  0, 0,

  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_refresh_register_req_msg_v01, register_refresh),
  12, 0
};

static const uint8_t uim_refresh_register_resp_msg_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_refresh_register_resp_msg_v01, resp),
  0, 1
};

static const uint8_t uim_refresh_ok_req_msg_data_v01[] = {
  0x01,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_refresh_ok_req_msg_v01, session_information),
  0, 0,

  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x02,
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_refresh_ok_req_msg_v01, ok_to_refresh)
};

static const uint8_t uim_refresh_ok_resp_msg_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_refresh_ok_resp_msg_v01, resp),
  0, 1
};

static const uint8_t uim_refresh_complete_req_msg_data_v01[] = {
  0x01,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_refresh_complete_req_msg_v01, session_information),
  0, 0,

  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x02,
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_refresh_complete_req_msg_v01, refresh_success)
};

static const uint8_t uim_refresh_complete_resp_msg_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_refresh_complete_resp_msg_v01, resp),
  0, 1
};

static const uint8_t uim_refresh_get_last_event_req_msg_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x01,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_refresh_get_last_event_req_msg_v01, session_information),
  0, 0
};

static const uint8_t uim_refresh_get_last_event_resp_msg_data_v01[] = {
  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_refresh_get_last_event_resp_msg_v01, resp),
  0, 1,

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_refresh_get_last_event_resp_msg_v01, refresh_event) - QMI_IDL_OFFSET8(uim_refresh_get_last_event_resp_msg_v01, refresh_event_valid)),
  0x10,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_refresh_get_last_event_resp_msg_v01, refresh_event),
  13, 0
};

static const uint8_t uim_set_pin_protection_req_msg_data_v01[] = {
  0x01,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_set_pin_protection_req_msg_v01, session_information),
  0, 0,

  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_set_pin_protection_req_msg_v01, set_pin_protection),
  14, 0,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_set_pin_protection_req_msg_v01, pin_key_ref_id) - QMI_IDL_OFFSET8(uim_set_pin_protection_req_msg_v01, pin_key_ref_id_valid)),
  0x10,
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_set_pin_protection_req_msg_v01, pin_key_ref_id),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_set_pin_protection_req_msg_v01, indication_token) - QMI_IDL_OFFSET8(uim_set_pin_protection_req_msg_v01, indication_token_valid)),
  0x11,
  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(uim_set_pin_protection_req_msg_v01, indication_token)
};

static const uint8_t uim_set_pin_protection_resp_msg_data_v01[] = {
  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_set_pin_protection_resp_msg_v01, resp),
  0, 1,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_set_pin_protection_resp_msg_v01, retries_left) - QMI_IDL_OFFSET8(uim_set_pin_protection_resp_msg_v01, retries_left_valid)),
  0x10,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_set_pin_protection_resp_msg_v01, retries_left),
  15, 0,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_set_pin_protection_resp_msg_v01, encrypted_pin1) - QMI_IDL_OFFSET8(uim_set_pin_protection_resp_msg_v01, encrypted_pin1_valid)),
  0x11,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_set_pin_protection_resp_msg_v01, encrypted_pin1),
  16, 0,

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET16RELATIVE(uim_set_pin_protection_resp_msg_v01, indication_token) - QMI_IDL_OFFSET16RELATIVE(uim_set_pin_protection_resp_msg_v01, indication_token_valid)),
  0x12,
  QMI_IDL_FLAGS_OFFSET_IS_16 | QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET16ARRAY(uim_set_pin_protection_resp_msg_v01, indication_token)
};

static const uint8_t uim_set_pin_protection_ind_msg_data_v01[] = {
  0x01,
  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(uim_set_pin_protection_ind_msg_v01, indication_token),

  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_set_pin_protection_ind_msg_v01, resp),
  0, 1,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_set_pin_protection_ind_msg_v01, retries_left) - QMI_IDL_OFFSET8(uim_set_pin_protection_ind_msg_v01, retries_left_valid)),
  0x10,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_set_pin_protection_ind_msg_v01, retries_left),
  15, 0,

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_set_pin_protection_ind_msg_v01, encrypted_pin1) - QMI_IDL_OFFSET8(uim_set_pin_protection_ind_msg_v01, encrypted_pin1_valid)),
  0x11,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_set_pin_protection_ind_msg_v01, encrypted_pin1),
  16, 0
};

static const uint8_t uim_verify_pin_req_msg_data_v01[] = {
  0x01,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_verify_pin_req_msg_v01, session_information),
  0, 0,

  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_verify_pin_req_msg_v01, verify_pin),
  17, 0,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_verify_pin_req_msg_v01, encrypted_pin1) - QMI_IDL_OFFSET8(uim_verify_pin_req_msg_v01, encrypted_pin1_valid)),
  0x10,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_verify_pin_req_msg_v01, encrypted_pin1),
  16, 0,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET16RELATIVE(uim_verify_pin_req_msg_v01, pin_key_ref_id) - QMI_IDL_OFFSET16RELATIVE(uim_verify_pin_req_msg_v01, pin_key_ref_id_valid)),
  0x11,
  QMI_IDL_FLAGS_OFFSET_IS_16 | QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET16ARRAY(uim_verify_pin_req_msg_v01, pin_key_ref_id),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET16RELATIVE(uim_verify_pin_req_msg_v01, indication_token) - QMI_IDL_OFFSET16RELATIVE(uim_verify_pin_req_msg_v01, indication_token_valid)),
  0x12,
  QMI_IDL_FLAGS_OFFSET_IS_16 | QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET16ARRAY(uim_verify_pin_req_msg_v01, indication_token)
};

static const uint8_t uim_verify_pin_resp_msg_data_v01[] = {
  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_verify_pin_resp_msg_v01, resp),
  0, 1,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_verify_pin_resp_msg_v01, retries_left) - QMI_IDL_OFFSET8(uim_verify_pin_resp_msg_v01, retries_left_valid)),
  0x10,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_verify_pin_resp_msg_v01, retries_left),
  15, 0,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_verify_pin_resp_msg_v01, encrypted_pin1) - QMI_IDL_OFFSET8(uim_verify_pin_resp_msg_v01, encrypted_pin1_valid)),
  0x11,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_verify_pin_resp_msg_v01, encrypted_pin1),
  16, 0,

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET16RELATIVE(uim_verify_pin_resp_msg_v01, indication_token) - QMI_IDL_OFFSET16RELATIVE(uim_verify_pin_resp_msg_v01, indication_token_valid)),
  0x12,
  QMI_IDL_FLAGS_OFFSET_IS_16 | QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET16ARRAY(uim_verify_pin_resp_msg_v01, indication_token)
};

static const uint8_t uim_verify_pin_ind_msg_data_v01[] = {
  0x01,
  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(uim_verify_pin_ind_msg_v01, indication_token),

  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_verify_pin_ind_msg_v01, resp),
  0, 1,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_verify_pin_ind_msg_v01, retries_left) - QMI_IDL_OFFSET8(uim_verify_pin_ind_msg_v01, retries_left_valid)),
  0x10,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_verify_pin_ind_msg_v01, retries_left),
  15, 0,

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_verify_pin_ind_msg_v01, encrypted_pin1) - QMI_IDL_OFFSET8(uim_verify_pin_ind_msg_v01, encrypted_pin1_valid)),
  0x11,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_verify_pin_ind_msg_v01, encrypted_pin1),
  16, 0
};

static const uint8_t uim_unblock_pin_req_msg_data_v01[] = {
  0x01,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_unblock_pin_req_msg_v01, session_information),
  0, 0,

  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_unblock_pin_req_msg_v01, unblock_pin),
  18, 0,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_unblock_pin_req_msg_v01, pin_key_ref_id) - QMI_IDL_OFFSET8(uim_unblock_pin_req_msg_v01, pin_key_ref_id_valid)),
  0x10,
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_unblock_pin_req_msg_v01, pin_key_ref_id),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_unblock_pin_req_msg_v01, indication_token) - QMI_IDL_OFFSET8(uim_unblock_pin_req_msg_v01, indication_token_valid)),
  0x11,
  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(uim_unblock_pin_req_msg_v01, indication_token)
};

static const uint8_t uim_unblock_pin_resp_msg_data_v01[] = {
  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_unblock_pin_resp_msg_v01, resp),
  0, 1,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_unblock_pin_resp_msg_v01, retries_left) - QMI_IDL_OFFSET8(uim_unblock_pin_resp_msg_v01, retries_left_valid)),
  0x10,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_unblock_pin_resp_msg_v01, retries_left),
  15, 0,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_unblock_pin_resp_msg_v01, encrypted_pin1) - QMI_IDL_OFFSET8(uim_unblock_pin_resp_msg_v01, encrypted_pin1_valid)),
  0x11,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_unblock_pin_resp_msg_v01, encrypted_pin1),
  16, 0,

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET16RELATIVE(uim_unblock_pin_resp_msg_v01, indication_token) - QMI_IDL_OFFSET16RELATIVE(uim_unblock_pin_resp_msg_v01, indication_token_valid)),
  0x12,
  QMI_IDL_FLAGS_OFFSET_IS_16 | QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET16ARRAY(uim_unblock_pin_resp_msg_v01, indication_token)
};

static const uint8_t uim_unblock_pin_ind_msg_data_v01[] = {
  0x01,
  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(uim_unblock_pin_ind_msg_v01, indication_token),

  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_unblock_pin_ind_msg_v01, resp),
  0, 1,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_unblock_pin_ind_msg_v01, retries_left) - QMI_IDL_OFFSET8(uim_unblock_pin_ind_msg_v01, retries_left_valid)),
  0x10,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_unblock_pin_ind_msg_v01, retries_left),
  15, 0,

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_unblock_pin_ind_msg_v01, encrypted_pin1) - QMI_IDL_OFFSET8(uim_unblock_pin_ind_msg_v01, encrypted_pin1_valid)),
  0x11,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_unblock_pin_ind_msg_v01, encrypted_pin1),
  16, 0
};

static const uint8_t uim_change_pin_req_msg_data_v01[] = {
  0x01,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_change_pin_req_msg_v01, session_information),
  0, 0,

  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_change_pin_req_msg_v01, change_pin),
  19, 0,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_change_pin_req_msg_v01, pin_key_ref_id) - QMI_IDL_OFFSET8(uim_change_pin_req_msg_v01, pin_key_ref_id_valid)),
  0x10,
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_change_pin_req_msg_v01, pin_key_ref_id),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_change_pin_req_msg_v01, indication_token) - QMI_IDL_OFFSET8(uim_change_pin_req_msg_v01, indication_token_valid)),
  0x11,
  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(uim_change_pin_req_msg_v01, indication_token)
};

static const uint8_t uim_change_pin_resp_msg_data_v01[] = {
  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_change_pin_resp_msg_v01, resp),
  0, 1,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_change_pin_resp_msg_v01, retries_left) - QMI_IDL_OFFSET8(uim_change_pin_resp_msg_v01, retries_left_valid)),
  0x10,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_change_pin_resp_msg_v01, retries_left),
  15, 0,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_change_pin_resp_msg_v01, encrypted_pin1) - QMI_IDL_OFFSET8(uim_change_pin_resp_msg_v01, encrypted_pin1_valid)),
  0x11,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_change_pin_resp_msg_v01, encrypted_pin1),
  16, 0,

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET16RELATIVE(uim_change_pin_resp_msg_v01, indication_token) - QMI_IDL_OFFSET16RELATIVE(uim_change_pin_resp_msg_v01, indication_token_valid)),
  0x12,
  QMI_IDL_FLAGS_OFFSET_IS_16 | QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET16ARRAY(uim_change_pin_resp_msg_v01, indication_token)
};

static const uint8_t uim_change_pin_ind_msg_data_v01[] = {
  0x01,
  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(uim_change_pin_ind_msg_v01, indication_token),

  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_change_pin_ind_msg_v01, resp),
  0, 1,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_change_pin_ind_msg_v01, retries_left) - QMI_IDL_OFFSET8(uim_change_pin_ind_msg_v01, retries_left_valid)),
  0x10,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_change_pin_ind_msg_v01, retries_left),
  15, 0,

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_change_pin_ind_msg_v01, encrypted_pin1) - QMI_IDL_OFFSET8(uim_change_pin_ind_msg_v01, encrypted_pin1_valid)),
  0x11,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_change_pin_ind_msg_v01, encrypted_pin1),
  16, 0
};

static const uint8_t uim_depersonalization_req_msg_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x01,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_depersonalization_req_msg_v01, depersonalization),
  20, 0
};

static const uint8_t uim_depersonalization_resp_msg_data_v01[] = {
  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_depersonalization_resp_msg_v01, resp),
  0, 1,

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_depersonalization_resp_msg_v01, retries_left) - QMI_IDL_OFFSET8(uim_depersonalization_resp_msg_v01, retries_left_valid)),
  0x10,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_depersonalization_resp_msg_v01, retries_left),
  21, 0
};

static const uint8_t uim_power_down_req_msg_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x01,
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_power_down_req_msg_v01, slot)
};

static const uint8_t uim_power_down_resp_msg_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_power_down_resp_msg_v01, resp),
  0, 1
};

static const uint8_t uim_power_up_req_msg_data_v01[] = {
  0x01,
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_power_up_req_msg_v01, slot),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_power_up_req_msg_v01, ignore_hotswap_switch) - QMI_IDL_OFFSET8(uim_power_up_req_msg_v01, ignore_hotswap_switch_valid)),
  0x10,
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_power_up_req_msg_v01, ignore_hotswap_switch)
};

static const uint8_t uim_power_up_resp_msg_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_power_up_resp_msg_v01, resp),
  0, 1
};

/* 
 * uim_get_card_status_req_msg is empty
 * static const uint8_t uim_get_card_status_req_msg_data_v01[] = {
 * };
 */
  
static const uint8_t uim_get_card_status_resp_msg_data_v01[] = {
  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_get_card_status_resp_msg_v01, resp),
  0, 1,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_get_card_status_resp_msg_v01, card_status) - QMI_IDL_OFFSET8(uim_get_card_status_resp_msg_v01, card_status_valid)),
  0x10,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_get_card_status_resp_msg_v01, card_status),
  25, 0,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET16RELATIVE(uim_get_card_status_resp_msg_v01, hot_swap) - QMI_IDL_OFFSET16RELATIVE(uim_get_card_status_resp_msg_v01, hot_swap_valid)),
  0x11,
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |   QMI_IDL_FLAGS_OFFSET_IS_16 | QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET16ARRAY(uim_get_card_status_resp_msg_v01, hot_swap),
  QMI_UIM_CARDS_MAX_V01,
  QMI_IDL_OFFSET16RELATIVE(uim_get_card_status_resp_msg_v01, hot_swap) - QMI_IDL_OFFSET16RELATIVE(uim_get_card_status_resp_msg_v01, hot_swap_len),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET16RELATIVE(uim_get_card_status_resp_msg_v01, card_status_validity) - QMI_IDL_OFFSET16RELATIVE(uim_get_card_status_resp_msg_v01, card_status_validity_valid)),
  0x12,
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |   QMI_IDL_FLAGS_OFFSET_IS_16 | QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET16ARRAY(uim_get_card_status_resp_msg_v01, card_status_validity),
  QMI_UIM_CARDS_MAX_V01,
  QMI_IDL_OFFSET16RELATIVE(uim_get_card_status_resp_msg_v01, card_status_validity) - QMI_IDL_OFFSET16RELATIVE(uim_get_card_status_resp_msg_v01, card_status_validity_len)
};

static const uint8_t uim_event_reg_req_msg_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x01,
  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(uim_event_reg_req_msg_v01, event_mask)
};

static const uint8_t uim_event_reg_resp_msg_data_v01[] = {
  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_event_reg_resp_msg_v01, resp),
  0, 1,

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_event_reg_resp_msg_v01, event_mask) - QMI_IDL_OFFSET8(uim_event_reg_resp_msg_v01, event_mask_valid)),
  0x10,
  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(uim_event_reg_resp_msg_v01, event_mask)
};

static const uint8_t uim_status_change_ind_msg_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_status_change_ind_msg_v01, card_status) - QMI_IDL_OFFSET8(uim_status_change_ind_msg_v01, card_status_valid)),
  0x10,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_status_change_ind_msg_v01, card_status),
  25, 0
};

static const uint8_t uim_refresh_ind_msg_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_refresh_ind_msg_v01, refresh_event) - QMI_IDL_OFFSET8(uim_refresh_ind_msg_v01, refresh_event_valid)),
  0x10,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_refresh_ind_msg_v01, refresh_event),
  13, 0
};

static const uint8_t uim_authenticate_req_msg_data_v01[] = {
  0x01,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_authenticate_req_msg_v01, session_information),
  0, 0,

  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_authenticate_req_msg_v01, authentication_data),
  26, 0,

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET16RELATIVE(uim_authenticate_req_msg_v01, indication_token) - QMI_IDL_OFFSET16RELATIVE(uim_authenticate_req_msg_v01, indication_token_valid)),
  0x10,
  QMI_IDL_FLAGS_OFFSET_IS_16 | QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET16ARRAY(uim_authenticate_req_msg_v01, indication_token)
};

static const uint8_t uim_authenticate_resp_msg_data_v01[] = {
  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_authenticate_resp_msg_v01, resp),
  0, 1,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_authenticate_resp_msg_v01, card_result) - QMI_IDL_OFFSET8(uim_authenticate_resp_msg_v01, card_result_valid)),
  0x10,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_authenticate_resp_msg_v01, card_result),
  3, 0,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_authenticate_resp_msg_v01, content) - QMI_IDL_OFFSET8(uim_authenticate_resp_msg_v01, content_valid)),
  0x11,
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN | QMI_IDL_FLAGS_SZ_IS_16 |   QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_authenticate_resp_msg_v01, content),
  ((QMI_UIM_AUTHENTICATE_RESULT_MAX_V01) & 0xFF), ((QMI_UIM_AUTHENTICATE_RESULT_MAX_V01) >> 8),
  QMI_IDL_OFFSET8(uim_authenticate_resp_msg_v01, content) - QMI_IDL_OFFSET8(uim_authenticate_resp_msg_v01, content_len),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET16RELATIVE(uim_authenticate_resp_msg_v01, indication_token) - QMI_IDL_OFFSET16RELATIVE(uim_authenticate_resp_msg_v01, indication_token_valid)),
  0x12,
  QMI_IDL_FLAGS_OFFSET_IS_16 | QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET16ARRAY(uim_authenticate_resp_msg_v01, indication_token)
};

static const uint8_t uim_authenticate_ind_msg_data_v01[] = {
  0x01,
  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(uim_authenticate_ind_msg_v01, indication_token),

  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_authenticate_ind_msg_v01, resp),
  0, 1,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_authenticate_ind_msg_v01, card_result) - QMI_IDL_OFFSET8(uim_authenticate_ind_msg_v01, card_result_valid)),
  0x10,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_authenticate_ind_msg_v01, card_result),
  3, 0,

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_authenticate_ind_msg_v01, content) - QMI_IDL_OFFSET8(uim_authenticate_ind_msg_v01, content_valid)),
  0x11,
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN | QMI_IDL_FLAGS_SZ_IS_16 |   QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_authenticate_ind_msg_v01, content),
  ((QMI_UIM_AUTHENTICATE_RESULT_MAX_V01) & 0xFF), ((QMI_UIM_AUTHENTICATE_RESULT_MAX_V01) >> 8),
  QMI_IDL_OFFSET8(uim_authenticate_ind_msg_v01, content) - QMI_IDL_OFFSET8(uim_authenticate_ind_msg_v01, content_len)
};

static const uint8_t uim_close_session_req_msg_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x01,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_close_session_req_msg_v01, session_information),
  0, 0
};

static const uint8_t uim_close_session_resp_msg_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_close_session_resp_msg_v01, resp),
  0, 1
};

static const uint8_t uim_get_service_status_req_msg_data_v01[] = {
  0x01,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_get_service_status_req_msg_v01, session_information),
  0, 0,

  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x02,
  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(uim_get_service_status_req_msg_v01, mask)
};

static const uint8_t uim_get_service_status_resp_msg_data_v01[] = {
  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_get_service_status_resp_msg_v01, resp),
  0, 1,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_get_service_status_resp_msg_v01, fdn_status) - QMI_IDL_OFFSET8(uim_get_service_status_resp_msg_v01, fdn_status_valid)),
  0x10,
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_get_service_status_resp_msg_v01, fdn_status),

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_get_service_status_resp_msg_v01, hidden_key_status) - QMI_IDL_OFFSET8(uim_get_service_status_resp_msg_v01, hidden_key_status_valid)),
  0x11,
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_get_service_status_resp_msg_v01, hidden_key_status),

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_get_service_status_resp_msg_v01, index) - QMI_IDL_OFFSET8(uim_get_service_status_resp_msg_v01, index_valid)),
  0x12,
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_get_service_status_resp_msg_v01, index),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_get_service_status_resp_msg_v01, esn_status) - QMI_IDL_OFFSET8(uim_get_service_status_resp_msg_v01, esn_status_valid)),
  0x13,
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_get_service_status_resp_msg_v01, esn_status)
};

static const uint8_t uim_set_service_status_req_msg_data_v01[] = {
  0x01,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_set_service_status_req_msg_v01, session_information),
  0, 0,

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_set_service_status_req_msg_v01, fdn_status) - QMI_IDL_OFFSET8(uim_set_service_status_req_msg_v01, fdn_status_valid)),
  0x10,
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_set_service_status_req_msg_v01, fdn_status)
};

static const uint8_t uim_set_service_status_resp_msg_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_set_service_status_resp_msg_v01, resp),
  0, 1
};

static const uint8_t uim_change_provisioning_session_req_msg_data_v01[] = {
  0x01,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_change_provisioning_session_req_msg_v01, session_change),
  27, 0,

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_change_provisioning_session_req_msg_v01, application_information) - QMI_IDL_OFFSET8(uim_change_provisioning_session_req_msg_v01, application_information_valid)),
  0x10,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_change_provisioning_session_req_msg_v01, application_information),
  28, 0
};

static const uint8_t uim_change_provisioning_session_resp_msg_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_change_provisioning_session_resp_msg_v01, resp),
  0, 1
};

static const uint8_t uim_get_label_req_msg_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x01,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_get_label_req_msg_v01, application_information),
  28, 0
};

static const uint8_t uim_get_label_resp_msg_data_v01[] = {
  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_get_label_resp_msg_v01, resp),
  0, 1,

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_get_label_resp_msg_v01, label) - QMI_IDL_OFFSET8(uim_get_label_resp_msg_v01, label_valid)),
  0x10,
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |   QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_get_label_resp_msg_v01, label),
  QMI_UIM_LABEL_MAX_V01,
  QMI_IDL_OFFSET8(uim_get_label_resp_msg_v01, label) - QMI_IDL_OFFSET8(uim_get_label_resp_msg_v01, label_len)
};

static const uint8_t uim_get_configuration_req_msg_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_get_configuration_req_msg_v01, configuration_mask) - QMI_IDL_OFFSET8(uim_get_configuration_req_msg_v01, configuration_mask_valid)),
  0x10,
  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(uim_get_configuration_req_msg_v01, configuration_mask)
};

static const uint8_t uim_get_configuration_resp_msg_data_v01[] = {
  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_get_configuration_resp_msg_v01, resp),
  0, 1,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_get_configuration_resp_msg_v01, automatic_selection) - QMI_IDL_OFFSET8(uim_get_configuration_resp_msg_v01, automatic_selection_valid)),
  0x10,
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_get_configuration_resp_msg_v01, automatic_selection),

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_get_configuration_resp_msg_v01, personalization_status) - QMI_IDL_OFFSET8(uim_get_configuration_resp_msg_v01, personalization_status_valid)),
  0x11,
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_get_configuration_resp_msg_v01, personalization_status),
  QMI_UIM_PERSO_FEATURE_MAX_V01,
  QMI_IDL_OFFSET8(uim_get_configuration_resp_msg_v01, personalization_status) - QMI_IDL_OFFSET8(uim_get_configuration_resp_msg_v01, personalization_status_len),
  29, 0,

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_get_configuration_resp_msg_v01, halt_subscription) - QMI_IDL_OFFSET8(uim_get_configuration_resp_msg_v01, halt_subscription_valid)),
  0x12,
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_get_configuration_resp_msg_v01, halt_subscription)
};

static const uint8_t uim_send_apdu_req_msg_data_v01[] = {
  0x01,
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_send_apdu_req_msg_v01, slot),

  0x02,
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN | QMI_IDL_FLAGS_SZ_IS_16 |   QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_send_apdu_req_msg_v01, apdu),
  ((QMI_UIM_APDU_DATA_MAX_V01) & 0xFF), ((QMI_UIM_APDU_DATA_MAX_V01) >> 8),
  QMI_IDL_OFFSET8(uim_send_apdu_req_msg_v01, apdu) - QMI_IDL_OFFSET8(uim_send_apdu_req_msg_v01, apdu_len),

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET16RELATIVE(uim_send_apdu_req_msg_v01, channel_id) - QMI_IDL_OFFSET16RELATIVE(uim_send_apdu_req_msg_v01, channel_id_valid)),
  0x10,
  QMI_IDL_FLAGS_OFFSET_IS_16 | QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET16ARRAY(uim_send_apdu_req_msg_v01, channel_id),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET16RELATIVE(uim_send_apdu_req_msg_v01, procedure_bytes) - QMI_IDL_OFFSET16RELATIVE(uim_send_apdu_req_msg_v01, procedure_bytes_valid)),
  0x11,
  QMI_IDL_FLAGS_OFFSET_IS_16 | QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET16ARRAY(uim_send_apdu_req_msg_v01, procedure_bytes)
};

static const uint8_t uim_send_apdu_resp_msg_data_v01[] = {
  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_send_apdu_resp_msg_v01, resp),
  0, 1,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_send_apdu_resp_msg_v01, apdu) - QMI_IDL_OFFSET8(uim_send_apdu_resp_msg_v01, apdu_valid)),
  0x10,
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN | QMI_IDL_FLAGS_SZ_IS_16 |   QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_send_apdu_resp_msg_v01, apdu),
  ((QMI_UIM_APDU_DATA_MAX_V01) & 0xFF), ((QMI_UIM_APDU_DATA_MAX_V01) >> 8),
  QMI_IDL_OFFSET8(uim_send_apdu_resp_msg_v01, apdu) - QMI_IDL_OFFSET8(uim_send_apdu_resp_msg_v01, apdu_len),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET16RELATIVE(uim_send_apdu_resp_msg_v01, long_response) - QMI_IDL_OFFSET16RELATIVE(uim_send_apdu_resp_msg_v01, long_response_valid)),
  0x11,
  QMI_IDL_FLAGS_OFFSET_IS_16 | QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET16ARRAY(uim_send_apdu_resp_msg_v01, long_response),
  30, 0
};

static const uint8_t uim_send_apdu_ind_msg_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x01,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_send_apdu_ind_msg_v01, long_response_chunk),
  31, 0
};

static const uint8_t uim_sap_connection_req_msg_data_v01[] = {
  0x01,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_sap_connection_req_msg_v01, sap_connect),
  32, 0,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_sap_connection_req_msg_v01, disconnect_mode) - QMI_IDL_OFFSET8(uim_sap_connection_req_msg_v01, disconnect_mode_valid)),
  0x10,
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_sap_connection_req_msg_v01, disconnect_mode),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_sap_connection_req_msg_v01, intermediate_get_response) - QMI_IDL_OFFSET8(uim_sap_connection_req_msg_v01, intermediate_get_response_valid)),
  0x11,
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_sap_connection_req_msg_v01, intermediate_get_response)
};

static const uint8_t uim_sap_connection_resp_msg_data_v01[] = {
  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_sap_connection_resp_msg_v01, resp),
  0, 1,

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_sap_connection_resp_msg_v01, sap_state) - QMI_IDL_OFFSET8(uim_sap_connection_resp_msg_v01, sap_state_valid)),
  0x10,
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_sap_connection_resp_msg_v01, sap_state)
};

static const uint8_t uim_sap_request_req_msg_data_v01[] = {
  0x01,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_sap_request_req_msg_v01, sap_request),
  33, 0,

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_sap_request_req_msg_v01, apdu) - QMI_IDL_OFFSET8(uim_sap_request_req_msg_v01, apdu_valid)),
  0x10,
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN | QMI_IDL_FLAGS_SZ_IS_16 |   QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_sap_request_req_msg_v01, apdu),
  ((QMI_UIM_APDU_DATA_MAX_V01) & 0xFF), ((QMI_UIM_APDU_DATA_MAX_V01) >> 8),
  QMI_IDL_OFFSET8(uim_sap_request_req_msg_v01, apdu) - QMI_IDL_OFFSET8(uim_sap_request_req_msg_v01, apdu_len)
};

static const uint8_t uim_sap_request_resp_msg_data_v01[] = {
  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_sap_request_resp_msg_v01, resp),
  0, 1,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_sap_request_resp_msg_v01, ATR_value) - QMI_IDL_OFFSET8(uim_sap_request_resp_msg_v01, ATR_value_valid)),
  0x10,
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |   QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_sap_request_resp_msg_v01, ATR_value),
  QMI_UIM_ATR_DATA_MAX_V01,
  QMI_IDL_OFFSET8(uim_sap_request_resp_msg_v01, ATR_value) - QMI_IDL_OFFSET8(uim_sap_request_resp_msg_v01, ATR_value_len),

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET16RELATIVE(uim_sap_request_resp_msg_v01, apdu) - QMI_IDL_OFFSET16RELATIVE(uim_sap_request_resp_msg_v01, apdu_valid)),
  0x11,
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN | QMI_IDL_FLAGS_SZ_IS_16 |   QMI_IDL_FLAGS_OFFSET_IS_16 | QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET16ARRAY(uim_sap_request_resp_msg_v01, apdu),
  ((QMI_UIM_APDU_DATA_MAX_V01) & 0xFF), ((QMI_UIM_APDU_DATA_MAX_V01) >> 8),
  QMI_IDL_OFFSET16RELATIVE(uim_sap_request_resp_msg_v01, apdu) - QMI_IDL_OFFSET16RELATIVE(uim_sap_request_resp_msg_v01, apdu_len),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET16RELATIVE(uim_sap_request_resp_msg_v01, status) - QMI_IDL_OFFSET16RELATIVE(uim_sap_request_resp_msg_v01, status_valid)),
  0x12,
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |   QMI_IDL_FLAGS_OFFSET_IS_16 | QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET16ARRAY(uim_sap_request_resp_msg_v01, status),
  QMI_UIM_CARD_READER_DATA_MAX_V01,
  QMI_IDL_OFFSET16RELATIVE(uim_sap_request_resp_msg_v01, status) - QMI_IDL_OFFSET16RELATIVE(uim_sap_request_resp_msg_v01, status_len)
};

static const uint8_t uim_sap_connection_ind_msg_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_sap_connection_ind_msg_v01, sap_connection_event) - QMI_IDL_OFFSET8(uim_sap_connection_ind_msg_v01, sap_connection_event_valid)),
  0x10,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_sap_connection_ind_msg_v01, sap_connection_event),
  34, 0
};

static const uint8_t uim_logical_channel_req_msg_data_v01[] = {
  0x01,
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_logical_channel_req_msg_v01, slot),

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_logical_channel_req_msg_v01, aid) - QMI_IDL_OFFSET8(uim_logical_channel_req_msg_v01, aid_valid)),
  0x10,
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |   QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_logical_channel_req_msg_v01, aid),
  QMI_UIM_AID_MAX_V01,
  QMI_IDL_OFFSET8(uim_logical_channel_req_msg_v01, aid) - QMI_IDL_OFFSET8(uim_logical_channel_req_msg_v01, aid_len),

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_logical_channel_req_msg_v01, channel_id) - QMI_IDL_OFFSET8(uim_logical_channel_req_msg_v01, channel_id_valid)),
  0x11,
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_logical_channel_req_msg_v01, channel_id),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_logical_channel_req_msg_v01, file_control_information) - QMI_IDL_OFFSET8(uim_logical_channel_req_msg_v01, file_control_information_valid)),
  0x12,
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_logical_channel_req_msg_v01, file_control_information)
};

static const uint8_t uim_logical_channel_resp_msg_data_v01[] = {
  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_logical_channel_resp_msg_v01, resp),
  0, 1,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_logical_channel_resp_msg_v01, channel_id) - QMI_IDL_OFFSET8(uim_logical_channel_resp_msg_v01, channel_id_valid)),
  0x10,
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_logical_channel_resp_msg_v01, channel_id),

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_logical_channel_resp_msg_v01, card_result) - QMI_IDL_OFFSET8(uim_logical_channel_resp_msg_v01, card_result_valid)),
  0x11,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_logical_channel_resp_msg_v01, card_result),
  3, 0,

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_logical_channel_resp_msg_v01, select_response) - QMI_IDL_OFFSET8(uim_logical_channel_resp_msg_v01, select_response_valid)),
  0x12,
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |   QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_logical_channel_resp_msg_v01, select_response),
  QMI_UIM_SELECT_RESPONSE_MAX_V01,
  QMI_IDL_OFFSET8(uim_logical_channel_resp_msg_v01, select_response) - QMI_IDL_OFFSET8(uim_logical_channel_resp_msg_v01, select_response_len)
};

static const uint8_t uim_subscription_ok_req_msg_data_v01[] = {
  0x01,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_subscription_ok_req_msg_v01, session_information),
  0, 0,

  0x02,
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_subscription_ok_req_msg_v01, ok_for_subscription),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_subscription_ok_req_msg_v01, encrypted_imsi) - QMI_IDL_OFFSET8(uim_subscription_ok_req_msg_v01, encrypted_imsi_valid)),
  0x10,
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |   QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_subscription_ok_req_msg_v01, encrypted_imsi),
  QMI_UIM_ENCRYPTED_IMSI_MAX_V01,
  QMI_IDL_OFFSET8(uim_subscription_ok_req_msg_v01, encrypted_imsi) - QMI_IDL_OFFSET8(uim_subscription_ok_req_msg_v01, encrypted_imsi_len)
};

static const uint8_t uim_subscription_ok_resp_msg_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_subscription_ok_resp_msg_v01, resp),
  0, 1
};

static const uint8_t uim_get_atr_req_msg_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x01,
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_get_atr_req_msg_v01, slot)
};

static const uint8_t uim_get_atr_resp_msg_data_v01[] = {
  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_get_atr_resp_msg_v01, resp),
  0, 1,

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_get_atr_resp_msg_v01, atr_value) - QMI_IDL_OFFSET8(uim_get_atr_resp_msg_v01, atr_value_valid)),
  0x10,
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |   QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_get_atr_resp_msg_v01, atr_value),
  QMI_UIM_ATR_MAX_V01,
  QMI_IDL_OFFSET8(uim_get_atr_resp_msg_v01, atr_value) - QMI_IDL_OFFSET8(uim_get_atr_resp_msg_v01, atr_value_len)
};

static const uint8_t uim_open_logical_channel_req_msg_data_v01[] = {
  0x01,
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_open_logical_channel_req_msg_v01, slot),

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_open_logical_channel_req_msg_v01, aid) - QMI_IDL_OFFSET8(uim_open_logical_channel_req_msg_v01, aid_valid)),
  0x10,
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |   QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_open_logical_channel_req_msg_v01, aid),
  QMI_UIM_AID_MAX_V01,
  QMI_IDL_OFFSET8(uim_open_logical_channel_req_msg_v01, aid) - QMI_IDL_OFFSET8(uim_open_logical_channel_req_msg_v01, aid_len),

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_open_logical_channel_req_msg_v01, file_control_information) - QMI_IDL_OFFSET8(uim_open_logical_channel_req_msg_v01, file_control_information_valid)),
  0x11,
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(uim_open_logical_channel_req_msg_v01, file_control_information)
};

static const uint8_t uim_open_logical_channel_resp_msg_data_v01[] = {
  0x02,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_open_logical_channel_resp_msg_v01, resp),
  0, 1,

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_open_logical_channel_resp_msg_v01, channel_id) - QMI_IDL_OFFSET8(uim_open_logical_channel_resp_msg_v01, channel_id_valid)),
  0x10,
  QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_open_logical_channel_resp_msg_v01, channel_id),

  QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_open_logical_channel_resp_msg_v01, card_result) - QMI_IDL_OFFSET8(uim_open_logical_channel_resp_msg_v01, card_result_valid)),
  0x11,
  QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(uim_open_logical_channel_resp_msg_v01, card_result),
  3, 0,

  QMI_IDL_TLV_FLAGS_LAST_TLV | QMI_IDL_TLV_FLAGS_OPTIONAL | (QMI_IDL_OFFSET8(uim_open_logical_channel_resp_msg_v01, select_response) - QMI_IDL_OFFSET8(uim_open_logical_channel_resp_msg_v01, select_response_valid)),
  0x12,
  QMI_IDL_FLAGS_IS_ARRAY | QMI_IDL_FLAGS_IS_VARIABLE_LEN |   QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(uim_open_logical_channel_resp_msg_v01, select_response),
  QMI_UIM_SELECT_RESPONSE_MAX_V01,
  QMI_IDL_OFFSET8(uim_open_logical_channel_resp_msg_v01, select_response) - QMI_IDL_OFFSET8(uim_open_logical_channel_resp_msg_v01, select_response_len)
};

/* Type Table */
static const qmi_idl_type_table_entry  uim_type_table_v01[] = {
  {sizeof(uim_session_information_type_v01), uim_session_information_type_data_v01},
  {sizeof(uim_file_id_type_v01), uim_file_id_type_data_v01},
  {sizeof(uim_read_transparent_type_v01), uim_read_transparent_type_data_v01},
  {sizeof(uim_card_result_type_v01), uim_card_result_type_data_v01},
  {sizeof(uim_read_transparent_result_type_v01), uim_read_transparent_result_type_data_v01},
  {sizeof(uim_read_record_type_v01), uim_read_record_type_data_v01},
  {sizeof(uim_read_record_result_type_v01), uim_read_record_result_type_data_v01},
  {sizeof(uim_read_additional_record_result_type_v01), uim_read_additional_record_result_type_data_v01},
  {sizeof(uim_write_transparent_type_v01), uim_write_transparent_type_data_v01},
  {sizeof(uim_write_record_type_v01), uim_write_record_type_data_v01},
  {sizeof(uim_file_attributes_type_v01), uim_file_attributes_type_data_v01},
  {sizeof(uim_register_refresh_file_id_type_v01), uim_register_refresh_file_id_type_data_v01},
  {sizeof(uim_register_refresh_type_v01), uim_register_refresh_type_data_v01},
  {sizeof(uim_refresh_event_type_v01), uim_refresh_event_type_data_v01},
  {sizeof(uim_set_pin_protection_type_v01), uim_set_pin_protection_type_data_v01},
  {sizeof(uim_pin_retries_left_type_v01), uim_pin_retries_left_type_data_v01},
  {sizeof(uim_encrypted_pin_type_v01), uim_encrypted_pin_type_data_v01},
  {sizeof(uim_verify_pin_type_v01), uim_verify_pin_type_data_v01},
  {sizeof(uim_unblock_pin_type_v01), uim_unblock_pin_type_data_v01},
  {sizeof(uim_change_pin_type_v01), uim_change_pin_type_data_v01},
  {sizeof(uim_depersonalization_type_v01), uim_depersonalization_type_data_v01},
  {sizeof(uim_perso_retries_left_type_v01), uim_perso_retries_left_type_data_v01},
  {sizeof(uim_pin_info_type_v01), uim_pin_info_type_data_v01},
  {sizeof(app_info_type_v01), app_info_type_data_v01},
  {sizeof(card_info_type_v01), card_info_type_data_v01},
  {sizeof(uim_card_status_type_v01), uim_card_status_type_data_v01},
  {sizeof(uim_authentication_data_type_v01), uim_authentication_data_type_data_v01},
  {sizeof(uim_session_change_type_v01), uim_session_change_type_data_v01},
  {sizeof(uim_application_information_type_v01), uim_application_information_type_data_v01},
  {sizeof(uim_personalization_status_type_v01), uim_personalization_status_type_data_v01},
  {sizeof(uim_send_apdu_long_response_type_v01), uim_send_apdu_long_response_type_data_v01},
  {sizeof(uim_send_apdu_long_response_chunk_type_v01), uim_send_apdu_long_response_chunk_type_data_v01},
  {sizeof(uim_sap_connect_type_v01), uim_sap_connect_type_data_v01},
  {sizeof(uim_sap_request_type_v01), uim_sap_request_type_data_v01},
  {sizeof(uim_sap_indication_type_v01), uim_sap_indication_type_data_v01}
};

/* Message Table */
static const qmi_idl_message_table_entry uim_message_table_v01[] = {
  {0, 0},
  {sizeof(uim_reset_resp_msg_v01), uim_reset_resp_msg_data_v01},
  {sizeof(uim_read_transparent_req_msg_v01), uim_read_transparent_req_msg_data_v01},
  {sizeof(uim_read_transparent_resp_msg_v01), uim_read_transparent_resp_msg_data_v01},
  {sizeof(uim_read_transparent_ind_msg_v01), uim_read_transparent_ind_msg_data_v01},
  {sizeof(uim_read_record_req_msg_v01), uim_read_record_req_msg_data_v01},
  {sizeof(uim_read_record_resp_msg_v01), uim_read_record_resp_msg_data_v01},
  {sizeof(uim_read_record_ind_msg_v01), uim_read_record_ind_msg_data_v01},
  {sizeof(uim_write_transparent_req_msg_v01), uim_write_transparent_req_msg_data_v01},
  {sizeof(uim_write_transparent_resp_msg_v01), uim_write_transparent_resp_msg_data_v01},
  {sizeof(uim_write_transparent_ind_msg_v01), uim_write_transparent_ind_msg_data_v01},
  {sizeof(uim_write_record_req_msg_v01), uim_write_record_req_msg_data_v01},
  {sizeof(uim_write_record_resp_msg_v01), uim_write_record_resp_msg_data_v01},
  {sizeof(uim_write_record_ind_msg_v01), uim_write_record_ind_msg_data_v01},
  {sizeof(uim_get_file_attributes_req_msg_v01), uim_get_file_attributes_req_msg_data_v01},
  {sizeof(uim_get_file_attributes_resp_msg_v01), uim_get_file_attributes_resp_msg_data_v01},
  {sizeof(uim_get_file_attributes_ind_msg_v01), uim_get_file_attributes_ind_msg_data_v01},
  {sizeof(uim_refresh_register_req_msg_v01), uim_refresh_register_req_msg_data_v01},
  {sizeof(uim_refresh_register_resp_msg_v01), uim_refresh_register_resp_msg_data_v01},
  {sizeof(uim_refresh_ok_req_msg_v01), uim_refresh_ok_req_msg_data_v01},
  {sizeof(uim_refresh_ok_resp_msg_v01), uim_refresh_ok_resp_msg_data_v01},
  {sizeof(uim_refresh_complete_req_msg_v01), uim_refresh_complete_req_msg_data_v01},
  {sizeof(uim_refresh_complete_resp_msg_v01), uim_refresh_complete_resp_msg_data_v01},
  {sizeof(uim_refresh_get_last_event_req_msg_v01), uim_refresh_get_last_event_req_msg_data_v01},
  {sizeof(uim_refresh_get_last_event_resp_msg_v01), uim_refresh_get_last_event_resp_msg_data_v01},
  {sizeof(uim_set_pin_protection_req_msg_v01), uim_set_pin_protection_req_msg_data_v01},
  {sizeof(uim_set_pin_protection_resp_msg_v01), uim_set_pin_protection_resp_msg_data_v01},
  {sizeof(uim_set_pin_protection_ind_msg_v01), uim_set_pin_protection_ind_msg_data_v01},
  {sizeof(uim_verify_pin_req_msg_v01), uim_verify_pin_req_msg_data_v01},
  {sizeof(uim_verify_pin_resp_msg_v01), uim_verify_pin_resp_msg_data_v01},
  {sizeof(uim_verify_pin_ind_msg_v01), uim_verify_pin_ind_msg_data_v01},
  {sizeof(uim_unblock_pin_req_msg_v01), uim_unblock_pin_req_msg_data_v01},
  {sizeof(uim_unblock_pin_resp_msg_v01), uim_unblock_pin_resp_msg_data_v01},
  {sizeof(uim_unblock_pin_ind_msg_v01), uim_unblock_pin_ind_msg_data_v01},
  {sizeof(uim_change_pin_req_msg_v01), uim_change_pin_req_msg_data_v01},
  {sizeof(uim_change_pin_resp_msg_v01), uim_change_pin_resp_msg_data_v01},
  {sizeof(uim_change_pin_ind_msg_v01), uim_change_pin_ind_msg_data_v01},
  {sizeof(uim_depersonalization_req_msg_v01), uim_depersonalization_req_msg_data_v01},
  {sizeof(uim_depersonalization_resp_msg_v01), uim_depersonalization_resp_msg_data_v01},
  {sizeof(uim_power_down_req_msg_v01), uim_power_down_req_msg_data_v01},
  {sizeof(uim_power_down_resp_msg_v01), uim_power_down_resp_msg_data_v01},
  {sizeof(uim_power_up_req_msg_v01), uim_power_up_req_msg_data_v01},
  {sizeof(uim_power_up_resp_msg_v01), uim_power_up_resp_msg_data_v01},
  {0, 0},
  {sizeof(uim_get_card_status_resp_msg_v01), uim_get_card_status_resp_msg_data_v01},
  {sizeof(uim_event_reg_req_msg_v01), uim_event_reg_req_msg_data_v01},
  {sizeof(uim_event_reg_resp_msg_v01), uim_event_reg_resp_msg_data_v01},
  {sizeof(uim_status_change_ind_msg_v01), uim_status_change_ind_msg_data_v01},
  {sizeof(uim_refresh_ind_msg_v01), uim_refresh_ind_msg_data_v01},
  {sizeof(uim_authenticate_req_msg_v01), uim_authenticate_req_msg_data_v01},
  {sizeof(uim_authenticate_resp_msg_v01), uim_authenticate_resp_msg_data_v01},
  {sizeof(uim_authenticate_ind_msg_v01), uim_authenticate_ind_msg_data_v01},
  {sizeof(uim_close_session_req_msg_v01), uim_close_session_req_msg_data_v01},
  {sizeof(uim_close_session_resp_msg_v01), uim_close_session_resp_msg_data_v01},
  {sizeof(uim_get_service_status_req_msg_v01), uim_get_service_status_req_msg_data_v01},
  {sizeof(uim_get_service_status_resp_msg_v01), uim_get_service_status_resp_msg_data_v01},
  {sizeof(uim_set_service_status_req_msg_v01), uim_set_service_status_req_msg_data_v01},
  {sizeof(uim_set_service_status_resp_msg_v01), uim_set_service_status_resp_msg_data_v01},
  {sizeof(uim_change_provisioning_session_req_msg_v01), uim_change_provisioning_session_req_msg_data_v01},
  {sizeof(uim_change_provisioning_session_resp_msg_v01), uim_change_provisioning_session_resp_msg_data_v01},
  {sizeof(uim_get_label_req_msg_v01), uim_get_label_req_msg_data_v01},
  {sizeof(uim_get_label_resp_msg_v01), uim_get_label_resp_msg_data_v01},
  {sizeof(uim_get_configuration_req_msg_v01), uim_get_configuration_req_msg_data_v01},
  {sizeof(uim_get_configuration_resp_msg_v01), uim_get_configuration_resp_msg_data_v01},
  {sizeof(uim_send_apdu_req_msg_v01), uim_send_apdu_req_msg_data_v01},
  {sizeof(uim_send_apdu_resp_msg_v01), uim_send_apdu_resp_msg_data_v01},
  {sizeof(uim_send_apdu_ind_msg_v01), uim_send_apdu_ind_msg_data_v01},
  {sizeof(uim_sap_connection_req_msg_v01), uim_sap_connection_req_msg_data_v01},
  {sizeof(uim_sap_connection_resp_msg_v01), uim_sap_connection_resp_msg_data_v01},
  {sizeof(uim_sap_request_req_msg_v01), uim_sap_request_req_msg_data_v01},
  {sizeof(uim_sap_request_resp_msg_v01), uim_sap_request_resp_msg_data_v01},
  {sizeof(uim_sap_connection_ind_msg_v01), uim_sap_connection_ind_msg_data_v01},
  {sizeof(uim_logical_channel_req_msg_v01), uim_logical_channel_req_msg_data_v01},
  {sizeof(uim_logical_channel_resp_msg_v01), uim_logical_channel_resp_msg_data_v01},
  {sizeof(uim_subscription_ok_req_msg_v01), uim_subscription_ok_req_msg_data_v01},
  {sizeof(uim_subscription_ok_resp_msg_v01), uim_subscription_ok_resp_msg_data_v01},
  {sizeof(uim_get_atr_req_msg_v01), uim_get_atr_req_msg_data_v01},
  {sizeof(uim_get_atr_resp_msg_v01), uim_get_atr_resp_msg_data_v01},
  {sizeof(uim_open_logical_channel_req_msg_v01), uim_open_logical_channel_req_msg_data_v01},
  {sizeof(uim_open_logical_channel_resp_msg_v01), uim_open_logical_channel_resp_msg_data_v01}
};

/* Predefine the Type Table Object */
static const qmi_idl_type_table_object uim_qmi_idl_type_table_object_v01;

/*Referenced Tables Array*/
static const qmi_idl_type_table_object *uim_qmi_idl_type_table_object_referenced_tables_v01[] =
{&uim_qmi_idl_type_table_object_v01, &common_qmi_idl_type_table_object_v01};

/*Type Table Object*/
static const qmi_idl_type_table_object uim_qmi_idl_type_table_object_v01 = {
  sizeof(uim_type_table_v01)/sizeof(qmi_idl_type_table_entry ),
  sizeof(uim_message_table_v01)/sizeof(qmi_idl_message_table_entry),
  1,
  uim_type_table_v01,
  uim_message_table_v01,
  uim_qmi_idl_type_table_object_referenced_tables_v01
};

/*Arrays of service_message_table_entries for commands, responses and indications*/
static const qmi_idl_service_message_table_entry uim_service_command_messages_v01[] = {
  {QMI_UIM_RESET_REQ_V01, TYPE16(0, 0), 0},
  {QMI_UIM_READ_TRANSPARENT_REQ_V01, TYPE16(0, 2), 71},
  {QMI_UIM_READ_RECORD_REQ_V01, TYPE16(0, 5), 72},
  {QMI_UIM_WRITE_TRANSPARENT_REQ_V01, TYPE16(0, 8), 4163},
  {QMI_UIM_WRITE_RECORD_REQ_V01, TYPE16(0, 11), 322},
  {QMI_UIM_GET_FILE_ATTRIBUTES_REQ_V01, TYPE16(0, 14), 60},
  {QMI_UIM_SET_PIN_PROTECTION_REQ_V01, TYPE16(0, 25), 62},
  {QMI_UIM_VERIFY_PIN_REQ_V01, TYPE16(0, 28), 320},
  {QMI_UIM_UNBLOCK_PIN_REQ_V01, TYPE16(0, 31), 70},
  {QMI_UIM_CHANGE_PIN_REQ_V01, TYPE16(0, 34), 70},
  {QMI_UIM_DEPERSONALIZATION_REQ_V01, TYPE16(0, 37), 22},
  {QMI_UIM_REFRESH_REGISTER_REQ_V01, TYPE16(0, 17), 1344},
  {QMI_UIM_REFRESH_OK_REQ_V01, TYPE16(0, 19), 41},
  {QMI_UIM_REFRESH_COMPLETE_REQ_V01, TYPE16(0, 21), 41},
  {QMI_UIM_REFRESH_GET_LAST_EVENT_REQ_V01, TYPE16(0, 23), 37},
  {QMI_UIM_EVENT_REG_REQ_V01, TYPE16(0, 45), 7},
  {QMI_UIM_GET_CARD_STATUS_REQ_V01, TYPE16(0, 43), 0},
  {QMI_UIM_POWER_DOWN_REQ_V01, TYPE16(0, 39), 4},
  {QMI_UIM_POWER_UP_REQ_V01, TYPE16(0, 41), 8},
  {QMI_UIM_AUTHENTICATE_REQ_V01, TYPE16(0, 49), 1074},
  {QMI_UIM_CLOSE_SESSION_REQ_V01, TYPE16(0, 52), 37},
  {QMI_UIM_GET_SERVICE_STATUS_REQ_V01, TYPE16(0, 54), 44},
  {QMI_UIM_SET_SERVICE_STATUS_REQ_V01, TYPE16(0, 56), 41},
  {QMI_UIM_CHANGE_PROVISIONING_SESSION_REQ_V01, TYPE16(0, 58), 42},
  {QMI_UIM_GET_LABEL_REQ_V01, TYPE16(0, 60), 37},
  {QMI_UIM_GET_CONFIGURATION_REQ_V01, TYPE16(0, 62), 7},
  {QMI_UIM_SEND_APDU_REQ_V01, TYPE16(0, 64), 1041},
  {QMI_UIM_SAP_CONNECTION_REQ_V01, TYPE16(0, 67), 13},
  {QMI_UIM_SAP_REQUEST_REQ_V01, TYPE16(0, 69), 1034},
  {QMI_UIM_LOGICAL_CHANNEL_REQ_V01, TYPE16(0, 72), 48},
  {QMI_UIM_SUBSCRIPTION_OK_REQ_V01, TYPE16(0, 74), 300},
  {QMI_UIM_GET_ATR_REQ_V01, TYPE16(0, 76), 4},
  {QMI_UIM_OPEN_LOGICAL_CHANNEL_REQ_V01, TYPE16(0, 78), 44}
};

static const qmi_idl_service_message_table_entry uim_service_response_messages_v01[] = {
  {QMI_UIM_RESET_RESP_V01, TYPE16(0, 1), 7},
  {QMI_UIM_READ_TRANSPARENT_RESP_V01, TYPE16(0, 3), 4129},
  {QMI_UIM_READ_RECORD_RESP_V01, TYPE16(0, 6), 4380},
  {QMI_UIM_WRITE_TRANSPARENT_RESP_V01, TYPE16(0, 9), 19},
  {QMI_UIM_WRITE_RECORD_RESP_V01, TYPE16(0, 12), 19},
  {QMI_UIM_GET_FILE_ATTRIBUTES_RESP_V01, TYPE16(0, 15), 1072},
  {QMI_UIM_SET_PIN_PROTECTION_RESP_V01, TYPE16(0, 26), 278},
  {QMI_UIM_VERIFY_PIN_RESP_V01, TYPE16(0, 29), 278},
  {QMI_UIM_UNBLOCK_PIN_RESP_V01, TYPE16(0, 32), 278},
  {QMI_UIM_CHANGE_PIN_RESP_V01, TYPE16(0, 35), 278},
  {QMI_UIM_DEPERSONALIZATION_RESP_V01, TYPE16(0, 38), 12},
  {QMI_UIM_REFRESH_REGISTER_RESP_V01, TYPE16(0, 18), 7},
  {QMI_UIM_REFRESH_OK_RESP_V01, TYPE16(0, 20), 7},
  {QMI_UIM_REFRESH_COMPLETE_RESP_V01, TYPE16(0, 22), 7},
  {QMI_UIM_REFRESH_GET_LAST_EVENT_RESP_V01, TYPE16(0, 24), 1348},
  {QMI_UIM_EVENT_REG_RESP_V01, TYPE16(0, 46), 14},
  {QMI_UIM_GET_CARD_STATUS_RESP_V01, TYPE16(0, 44), 779},
  {QMI_UIM_POWER_DOWN_RESP_V01, TYPE16(0, 40), 7},
  {QMI_UIM_POWER_UP_RESP_V01, TYPE16(0, 42), 7},
  {QMI_UIM_AUTHENTICATE_RESP_V01, TYPE16(0, 50), 1048},
  {QMI_UIM_CLOSE_SESSION_RESP_V01, TYPE16(0, 53), 7},
  {QMI_UIM_GET_SERVICE_STATUS_RESP_V01, TYPE16(0, 55), 23},
  {QMI_UIM_SET_SERVICE_STATUS_RESP_V01, TYPE16(0, 57), 7},
  {QMI_UIM_CHANGE_PROVISIONING_SESSION_RESP_V01, TYPE16(0, 59), 7},
  {QMI_UIM_GET_LABEL_RESP_V01, TYPE16(0, 61), 266},
  {QMI_UIM_GET_CONFIGURATION_RESP_V01, TYPE16(0, 63), 79},
  {QMI_UIM_SEND_APDU_RESP_V01, TYPE16(0, 65), 1045},
  {QMI_UIM_SAP_CONNECTION_RESP_V01, TYPE16(0, 68), 11},
  {QMI_UIM_SAP_REQUEST_RESP_V01, TYPE16(0, 70), 1554},
  {QMI_UIM_LOGICAL_CHANNEL_RESP_V01, TYPE16(0, 73), 275},
  {QMI_UIM_SUBSCRIPTION_OK_RESP_V01, TYPE16(0, 75), 7},
  {QMI_UIM_GET_ATR_RESP_V01, TYPE16(0, 77), 44},
  {QMI_UIM_OPEN_LOGICAL_CHANNEL_RESP_V01, TYPE16(0, 79), 275}
};

static const qmi_idl_service_message_table_entry uim_service_indication_messages_v01[] = {
  {QMI_UIM_READ_TRANSPARENT_IND_V01, TYPE16(0, 4), 4129},
  {QMI_UIM_READ_RECORD_IND_V01, TYPE16(0, 7), 4380},
  {QMI_UIM_WRITE_TRANSPARENT_IND_V01, TYPE16(0, 10), 19},
  {QMI_UIM_WRITE_RECORD_IND_V01, TYPE16(0, 13), 19},
  {QMI_UIM_GET_FILE_ATTRIBUTES_IND_V01, TYPE16(0, 16), 1072},
  {QMI_UIM_SET_PIN_PROTECTION_IND_V01, TYPE16(0, 27), 278},
  {QMI_UIM_VERIFY_PIN_IND_V01, TYPE16(0, 30), 278},
  {QMI_UIM_UNBLOCK_PIN_IND_V01, TYPE16(0, 33), 278},
  {QMI_UIM_CHANGE_PIN_IND_V01, TYPE16(0, 36), 278},
  {QMI_UIM_STATUS_CHANGE_IND_V01, TYPE16(0, 47), 760},
  {QMI_UIM_REFRESH_IND_V01, TYPE16(0, 48), 1341},
  {QMI_UIM_AUTHENTICATE_IND_V01, TYPE16(0, 51), 1048},
  {QMI_UIM_SEND_APDU_IND_V01, TYPE16(0, 66), 1037},
  {QMI_UIM_SAP_CONNECTION_IND_V01, TYPE16(0, 71), 5}
};

/*Service Object*/
const struct qmi_idl_service_object uim_qmi_idl_service_object_v01 = {
  0x04,
  0x01,
  0x0B,
  4380,
  { sizeof(uim_service_command_messages_v01)/sizeof(qmi_idl_service_message_table_entry),
    sizeof(uim_service_response_messages_v01)/sizeof(qmi_idl_service_message_table_entry),
    sizeof(uim_service_indication_messages_v01)/sizeof(qmi_idl_service_message_table_entry) },
  { uim_service_command_messages_v01, uim_service_response_messages_v01, uim_service_indication_messages_v01},
  &uim_qmi_idl_type_table_object_v01
};

/* Service Object Accessor */
qmi_idl_service_object_type uim_get_service_object_internal_v01
 ( int32_t idl_maj_version, int32_t idl_min_version, int32_t library_version ){
  if ( UIM_V01_IDL_MAJOR_VERS != idl_maj_version || UIM_V01_IDL_MINOR_VERS != idl_min_version 
       || UIM_V01_IDL_TOOL_VERS != library_version) 
  {
    return NULL;
  } 
  return (qmi_idl_service_object_type)&uim_qmi_idl_service_object_v01;
}

