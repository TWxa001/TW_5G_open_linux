#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "qmi_client.h"
#include "user_identity_module_v01.h"
#include "network_access_service_v01.h"
#include "device_management_service_v01.h"
#include "wireless_data_service_v01.h"
#include "voice_service_v02.h"
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
int client;
#define WDS_MSG_DEFAULT_TIMEOUT 20000

/*qmi message library handle*/
qmi_idl_service_object_type nas_service_obj;
qmi_idl_service_object_type dms_service_obj;
qmi_idl_service_object_type uim_service_obj;
qmi_idl_service_object_type wds_service_obj;

typedef struct qmi_simple_ril_voice_info_tp
{
	int ringing_call_id;
	int existing_call_id;
} qmi_simple_ril_voice_info_t;

static qmi_simple_ril_voice_info_t qmi_simple_ril_voice_info;

/*
  CALL_STATE_ORIGINATING_V02 = 0x01, 
  CALL_STATE_INCOMING_V02 = 0x02, 
  CALL_STATE_CONVERSATION_V02 = 0x03, 
  CALL_STATE_CC_IN_PROGRESS_V02 = 0x04, 
  CALL_STATE_ALERTING_V02 = 0x05, 
  CALL_STATE_HOLD_V02 = 0x06, 
  CALL_STATE_WAITING_V02 = 0x07, 
  CALL_STATE_DISCONNECTING_V02 = 0x08, 
  CALL_STATE_END_V02 = 0x09, 
  CALL_STATE_SETUP_V02 = 0x0A, 
 * */
static char voice_call_status[64] = {0};
static char voice_call_type[64] = {0};
static char voice_call_direction[64] = {0};
/*********************************************************************
 * 
 *  function name:   qmi_voice_get_call_state_str
 *  nots: tranform the call status code to strings.
 *  author: qiaoyongkang
 *  datetime: 2015.05.08
 * 
 **********************************************************************/
char* qmi_voice_get_call_state_str(int state)
    {
    char* res;
    switch (state)
        {
        case CALL_STATE_ORIGINATING_V02: 
            res = "ORIGINATION";
            break;

        case CALL_STATE_INCOMING_V02: 
            res = "INCOMING";
            break;

        case CALL_STATE_CONVERSATION_V02: 
            res = "CONVERSATION";
            break;

        case CALL_STATE_CC_IN_PROGRESS_V02: 
            res = "CC_IN_PROGRESS";
            break;

        case CALL_STATE_ALERTING_V02: 
            res = "ALERTING";
            break;

        case CALL_STATE_HOLD_V02: 
            res = "HOLD";
            break;

        case CALL_STATE_WAITING_V02: 
            res = "WAITING";
            break;

        case CALL_STATE_DISCONNECTING_V02: 
            res = "DISCONNECTING";
            break;

        case CALL_STATE_END_V02: 
            res = "END";
            break;

        case CALL_STATE_SETUP_V02: 
            res = "SETUP";
            break;

        default:
            res = "UNKNOWN";
            break;
        }
    strcpy(voice_call_status,res);
    return res;
 }
/*********************************************************************
 * 
 *  function name:   qmi_voice_get_call_type_str
 *  nots: tranform the call type code to strings.
 *  author: qiaoyongkang
 *  datetime: 2015.05.08
 * 
 **********************************************************************/
char* qmi_voice_get_call_type_str(int state)
    {
    char* res;
    switch (state)
        {
        case CALL_TYPE_VOICE_V02: 
            res = "voice";
            break;

        case CALL_TYPE_VOICE_IP_V02: 
            res = "Voice over IP";
            break;

        case CALL_TYPE_VT_V02: 
            res = "Videotelephony call over IP";
            break;

        case CALL_TYPE_VIDEOSHARE_V02: 
            res = "Video Share call ";
            break;

        case CALL_TYPE_TEST_V02: 
            res = "Test call type ";
            break;

        case CALL_TYPE_OTAPA_V02: 
            res = "OTAPA";
            break;

        case CALL_TYPE_STD_OTASP_V02: 
            res = "Standard OTASP";
            break;

        case CALL_TYPE_NON_STD_OTASP_V02: 
            res = "Nonstandard OTASP";
            break;

        case CALL_TYPE_EMERGENCY_V02: 
            res = "Emergency";
            break;

        case CALL_TYPE_SUPS_V02: 
            res = "Supplementary service";
            break;
		case CALL_TYPE_EMERGENCY_IP_V02:
			res = "Emergency VOIP call";
			break;
        default:
            res = "UNKNOWN";
            break;
        }
    strcpy(voice_call_type,res);
    return res;
 }
//ok
static void nas_unsol_ind ( qmi_client_type                user_handle,
                            unsigned int                  msg_id,
                            void                          *ind_buf,
                            unsigned int                   ind_buf_len,
                            void                           *ind_cb_data
                  )
{
    uint decoded_payload_len = 0;
    qmi_client_error_type rc;
    void* decoded_payload;
    unsigned int i;

    nas_serving_system_ind_msg_v01 service_ind;
    nas_event_report_ind_msg_v01 report_ind;

    qmi_idl_get_message_c_struct_len(nas_service_obj,
                                QMI_IDL_INDICATION, 
                                msg_id,
                                &decoded_payload_len); 

    decoded_payload = malloc(decoded_payload_len);
    if (decoded_payload == NULL)
    {
        printf("Error: Unable to allocate memory\n");
    }
    else
    {
        rc = qmi_client_message_decode(user_handle,
                                  QMI_IDL_INDICATION,
                                  msg_id,
                                  ind_buf,
                                  ind_buf_len,
                                  decoded_payload,
                                  decoded_payload_len);
    
        if (rc != QMI_NO_ERR)
        {
            printf("Error: Decoding unsolicited indication with id = %u, returned in error = %d\n", msg_id, (int)rc);
        }
        else
        {
            printf("nas msg id %u\n", msg_id);
            switch(msg_id)
            {
                case QMI_NAS_SERVING_SYSTEM_IND_MSG_V01:
                {
                    memcpy(&service_ind,decoded_payload,decoded_payload_len);
                    printf("nas service ind\n");
                    printf("%d\n", service_ind.serving_system.registration_state);
                    printf("%d\n", service_ind.serving_system.radio_if_len);
                    printf("%d\n", service_ind.serving_system.radio_if[0]);
                    printf("%d\n", service_ind.roaming_indicator_valid);
                    printf("%d\n", service_ind.roaming_indicator);
                    printf("%d\n", service_ind.current_plmn_valid);
                    printf("%d,%d\n", service_ind.current_plmn.mobile_country_code,service_ind.current_plmn.mobile_network_code);
                    break;
                }
                case QMI_NAS_EVENT_REPORT_IND_MSG_V01:
                {
                    memcpy(&report_ind,decoded_payload,decoded_payload_len);
                    printf("nas event report ind\n");
                    if(report_ind.signal_strength_valid)
                    {
                        printf("signal strength\n");
                        printf("%d\n", report_ind.signal_strength.sig_strength);
                        printf("%d\n", report_ind.signal_strength.radio_if);
                    }
                    if(report_ind.rf_band_info_list_valid)
                    {
                        printf("rf_band_info_list\n");
                        for(i=0;i<report_ind.rf_band_info_list_len;i++)
                        {
                            printf("radio_if=%d,active_band=%d,active_channel=%d\n", report_ind.rf_band_info_list[i].radio_if,report_ind.rf_band_info_list[i].active_band,report_ind.rf_band_info_list[i].active_channel);
                        }
                    }
                    if(report_ind.registration_reject_reason_valid)
                    {
                        printf("registration_reject_reason\n");
                        printf("%d\n", report_ind.registration_reject_reason.service_domain);
                        printf("%d\n", report_ind.registration_reject_reason.reject_cause);
                    }
                    if(report_ind.rssi_valid)
                    {
                        printf("rssi\n");
                        printf("%d\n", report_ind.rssi.rssi);
                        printf("%d\n", report_ind.rssi.radio_if);
                    }
                    if(report_ind.ecio_valid)
                    {
                        printf("ecio\n");
                        printf("%d\n", report_ind.ecio.ecio);
                        printf("%d\n", report_ind.ecio.radio_if);
                    }
                    if(report_ind.io_valid)
                    {
                        printf("io\n");
                        printf("%d\n", report_ind.io);
                    }
                    if(report_ind.sinr_valid)
                    {
                        printf("sinr\n");
                        printf("%d\n", report_ind.sinr);
                    }
                    if(report_ind.error_rate_valid)
                    {
                        printf("error_rate\n");
                        printf("%d\n", report_ind.error_rate.error_rate);
                        printf("%d\n", report_ind.error_rate.radio_if);
                    }
                    if(report_ind.rsrq_valid)
                    {
                        printf("rsrq\n");
                        printf("%d\n", report_ind.rsrq.rsrq);
                        printf("%d\n", report_ind.rsrq.radio_if);
                    }
                    if(report_ind.snr_valid)
                    {
                        printf("snr\n");
                        printf("%d\n", report_ind.snr);
                    }
                    if(report_ind.rsrp_valid)
                    {
                        printf("rsrp\n");
                        printf("%d\n", report_ind.rsrp);
                    }
                    break;
                }
            }
        }
        free(decoded_payload);
    }
}
//ok
static void dms_unsol_ind ( qmi_client_type                user_handle,
                            unsigned int                  msg_id,
                            void                          *ind_buf,
                            unsigned int                   ind_buf_len,
                            void                           *ind_cb_data
                  )
{
    uint decoded_payload_len = 0;
    qmi_client_error_type rc;
    void* decoded_payload;

    dms_event_report_ind_msg_v01 report_ind;

    qmi_idl_get_message_c_struct_len(dms_service_obj,
                                QMI_IDL_INDICATION, 
                                msg_id,
                                &decoded_payload_len); 

    decoded_payload = malloc(decoded_payload_len);
    if (decoded_payload == NULL)
    {
        printf("Error: Unable to allocate memory\n");
    }
    else
    {
        rc = qmi_client_message_decode(user_handle,
                                  QMI_IDL_INDICATION,
                                  msg_id,
                                  ind_buf,
                                  ind_buf_len,
                                  decoded_payload,
                                  decoded_payload_len);
    
        if (rc != QMI_NO_ERR)
        {
            printf("Error: Decoding unsolicited indication with id = %u, returned in error = %d\n", msg_id, (int)rc);
        }
        else
        {
            printf("dms msg id %u\n", msg_id);
            switch(msg_id)
            {
                case QMI_DMS_EVENT_REPORT_IND_V01:
                {
                    memcpy(&report_ind,decoded_payload,decoded_payload_len);
                    printf("dms event report ind\n");
                    if(report_ind.power_state_valid)
                    {
                        printf("power_state\n");
                        printf("power_status %d,battery_lvl %d\n", report_ind.power_state.power_status,report_ind.power_state.battery_lvl);
                    }
                    if(report_ind.pin1_status_valid)
                    {
                        printf("pin1_status\n");
                        printf("status %d,verify_retries_left %d,unblock_retries_left %d\n", report_ind.pin1_status.status,report_ind.pin1_status.verify_retries_left,report_ind.pin1_status.unblock_retries_left);
                    }
                    if(report_ind.pin2_status_valid)
                    {
                        printf("pin2_status\n");
                        printf("status %d,verify_retries_left %d,unblock_retries_left %d\n", report_ind.pin2_status.status,report_ind.pin2_status.verify_retries_left,report_ind.pin2_status.unblock_retries_left);
                    }
                    if(report_ind.activation_state_valid)
                    {
                        printf("activation_state\n");
                        printf("activation_state %d\n", report_ind.activation_state);
                    }
                    if(report_ind.operating_mode_valid)
                    {
                        printf("operating_mode\n");
                        printf("operating_mode %d\n", report_ind.operating_mode);
                    }
                    if(report_ind.uim_state_valid)
                    {
                        printf("uim_state\n");
                        printf("uim_state %d\n", report_ind.uim_state);
                    }
                    if(report_ind.wireless_disable_state_valid)
                    {
                        printf("wireless_disable_state\n");
                        printf("wireless_disable_state %d\n", report_ind.wireless_disable_state);
                    }
                    if(report_ind.prl_init_valid)
                    {
                        printf("prl_init\n");
                        printf("prl_init %d\n", report_ind.prl_init);
                    }
                    break;
                }
            }
        }
        free(decoded_payload);
    }
}

static void uim_unsol_ind ( qmi_client_type                user_handle,
                            unsigned int                  msg_id,
                            void                          *ind_buf,
                            unsigned int                   ind_buf_len,
                            void                           *ind_cb_data
                  )
{
    uint decoded_payload_len = 0;
    qmi_client_error_type rc;
    void* decoded_payload;

    qmi_idl_get_message_c_struct_len(uim_service_obj,
                                QMI_IDL_INDICATION, 
                                msg_id,
                                &decoded_payload_len); 

    decoded_payload = malloc(decoded_payload_len);
    if (decoded_payload == NULL)
    {
        printf("Error: Unable to allocate memory\n");
    }
    else
    {
        rc = qmi_client_message_decode(user_handle,
                                  QMI_IDL_INDICATION,
                                  msg_id,
                                  ind_buf,
                                  ind_buf_len,
                                  decoded_payload,
                                  decoded_payload_len);
    
        if (rc != QMI_NO_ERR)
        {
            printf("Error: Decoding unsolicited indication with id = %u, returned in error = %d\n", msg_id, (int)rc);
        }
        else
        {
            printf("uim msg id %u\n", msg_id);
        }
        free(decoded_payload);
    }
}

static void wds_unsol_ind ( qmi_client_type                user_handle,
                            unsigned int                  msg_id,
                            void                          *ind_buf,
                            unsigned int                   ind_buf_len,
                            void                           *ind_cb_data
                  )
{
    uint decoded_payload_len = 0;
    qmi_client_error_type rc;
    void* decoded_payload;

    qmi_idl_get_message_c_struct_len(wds_service_obj,
                                QMI_IDL_INDICATION, 
                                msg_id,
                                &decoded_payload_len); 

    decoded_payload = malloc(decoded_payload_len);
    if (decoded_payload == NULL)
    {
        printf("Error: Unable to allocate memory\n");
    }
    else
    {
        rc = qmi_client_message_decode(user_handle,
                                  QMI_IDL_INDICATION,
                                  msg_id,
                                  ind_buf,
                                  ind_buf_len,
                                  decoded_payload,
                                  decoded_payload_len);
    
        if (rc != QMI_NO_ERR)
        {
            printf("Error: Decoding unsolicited indication with id = %u, returned in error = %d\n", msg_id, (int)rc);
        }
        else
        {
            printf("wds msg id %u\n", msg_id);
        }
        free(decoded_payload);
    }
}
/*********************************************************************
 * 
 *  function name:   voice_dail_call
 *  nots:  send a voice dail call request with call number.
 *  author: qiaoyongkang
 *  datetime: 2015.05.08
 * 
 **********************************************************************/
int voice_dail_call(qmi_client_type qmi_user_handle,char* call_number)
{
	voice_dial_call_req_msg_v02  req;  //QMI_VOICE_DIAL_CALL_REQ_V02
	voice_dial_call_resp_msg_v02 rsp;  //QMI_VOICE_DIAL_CALL_RESP_V02  

    int ret;

    /* memset zero */
    memset(&rsp,0,sizeof(rsp));
    memset(&req,0,sizeof(req));
	
	// from the qxdm log, the call type should set as CALL_TYPE_VOICE
	// log file: volte call send a call. qiao 2015.05.08
    strncpy(req.calling_number,call_number,strlen(call_number));
    req.call_type_valid = 1;
    req.call_type = CALL_TYPE_VOICE_V02;

    ret = qmi_client_send_msg_sync(qmi_user_handle,
                                  QMI_VOICE_DIAL_CALL_REQ_V02,
                                  &req,
                                  sizeof(req),
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(ret != QMI_NO_ERR)
    {
        printf("set voice_indication_register_req successful.\n");        
    } else
    {
		printf("set voice_indication_register_req failed. error:0x%x\n", ret);
	}
    return ret;
}
/*********************************************************************
 * 
 *  function name:   voice_reg_call_status_ind
 *  nots: send a request to register a voice call status indication,so 
 *   that you can get the call status from modem side.
 *  author: qiaoyongkang
 *  datetime: 2015.05.08
 * 
 **********************************************************************/
int voice_reg_call_status_ind(qmi_client_type qmi_user_handle)
{
	voice_indication_register_req_msg_v02  req;  //QMI_VOICE_INDICATION_REGISTER_REQ_V02
	voice_indication_register_resp_msg_v02 rsp;  //QMI_VOICE_INDICATION_REGISTER_RESP_V02 
    
    int ret;

    /* memset zero */
    memset(&rsp,0,sizeof(rsp));
    memset(&req,0,sizeof(req));
	
	// from the qxdm log, it seems only registered the handover event.
	// log file: volte call send a call. qiao 2015.05.08
    req.handover_events_valid = 1;
    req.handover_events = 1;

    ret = qmi_client_send_msg_sync(qmi_user_handle,
                                  QMI_VOICE_INDICATION_REGISTER_REQ_V02,
                                  &req,
                                  sizeof(req),
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(ret != QMI_NO_ERR)
    {
        printf("set voice_indication_register_req successful.\n");        
    } else
    {
		printf("set voice_indication_register_req failed. error:0x%x\n", ret);
	}
    return ret;
}

//ok
static int get_signal(qmi_client_type user_handle)
{
    nas_get_signal_strength_req_msg_v01 req;
    nas_get_signal_strength_resp_msg_v01 rsp;
    int rc;

    /* memset the response structure to zero */
    memset(&req,0,sizeof(req));
    memset(&rsp,0,sizeof(rsp));


    req.request_mask_valid = 1;
    req.request_mask = 0x3f;
    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_NAS_GET_SIGNAL_STRENGTH_REQ_MSG_V01,
                                  &req,
                                  sizeof(req),
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {
        printf("sig strength\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
        printf("%d\n", rsp.signal_strength.sig_strength);
        printf("%d\n", rsp.signal_strength.radio_if);
    }
   /* we can check for standard response here */
    return rc;

}
static int get_sig_info(qmi_client_type user_handle)
{
    nas_get_sig_info_resp_msg_v01 rsp;
    int rc;

    /* memset the response structure to zero */
    memset(&rsp,0,sizeof(rsp));

    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_NAS_GET_SIG_INFO_REQ_MSG_V01,
                                  NULL,
                                  0,
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {
        printf("sig strength\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
        if(rsp.cdma_sig_info_valid)
        {
            printf("cdma:rssi %d ecio %d\n",rsp.cdma_sig_info.rssi, rsp.cdma_sig_info.ecio);
        }

        if(rsp.hdr_sig_info_valid)
        {
            printf("hdr:rssi %d ecio %d sinr %d\n",rsp.hdr_sig_info.common_sig_str.rssi, rsp.hdr_sig_info.common_sig_str.ecio,rsp.hdr_sig_info.sinr);
        }

        if(rsp.gsm_sig_info_valid)
        {
            printf("gsm:%d\n",rsp.gsm_sig_info);
        }

        if(rsp.wcdma_sig_info_valid)
        {
            printf("wcdma:rssi %d ecio %d\n",rsp.wcdma_sig_info.rssi, rsp.wcdma_sig_info.ecio);
        }

        if(rsp.lte_sig_info_valid)
        {
            printf("lte:rssi %d rsrq %d rsrp %d snr %d\n",rsp.lte_sig_info.rssi, rsp.lte_sig_info.rsrq, rsp.lte_sig_info.rsrp, rsp.lte_sig_info.snr);
        }

        if(rsp.rscp_valid)
        {
            printf("td:rscp %d\n",rsp.rscp);
        }
    }
   /* we can check for standard response here */
    return rc;

}
//ok
static int get_service(qmi_client_type user_handle)
{
    nas_get_serving_system_resp_msg_v01 rsp;
    int rc;
    int i;

    /* memset the response structure to zero */
    memset(&rsp,0,sizeof(rsp));


    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_NAS_GET_SERVING_SYSTEM_REQ_MSG_V01,
                                  NULL,
                                  0,
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {
        printf("service\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
        printf("%d\n", rsp.serving_system.registration_state);
        printf("%d\n", rsp.serving_system.radio_if_len);
        printf("%d\n", rsp.serving_system.radio_if[0]);
        printf("%d\n", rsp.roaming_indicator_valid);
        printf("%d\n", rsp.roaming_indicator);
        printf("%d\n", rsp.current_plmn_valid);
        printf("%d,%d\n", rsp.current_plmn.mobile_country_code,rsp.current_plmn.mobile_network_code);
        printf("%d,%d\n", rsp.data_capabilities_valid,rsp.data_capabilities_len);
        if(rsp.data_capabilities_valid)
        {
            printf("data capabilities:");
            for(i=0;i<rsp.data_capabilities_len;i++)
            {
                printf("%d,", rsp.data_capabilities[i]);
            }
            printf("\n");
        }

    }
   /* we can check for standard response here */
    return rc;

}
//ok,but cannot get rat
static int get_netlist(qmi_client_type user_handle)
{
    nas_perform_network_scan_req_msg_v01 req;
    nas_perform_network_scan_resp_msg_v01 rsp;
    int rc;
    unsigned int i;

    /* memset the response structure to zero */
    memset(&req,0,sizeof(req));
    memset(&rsp,0,sizeof(rsp));


    req.network_type_valid = 1;
    req.network_type=0x7;
    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_NAS_PERFORM_NETWORK_SCAN_REQ_MSG_V01,
                                  &req,
                                  sizeof(req),
                                  &rsp,
                                  sizeof(rsp),
                                  100000);

    if(rc == QMI_NO_ERR)
    {
        printf("get_netlist\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
        
        if(rsp.nas_3gpp_network_info_valid)
        {
            printf("nas_3gpp_network_info\n");
            for(i=0;i<rsp.nas_3gpp_network_info_len;i++)
            {
                printf("%d\n", rsp.nas_3gpp_network_info[i].mobile_country_code);
                printf("%d\n", rsp.nas_3gpp_network_info[i].mobile_network_code);
                printf("0x%x\n", rsp.nas_3gpp_network_info[i].network_status);
                printf("%s\n", rsp.nas_3gpp_network_info[i].network_description);
            }
        }

        if(rsp.nas_network_radio_access_technology_valid)
        {
            printf("nas_network_radio_access_technology\n");
            for(i=0;i<rsp.nas_network_radio_access_technology_len;i++)
            {
                printf("%d\n", rsp.nas_network_radio_access_technology[i].mcc);
                printf("%d\n", rsp.nas_network_radio_access_technology[i].mnc);
                printf("%d\n", rsp.nas_network_radio_access_technology[i].rat);
            }
        }

        if(rsp.mnc_includes_pcs_digit_valid)
        {
            printf("mnc_includes_pcs_digit\n");
            for(i=0;i<rsp.mnc_includes_pcs_digit_len;i++)
            {
                printf("%d\n", rsp.mnc_includes_pcs_digit[i].mcc);
                printf("%d\n", rsp.mnc_includes_pcs_digit[i].mnc);
                printf("%d\n", rsp.mnc_includes_pcs_digit[i].mnc_includes_pcs_digit);
            }
        }
    }

   /* we can check for standard response here */
    return rc;

}
//ok
static int set_selmod(qmi_client_type user_handle)
{
    nas_initiate_network_register_req_msg_v01 req;
    nas_initiate_network_register_resp_msg_v01 rsp;
    int rc;
    int option;

    /* memset the response structure to zero */
    memset(&req,0,sizeof(req));
    memset(&rsp,0,sizeof(rsp));
    printf("input selmod 'auto(0) or manual(1)'?\n");
    if(scanf("%d", &option) == 0)
    {
        scanf("%*s");
        return -1;
    }
    req.change_duration = NAS_PERMANENT_V01;
    if(option == 0)
    {
        req.register_action = NAS_AUTO_REGISTER_V01;
    }
    else if(option == 1)
    {
        req.register_action = NAS_MANUAL_REGISTER_V01;
        req.manual_network_register_info_valid = 1;
        printf("input mcc:\n");
        if(scanf("%hu", &req.manual_network_register_info.mobile_country_code) == 0)
        {
            scanf("%*s");
            return -1;
        }
        printf("input mnc:\n");
        if(scanf("%hu", &req.manual_network_register_info.mobile_network_code) == 0)
        {
            scanf("%*s");
            return -1;
        }
        printf("input radio_access_technology(gsm(1),wcdma(2),lte(3)):\n");
        if(scanf("%d", &option) == 0)
        {
            scanf("%*s");
            return -1;
        }
        if(option == 1)
        {
            req.manual_network_register_info.radio_access_technology=NAS_RADIO_IF_GSM_V01;
        }
        else if(option == 2)
        {
            req.manual_network_register_info.radio_access_technology=NAS_RADIO_IF_UMTS_V01;
        }
        else if(option ==3)
        {
            req.manual_network_register_info.radio_access_technology=NAS_RADIO_IF_LTE_V01;
        }
        else
        {
            return -1;
        }
    }

    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_NAS_INITIATE_NETWORK_REGISTER_REQ_MSG_V01,
                                  &req,
                                  sizeof(req),
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {
        printf("set_selmod\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);

    }
   /* we can check for standard response here */
    return rc;

}

//failure, 0x47-QMI_ERR_INVALID_QMI_CMD_V01
static int get_selmod(qmi_client_type user_handle)
{
    nas_get_system_selection_preference_resp_msg_v01 rsp;
    int rc;

    /* memset the response structure to zero */
    memset(&rsp,0,sizeof(rsp));

    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_NAS_GET_SYSTEM_SELECTION_PREFERENCE_REQ_MSG_V01,
                                  NULL,
                                  0,
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {
        printf("get_selmod\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
        
        if(rsp.net_sel_pref_valid)
        {
            printf("net_sel_pref\n");
            printf("%d\n", rsp.net_sel_pref);
        }
    }

   /* we can check for standard response here */
    return rc;
}

static int getmode(qmi_client_type user_handle)
{
    nas_get_mode_pref_resp_msg_v01 rsp;
    int rc;

    /* memset the response structure to zero */
    memset(&rsp,0,sizeof(rsp));


    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_NAS_GET_MODE_PREF_REQ_MSG_V01,
                                  NULL,
                                  0,
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {
        printf("getmode\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
        
        if(rsp.idx0_mode_pref_valid)
        {
            printf("idx0_mode_pref\n");
            printf("%d\n", rsp.idx0_mode_pref);
        }

        if(rsp.idx1_mode_pref_valid)
        {
            printf("idx1_mode_pref\n");
            printf("%d\n", rsp.idx1_mode_pref);
        }
    }

   /* we can check for standard response here */
    return rc;

}
//fail,0x01-QMI_ERR_MALFORMED_MSG_V01
//after change nas_technology_pref_type_v01 structure ok(9x15 nochange will be ok)
static int set_nettype(qmi_client_type user_handle)
{
    nas_set_technology_preference_req_msg_v01 req;
    nas_set_technology_preference_resp_msg_v01 rsp;
    int rc;
    int option;

    /* memset the response structure to zero */
    memset(&rsp,0,sizeof(rsp));
    memset(&req,0,sizeof(req));

    printf("input nettype 'auto(0),gsm(1),wcdma(2),lte(3)'?\n");
    if(scanf("%d", &option) == 0)
    {
        scanf("%*s");
        return -1;
    }
    switch(option)
    {
        case 0:
        {
            req.technology_pref.technology_pref=0x00;
            break;
        }
        case 1:
        {
            req.technology_pref.technology_pref=0x06;
            break;
        }
        case 2:
        {
            req.technology_pref.technology_pref=0x0a;
            break;
        }
        case 3:
        {
            req.technology_pref.technology_pref=0x22;
            break;
        }
        default:
        {
            return -1;
        }
    }
    req.technology_pref.duration = 0x00;

    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_NAS_SET_TECHNOLOGY_PREFERENCE_REQ_V01,
                                  &req,
                                  sizeof(req),
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {
        printf("set_nettype\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
    }

   /* we can check for standard response here */
    return rc;

}
//fail,return -45   QMI_IDL_LIB_LENGTH_INCONSISTENCY
//after change nas_active_technology_type_v01 structure ok(9x15 nochange will be ok)
static int get_nettype(qmi_client_type user_handle)
{
    nas_get_technology_preference_resp_type_v01 rsp;
    int rc;

    /* memset the response structure to zero */
    memset(&rsp,0,sizeof(rsp));

    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_NAS_GET_TECHNOLOGY_PREFERENCE_REQ_V01,
                                  NULL,
                                  0,
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {
        printf("get_nettype\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
        printf("active_technology_pref\n");
        printf("%d\n",rsp.active_technology_pref.technology_pref);
        printf("%d\n",rsp.active_technology_pref.duration);
        if(rsp.persistent_technology_pref_valid)
        {
            printf("persistent_technology_pref\n");
            printf("%d\n",rsp.persistent_technology_pref);
        }
    }

   /* we can check for standard response here */
    return rc;

}
//failure, 0x47-QMI_ERR_INVALID_QMI_CMD_V01
int get_system_selection(qmi_client_type user_handle)
{
    nas_get_system_selection_preference_resp_msg_v01 rsp;
    int rc, i;

    /* memset the response structure to zero */
    memset(&rsp,0,sizeof(rsp));

    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_NAS_GET_SYSTEM_SELECTION_PREFERENCE_REQ_MSG_V01,
                                  NULL,
                                  0,
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {
        printf("get system selection\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
        
        if(rsp.emergency_mode_valid)
        {
            printf("emergency_mode\n");
            printf("%d\n", rsp.emergency_mode);
        }

        if(rsp.mode_pref_valid)
        {
            printf("mode_pref\n");
            printf("%d\n", rsp.mode_pref);
        }

        if(rsp.band_pref_valid)
        {
            printf("band_pref\n");
            printf("%llu\n", rsp.band_pref);
        }

        if(rsp.prl_pref_valid)
        {
            printf("prl_pref\n");
            printf("%d\n", rsp.prl_pref);
        }

        if(rsp.roam_pref_valid)
        {
            printf("roam_pref\n");
            printf("%d\n", rsp.roam_pref);
        }

        if(rsp.band_pref_ext_valid)
        {
            printf("band_pref_ext\n");
            printf("%llu\n", rsp.band_pref_ext);
        }

        if(rsp.net_sel_pref_valid)
        {
            printf("net_sel_pref\n");
            printf("%d\n", rsp.net_sel_pref);
        }

        if(rsp.srv_domain_pref_valid)
        {
            printf("srv_domain_pref\n");
            printf("%d\n", rsp.srv_domain_pref);
        }

        
        if(rsp.gw_acq_order_pref_valid)
        {
            printf("gw_acq_order_pref\n");
            printf("%d\n", rsp.gw_acq_order_pref);
        }
        if(rsp.tdscdma_band_pref_valid)
        {
            printf("tdscdma_band_pref\n");
            printf("%d\n", rsp.tdscdma_band_pref);
        }

        if(rsp.manual_net_sel_plmn_valid)
        {
            printf("manual_net_sel_plmn\n");
            printf("%d %d %d\n", rsp.manual_net_sel_plmn.mcc,rsp.manual_net_sel_plmn.mnc, rsp.manual_net_sel_plmn.mnc_includes_pcs_digit);
        }
        if(rsp.acq_order_valid)
        {
            printf("acq_order\n");
            for(i=0;i<rsp.acq_order_len;i++)
            {
                printf("%d\n", rsp.acq_order[i]);
            }
        }
    }

   /* we can check for standard response here */
    return rc;
}

static int set_system_selection(qmi_client_type user_handle)
{
    nas_set_system_selection_preference_req_msg_v01 req;
    nas_set_system_selection_preference_resp_msg_v01 rsp;
    int rc;
    int option;

    /* memset the response structure to zero */
    memset(&rsp,0,sizeof(rsp));
    memset(&req,0,sizeof(req));

    printf("input nettype 'auto(0),gsm(1),wcdma(2),lte(3),td(4),gsm&td(5)'?\n");
    if(scanf("%d", &option) == 0)
    {
        scanf("%*s");
        return -1;
    }
    switch(option)
    {
        case 0:
        {
            req.mode_pref=0x3f;
            break;
        }
        case 1:
        {
            req.mode_pref=0x04;
            break;
        }
        case 2:
        {
            req.mode_pref=0x08;
            break;
        }
        case 3:
        {
            req.mode_pref=0x10;
            break;
        }
        case 4:
        {
            req.mode_pref=0x20;
            break;
        }
        case 5:
        {
            req.mode_pref=0x24;
            break;
        }
        default:
        {
            return -1;
        }
    }
    req.mode_pref_valid=1;
    req.change_duration_valid = 1;
    req.change_duration=0x01;

    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_NAS_SET_SYSTEM_SELECTION_PREFERENCE_REQ_MSG_V01,
                                  &req,
                                  sizeof(req),
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {
        printf("set_sys_sel\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
    }

   /* we can check for standard response here */
    return rc;

}

//failure, 0x47-QMI_ERR_INVALID_QMI_CMD_V01
static int set_roam(qmi_client_type user_handle)
{
    nas_set_system_selection_preference_req_msg_v01 req;
    nas_set_system_selection_preference_resp_msg_v01 rsp;
    int rc;
    int option;

    /* memset the response structure to zero */
    memset(&rsp,0,sizeof(rsp));
    memset(&req,0,sizeof(req));

    printf("input roam control 'allow(1),deny(2)'?\n");
    if(scanf("%d", &option) == 0)
    {
        scanf("%*s");
        return -1;
    }
    switch(option)
    {
        case 1:
        {
            req.roam_pref=NAS_ROAMING_PREF_ANY_V01;//any
            break;
        }
        case 2:
        {
            req.roam_pref=NAS_ROAMING_PREF_OFF_V01;//home_only
            break;
        }
        default:
        {
            return -1;
        }
    }

    req.roam_pref_valid=1;
    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_NAS_SET_SYSTEM_SELECTION_PREFERENCE_REQ_MSG_V01,
                                  &req,
                                  sizeof(req),
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {
        printf("set_roam\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
    }

   /* we can check for standard response here */
    return rc;

}
//failure, 0x47-QMI_ERR_INVALID_QMI_CMD_V01
static int get_roam(qmi_client_type user_handle)
{
    nas_get_system_selection_preference_resp_msg_v01 rsp;
    int rc;

    /* memset the response structure to zero */
    memset(&rsp,0,sizeof(rsp));

    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_NAS_GET_SYSTEM_SELECTION_PREFERENCE_REQ_MSG_V01,
                                  NULL,
                                  0,
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {
        printf("get_roam\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
        
        if(rsp.roam_pref_valid)
        {
            printf("roam_pref\n");
            printf("%d\n", rsp.roam_pref);
        }
    }

   /* we can check for standard response here */
    return rc;
}
//result not match the real status
static int get_sim_status(qmi_client_type user_handle)
{
    dms_uim_get_state_resp_msg_v01 rsp;
    int rc;

    /* memset the response structure to zero */
    memset(&rsp,0,sizeof(rsp));

    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_DMS_UIM_GET_STATE_REQ_V01,
                                  NULL,
                                  0,
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {
        printf("get sim status\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
        
        printf("uim_state\n");
        printf("%d\n", rsp.uim_state);
    }

   /* we can check for standard response here */
    return rc;

}
//ok
static int get_sim_status2(qmi_client_type user_handle)
{
    uim_get_card_status_resp_msg_v01 rsp;
    int rc, i;

    /* memset the response structure to zero */
    memset(&rsp,0,sizeof(rsp));

    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_UIM_GET_CARD_STATUS_REQ_V01,
                                  NULL,
                                  0,
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {
        printf("get sim status2\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
        
        if(rsp.card_status_valid)
        {
            printf("card_status\n");
            printf("card_info\n");
            for(i=0;i<rsp.card_status.card_info_len;i++)
            {
                printf("%d\n", rsp.card_status.card_info[i].card_state);
            }
        }
    }

   /* we can check for standard response here */
    return rc;

}
//ok
static int get_pin_status(qmi_client_type user_handle)
{
    dms_uim_get_pin_status_resp_msg_v01 rsp;
    int rc;

    /* memset the response structure to zero */
    memset(&rsp,0,sizeof(rsp));

    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_DMS_UIM_GET_PIN_STATUS_REQ_V01,
                                  NULL,
                                  0,
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {

        printf("get pin status\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
        if(rsp.pin1_status_valid)
        {        
            printf("pin1_status\n");
            printf("%d\n", rsp.pin1_status.status);
            printf("%d\n", rsp.pin1_status.verify_retries_left);
            printf("%d\n", rsp.pin1_status.unblock_retries_left);
        }
    }

   /* we can check for standard response here */
    return rc;

}
//ok
static int enable_pin(qmi_client_type user_handle)
{
    dms_uim_set_pin_protection_req_msg_v01 req;
    dms_uim_set_pin_protection_resp_msg_v01 rsp;
    int rc;

    /* memset the response structure to zero */
    memset(&rsp,0,sizeof(rsp));
    memset(&req,0,sizeof(req));

    req.pin_protection_info.pin_id=DMS_QMI_PIN_ID_PIN_1_V01;
    req.pin_protection_info.protection_setting_enabled=1;
    printf("input pin code:\n");
    if(scanf("%s", req.pin_protection_info.pin_value) == 0)
    {
        scanf("%*s");
        return -1;
    }
    req.pin_protection_info.pin_value_len=strlen((char*)req.pin_protection_info.pin_value);
    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_DMS_UIM_SET_PIN_PROTECTION_REQ_V01,
                                  &req,
                                  sizeof(req),
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {

        printf("enable_pin\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
        if(rsp.pin_retries_status_valid)
        {        
            printf("pin_retries_status\n");
            printf("%d\n", rsp.pin_retries_status.verify_retries_left);
            printf("%d\n", rsp.pin_retries_status.unblock_retries_left);
        }
    }

   /* we can check for standard response here */
    return rc;

}
//ok
static int disable_pin(qmi_client_type user_handle)
{
    dms_uim_set_pin_protection_req_msg_v01 req;
    dms_uim_set_pin_protection_resp_msg_v01 rsp;
    int rc;

    /* memset the response structure to zero */
    memset(&rsp,0,sizeof(rsp));
    memset(&req,0,sizeof(req));

    req.pin_protection_info.pin_id=DMS_QMI_PIN_ID_PIN_1_V01;
    req.pin_protection_info.protection_setting_enabled=0;
    printf("input pin code:\n");
    if(scanf("%s", req.pin_protection_info.pin_value) == 0)
    {
        scanf("%*s");
        return -1;
    }
    req.pin_protection_info.pin_value_len=strlen((char*)req.pin_protection_info.pin_value);
    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_DMS_UIM_SET_PIN_PROTECTION_REQ_V01,
                                  &req,
                                  sizeof(req),
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {

        printf("disable_pin\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
        if(rsp.pin_retries_status_valid)
        {        
            printf("pin_retries_status\n");
            printf("%d\n", rsp.pin_retries_status.verify_retries_left);
            printf("%d\n", rsp.pin_retries_status.unblock_retries_left);
        }
    }

   /* we can check for standard response here */
    return rc;

}
//ok
static int unlock_pin(qmi_client_type user_handle)
{
    dms_uim_verify_pin_req_msg_v01 req;
    dms_uim_verify_pin_resp_msg_v01 rsp;
    int rc;

    /* memset the response structure to zero */
    memset(&rsp,0,sizeof(rsp));
    memset(&req,0,sizeof(req));

    req.pin_info.pin_id=DMS_QMI_PIN_ID_PIN_1_V01;
    printf("input pin code:\n");
    if(scanf("%s", req.pin_info.pin_value) == 0)
    {
        scanf("%*s");
        return -1;
    }
    req.pin_info.pin_value_len=strlen((char*)req.pin_info.pin_value);
    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_DMS_UIM_VERIFY_PIN_REQ_V01,
                                  &req,
                                  sizeof(req),
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {

        printf("unlock_pin\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
        if(rsp.pin_retries_status_valid)
        {        
            printf("pin_retries_status\n");
            printf("%d\n", rsp.pin_retries_status.verify_retries_left);
            printf("%d\n", rsp.pin_retries_status.unblock_retries_left);
        }
    }

   /* we can check for standard response here */
    return rc;

}
//not test
static int unlock_puk(qmi_client_type user_handle)
{
    dms_uim_unblock_pin_req_msg_v01 req;
    dms_uim_unblock_pin_resp_msg_v01 rsp;
    int rc;

    /* memset the response structure to zero */
    memset(&rsp,0,sizeof(rsp));
    memset(&req,0,sizeof(req));

    req.pin_unblock_info.unblock_pin_id=DMS_QMI_PIN_ID_PIN_1_V01;
    printf("input puk code:\n");
    if(scanf("%s", req.pin_unblock_info.puk_value) == 0)
    {
        scanf("%*s");
        return -1;
    }
    req.pin_unblock_info.puk_value_len=strlen((char*)req.pin_unblock_info.puk_value);

    printf("input new pin code:\n");
    if(scanf("%s", req.pin_unblock_info.new_pin_value) == 0)
    {
        scanf("%*s");
        return -1;
    }
    req.pin_unblock_info.new_pin_value_len=strlen((char*)req.pin_unblock_info.new_pin_value);
    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_DMS_UIM_UNBLOCK_PIN_REQ_V01,
                                  &req,
                                  sizeof(req),
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {

        printf("unlock_puk\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
        if(rsp.pin_retries_status_valid)
        {        
            printf("pin_retries_status\n");
            printf("%d\n", rsp.pin_retries_status.verify_retries_left);
            printf("%d\n", rsp.pin_retries_status.unblock_retries_left);
        }
    }

   /* we can check for standard response here */
    return rc;

}
//ok
static int change_pin(qmi_client_type user_handle)
{
    dms_uim_change_pin_req_msg_v01 req;
    dms_uim_change_pin_resp_msg_v01 rsp;
    int rc;

    /* memset the response structure to zero */
    memset(&rsp,0,sizeof(rsp));
    memset(&req,0,sizeof(req));

    req.pin_change_info.pin_id=DMS_QMI_PIN_ID_PIN_1_V01;

    printf("input old pin code:\n");
    if(scanf("%s", req.pin_change_info.old_pin_value) == 0)
    {
        scanf("%*s");
        return -1;
    }
    req.pin_change_info.old_pin_value_len=strlen((char*)req.pin_change_info.old_pin_value);

    printf("input new pin code:\n");
    if(scanf("%s", req.pin_change_info.new_pin_value) == 0)
    {
        scanf("%*s");
        return -1;
    }
    req.pin_change_info.new_pin_value_len=strlen((char*)req.pin_change_info.new_pin_value);

    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_DMS_UIM_CHANGE_PIN_REQ_V01,
                                  &req,
                                  sizeof(req),
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {

        printf("change pin\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
        if(rsp.pin_retries_status_valid)
        {        
            printf("pin_retries_status\n");
            printf("%d\n", rsp.pin_retries_status.verify_retries_left);
            printf("%d\n", rsp.pin_retries_status.unblock_retries_left);
        }
    }

   /* we can check for standard response here */
    return rc;

}
//ok
static int set_dms_report(qmi_client_type user_handle)
{
    dms_set_event_report_req_msg_v01 req;
    dms_set_event_report_resp_msg_v01 rsp;
    int rc;

    /* memset the response structure to zero */
    memset(&rsp,0,sizeof(rsp));
    memset(&req,0,sizeof(req));

    req.report_power_state_valid=1;
    req.report_power_state=1;

    req.lvl_limits_valid=1;
    req.lvl_limits.battery_lvl_lower_limit=1;
    req.lvl_limits.battery_lvl_upper_limit=1;

    req.report_pin_state_valid=1;
    req.report_pin_state=1;
    req.report_activation_state_valid=1;
    req.report_activation_state=1;
    req.report_oprt_mode_state_valid=1;
    req.report_oprt_mode_state=1;

    req.report_uim_state_valid=1;
    req.report_uim_state=1;
    req.report_wireless_disable_state_valid=1;
    req.report_wireless_disable_state=1;

    req.report_prl_init_valid=1;
    req.report_prl_init=1;

    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_DMS_SET_EVENT_REPORT_REQ_V01,
                                  &req,
                                  sizeof(req),
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {

        printf("set dms report\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
    }

   /* we can check for standard response here */
    return rc;

}
//ok
static int set_nas_report(qmi_client_type user_handle)
{
    nas_set_event_report_req_msg_v01 req;
    nas_set_event_report_resp_msg_v01 rsp;
    int rc;

    /* memset the response structure to zero */
    memset(&rsp,0,sizeof(rsp));
    memset(&req,0,sizeof(req));

    req.signal_strength_valid=1;
    req.signal_strength.report_signal_strength=1;
    req.signal_strength.report_signal_strength_threshold_list_len=5;
    req.signal_strength.report_signal_strength_threshold_list[0]=-120;
    req.signal_strength.report_signal_strength_threshold_list[1]=-100;
    req.signal_strength.report_signal_strength_threshold_list[2]=-80;
    req.signal_strength.report_signal_strength_threshold_list[3]=-70;
    req.signal_strength.report_signal_strength_threshold_list[4]=-60;

    req.report_rf_band_info_valid=1;
    req.report_rf_band_info=1;

    req.report_reg_reject_valid=1;
    req.report_reg_reject=1;

    req.rssi_indicator_valid=1;
    req.rssi_indicator.report_rssi=1;
    req.rssi_indicator.rssi_delta=5;

    req.ecio_indicator_valid=1;
    req.ecio_indicator.report_ecio=1;
    req.ecio_indicator.ecio_delta=10;

    req.io_indicator_valid=1;
    req.io_indicator.report_io=1;
    req.io_indicator.io_delta=5;

    req.sinr_indicator_valid=1;
    req.sinr_indicator.report_sinr=1;
    req.sinr_indicator.sinr_delta=5;

    req.report_error_rate_valid=1;
    req.report_error_rate=1;

    req.rsrq_indicator_valid=1;
    req.rsrq_indicator.report_rsrq=1;
    req.rsrq_indicator.rsrq_delta=5;

    req.ecio_threshold_indicator_valid=1;
    req.ecio_threshold_indicator.report_ecio=1;
    req.ecio_threshold_indicator.threshold_list_len=5;
    req.ecio_threshold_indicator.threshold_list[0]=-120;
    req.ecio_threshold_indicator.threshold_list[1]=-100;
    req.ecio_threshold_indicator.threshold_list[2]=-80;
    req.ecio_threshold_indicator.threshold_list[3]=-70;
    req.ecio_threshold_indicator.threshold_list[4]=-60;

    req.sinr_threshold_indicator_valid=1;
    req.sinr_threshold_indicator.report_sinr=1;
    req.sinr_threshold_indicator.threshold_list_len=5;
    req.sinr_threshold_indicator.threshold_list[0]=-120;
    req.sinr_threshold_indicator.threshold_list[1]=-100;
    req.sinr_threshold_indicator.threshold_list[2]=-80;
    req.sinr_threshold_indicator.threshold_list[3]=-70;
    req.sinr_threshold_indicator.threshold_list[4]=-60;
    req.lte_snr_delta_indicator_valid=1;
    req.lte_snr_delta_indicator.report_lte_snr=1;
    req.lte_snr_delta_indicator.lte_snr_delta=5;

    req.lte_rsrp_delta_indicator_valid=1;
    req.lte_rsrp_delta_indicator.report_lte_rsrp=1;
    req.lte_rsrp_delta_indicator.lte_rsrp_delta=5;

    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_NAS_SET_EVENT_REPORT_REQ_MSG_V01,
                                  &req,
                                  sizeof(req),
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {

        printf("set nas report\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
    }

   /* we can check for standard response here */
    return rc;

}
int wm_reg_sig_ind(qmi_client_type qmi_user_handle)
{
    nas_indication_register_req_msg_v01 req;
    nas_indication_register_resp_msg_v01 rsp;
    nas_config_sig_info2_req_msg_v01 req1;
    nas_config_sig_info2_resp_msg_v01 rsp1;
    int ret;

    /* memset zero */
    memset(&rsp,0,sizeof(rsp));
    memset(&req,0,sizeof(req));
    memset(&rsp1,0,sizeof(rsp1));
    memset(&req1,0,sizeof(req1));
    /*
    req1.gsm_rssi_threshold_list_valid = 1;
    req1.gsm_rssi_threshold_list_len=8;
    req1.gsm_rssi_threshold_list[0] = -1250;
    req1.gsm_rssi_threshold_list[1] = -1080;
    req1.gsm_rssi_threshold_list[2] = -1020;
    req1.gsm_rssi_threshold_list[3] = -950;
    req1.gsm_rssi_threshold_list[4] = -890;
    req1.gsm_rssi_threshold_list[5] = -840;
    req1.gsm_rssi_threshold_list[6] = -10;
    req1.gsm_rssi_threshold_list[7] = 0;
    */
    req1.gsm_rssi_delta_valid = 1;
    req1.gsm_rssi_delta = 10;

    /*
    req1.wcdma_rssi_threshold_list_valid = 1;
    req1.wcdma_rssi_threshold_list_len=8;
    req1.wcdma_rssi_threshold_list[0] = -1250;
    req1.wcdma_rssi_threshold_list[1] = -1100;
    req1.wcdma_rssi_threshold_list[2] = -1020;
    req1.wcdma_rssi_threshold_list[3] = -970;
    req1.wcdma_rssi_threshold_list[4] = -920;
    req1.wcdma_rssi_threshold_list[5] = -870;
    req1.wcdma_rssi_threshold_list[6] = -50;
    req1.wcdma_rssi_threshold_list[7] = 0;
    */
    req1.wcdma_rssi_delta_valid = 1;
    req1.wcdma_rssi_delta = 10;

    /*
    req1.lte_rssi_threshold_list_valid = 1;
    req1.lte_rssi_threshold_list_len=8;
    req1.lte_rssi_threshold_list[0] = -1250;
    req1.lte_rssi_threshold_list[1] = -1020;
    req1.lte_rssi_threshold_list[2] = -940;
    req1.lte_rssi_threshold_list[3] = -890;
    req1.lte_rssi_threshold_list[4] = -840;
    req1.lte_rssi_threshold_list[5] = -790;
    req1.lte_rssi_threshold_list[6] = -10;
    req1.lte_rssi_threshold_list[7] = 0;
    */
    req1.lte_rssi_delta_valid = 1;
    req1.lte_rssi_delta = 10;

    /*
    req1.tdscdma_rscp_threshold_list_valid = 1;
    req1.tdscdma_rscp_threshold_list_len=8;
    req1.tdscdma_rscp_threshold_list[0] = -1250;
    req1.tdscdma_rscp_threshold_list[1] = -1100;
    req1.tdscdma_rscp_threshold_list[2] = -1020;
    req1.tdscdma_rscp_threshold_list[3] = -970;
    req1.tdscdma_rscp_threshold_list[4] = -920;
    req1.tdscdma_rscp_threshold_list[5] = -870;
    req1.tdscdma_rscp_threshold_list[6] = -50;
    req1.tdscdma_rscp_threshold_list[7] = 0;
    */
    req1.tdscdma_rscp_delta_valid = 1;
    req1.tdscdma_rscp_delta = 10;

    ret = qmi_client_send_msg_sync(qmi_user_handle,
                                  QMI_NAS_CONFIG_SIG_INFO2_REQ_MSG_V01,
                                  &req1,
                                  sizeof(req1),
                                  &rsp1,
                                  sizeof(rsp1),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(ret == QMI_NO_ERR)
    {

        printf("set sig threshold\n");
        printf("result:%d, error:0x%x\n", rsp1.resp.result,rsp1.resp.error);
    } 

    req.sig_info_valid=1;
    req.sig_info=1;
    
    ret = qmi_client_send_msg_sync(qmi_user_handle,
                                  QMI_NAS_INDICATION_REGISTER_REQ_MSG_V01,
                                  &req,
                                  sizeof(req),
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(ret == QMI_NO_ERR)
    {

        printf("reg sig info\n");
        printf("result:%d, error:0x%x\n", rsp.resp.result,rsp.resp.error);
    }
    return ret;
}
static int get_imsi(qmi_client_type user_handle)
{
    dms_uim_get_imsi_resp_msg_v01 rsp;
    int rc;

    /* memset the response structure to zero */
    memset(&rsp,0,sizeof(rsp));

    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_DMS_UIM_GET_IMSI_REQ_V01,
                                  NULL,
                                  0,
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {
        printf("get imsi\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
        
        printf("imsi\n");
        printf("%s\n", rsp.imsi);
    }

   /* we can check for standard response here */
    return rc;

}

static int get_iccid(qmi_client_type user_handle)
{
    dms_uim_get_iccid_resp_msg_v01 rsp;
    int rc;

    /* memset the response structure to zero */
    memset(&rsp,0,sizeof(rsp));

    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_DMS_UIM_GET_ICCID_REQ_V01,
                                  NULL,
                                  0,
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {
        printf("get iccid\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
        
        printf("uim_id\n");
        printf("%s\n", rsp.uim_id);
    }

   /* we can check for standard response here */
    return rc;

}

static int get_ipinfo_bywds(qmi_client_type user_handle)
{
    wds_get_runtime_settings_req_msg_v01 req;
    wds_get_runtime_settings_resp_msg_v01 rsp;
    int rc;
    struct in_addr addr;
    int i;

    /* memset the response structure to zero */
    memset(&req,0,sizeof(req));
    memset(&rsp,0,sizeof(rsp));

    req.requested_settings_valid = 1;
    req.requested_settings = QMI_WDS_MASK_REQ_SETTINGS_DNS_ADDR_V01 | QMI_WDS_MASK_REQ_SETTINGS_IP_ADDR_V01 | QMI_WDS_MASK_REQ_SETTINGS_GATEWAY_INFO_V01 | QMI_WDS_MASK_REQ_SETTINGS_IP_FAMILY_V01 | QMI_WDS_MASK_REQ_SETTINGS_MTU_V01;
    req.requested_settings = (req.requested_settings);

    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_WDS_GET_RUNTIME_SETTINGS_REQ_V01,
                                  &req,
                                  sizeof(req),
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {
        printf("get ipinfo\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
        
        if(rsp.profile_valid)
        {
            printf("profile\n");
            printf("%d, %d\n", rsp.profile.profile_type, rsp.profile.profile_index);
        }

        if(rsp.ipv4_address_preference_valid)
        {
            printf("ipv4_addr\n");
            addr.s_addr = (rsp.ipv4_address_preference);
            printf("%08x, %s\n", rsp.ipv4_address_preference, inet_ntoa(addr));
        }

        if(rsp.ipv4_gateway_addr_valid)
        {
            printf("ipv4_gateway_addr\n");
            addr.s_addr = (rsp.ipv4_gateway_addr);
            printf("%08x, %s\n", rsp.ipv4_gateway_addr, inet_ntoa(addr));
        }

        if(rsp.ipv4_subnet_mask_valid)
        {
            printf("ipv4_subnet_mask\n");
            addr.s_addr = (rsp.ipv4_subnet_mask);
            printf("%08x, %s\n", rsp.ipv4_subnet_mask, inet_ntoa(addr));
        }

        if(rsp.primary_DNS_IPv4_address_preference_valid)
        {
            printf("primary_dns_IPv4_address\n");
            addr.s_addr = (rsp.primary_DNS_IPv4_address_preference);
            printf("%08x, %s\n", rsp.primary_DNS_IPv4_address_preference, inet_ntoa(addr));
        }

        if(rsp.secondary_DNS_IPv4_address_preference_valid)
        {
            printf("secondary_dns_IPv4_address\n");
            addr.s_addr = (rsp.secondary_DNS_IPv4_address_preference);
            printf("%08x, %s\n", rsp.secondary_DNS_IPv4_address_preference, inet_ntoa(addr));
        }
        
        if(rsp.ipv6_addr_valid)
        {
            printf("ipv6_prefix_length %hhd\n", rsp.ipv6_addr.ipv6_prefix_length);
            for(i=0;i<sizeof(rsp.ipv6_addr.ipv6_addr);i++)
                printf("%02hhx:", rsp.ipv6_addr.ipv6_addr[i]);
            printf("\n");
        }

        if(rsp.ipv6_gateway_addr_valid)
        {
            printf("gw ipv6_prefix_length %hhd\n", rsp.ipv6_gateway_addr.ipv6_prefix_length);
            for(i=0;i<sizeof(rsp.ipv6_gateway_addr.ipv6_addr);i++)
                printf("%02hhx:", rsp.ipv6_gateway_addr.ipv6_addr[i]);
            printf("\n");
        }

        if(rsp.primary_dns_IPv6_address_valid)
        {
            printf("primary_dns_IPv6_address\n");
            for(i=0;i<sizeof(rsp.primary_dns_IPv6_address);i++)
                printf("%hu:", rsp.primary_dns_IPv6_address[i]);
            printf("\n");
        }

        if(rsp.secondary_dns_IPv6_address_valid)
        {
            printf("secondary_dns_IPv6_address\n");
            for(i=0;i<sizeof(rsp.secondary_dns_IPv6_address);i++)
                printf("%hu:", rsp.secondary_dns_IPv6_address[i]);
            printf("\n");
        }

        if(rsp.mtu_valid)
        {
            printf("mtu\n");
            printf("%u\n", (rsp.mtu));
        }

        if(rsp.ip_family_valid)
        {
            printf("ip_family\n");
            printf("%d\n", rsp.ip_family);
        }
    }
    else
    {
        printf("get ip failed\n");
    }
   /* we can check for standard response here */
    return rc;
}

static int get_profile(qmi_client_type user_handle)
{
    wds_get_profile_settings_req_msg_v01 req;
    wds_get_profile_settings_resp_msg_v01 rsp;
    int rc;

    /* memset the response structure to zero */
    memset(&req,0,sizeof(req));
    memset(&rsp,0,sizeof(rsp));

    req.profile.profile_type = WDS_PROFILE_TYPE_3GPP_V01;

    printf("input index:\n");
    if(scanf("%hhu", &req.profile.profile_index) == 0)
    {
        scanf("%*s");
        return -1;
    }

    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_WDS_GET_PROFILE_SETTINGS_REQ_V01,
                                  &req,
                                  sizeof(req),
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {
        printf("get profile\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
        
        if(rsp.profile_name_valid)
        {
            printf("profile\n");
            printf("%s\n", rsp.profile_name);
        }

        if(rsp.pdp_type_valid)
        {
            printf("pdp_type\n");
            printf("%d\n", rsp.pdp_type);
        }

        if(rsp.apn_name_valid)
        {
            printf("apn_name_valid\n");
            printf("%s\n", rsp.apn_name);
        }

    }

   /* we can check for standard response here */
    return rc;
}

static int delete_profile(qmi_client_type user_handle)
{
    wds_delete_profile_req_msg_v01 req;
    wds_delete_profile_resp_msg_v01 rsp;
    int rc;

    /* memset the response structure to zero */
    memset(&req,0,sizeof(req));
    memset(&rsp,0,sizeof(rsp));

    req.profile.profile_type = WDS_PROFILE_TYPE_3GPP_V01;

    printf("input index:\n");
    if(scanf("%hhu", &req.profile.profile_index) == 0)
    {
        scanf("%*s");
        return -1;
    }

    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_WDS_DELETE_PROFILE_REQ_V01,
                                  &req,
                                  sizeof(req),
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {
        printf("delete profile\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
        
        if(rsp.extended_error_code_valid)
        {
            printf("extended_error_code\n");
            printf("%d\n", rsp.extended_error_code);
        }

    }


   /* we can check for standard response here */
    return rc;
}

static int create_profile(qmi_client_type user_handle)
{
    wds_create_profile_req_msg_v01 req;
    wds_create_profile_resp_msg_v01 rsp;
    int rc;

    /* memset the response structure to zero */
    memset(&req,0,sizeof(req));
    memset(&rsp,0,sizeof(rsp));

    req.profile_type = WDS_PROFILE_TYPE_3GPP_V01;
    
    req.profile_name_valid = 1;
    printf("input profile name:\n");
    if(scanf("%s", req.profile_name) == 0)
    {
        scanf("%*s");
        return -1;
    }
    
    req.pdp_type_valid = 1;
    req.pdp_type=0;

    req.apn_name_valid = 1;
    printf("input apn name:\n");
    if(scanf("%s", req.apn_name) == 0)
    {
        scanf("%*s");
        return -1;
    }

    req.authentication_preference_valid = 1;
    printf("input authentication:\n");
    if(scanf("%hhu", &req.authentication_preference) == 0)
    {
        scanf("%*s");
        return -1;
    }

    req.username_valid = 1;
    printf("input username:\n");
    if(scanf("%s", req.username) == 0)
    {
        scanf("%*s");
        return -1;
    }

    req.password_valid = 1;
    printf("input password:\n");
    if(scanf("%s", req.password) == 0)
    {
        scanf("%*s");
        return -1;
    }

    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_WDS_CREATE_PROFILE_REQ_V01,
                                  &req,
                                  sizeof(req),
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {
        printf("create profile\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
        
        printf("profile\n");
        printf("%d,%d\n", rsp.profile.profile_type,rsp.profile.profile_index);

    }
   /* we can check for standard response here */
    return rc;
}

static int modify_profile(qmi_client_type user_handle)
{
    wds_modify_profile_settings_req_msg_v01 req;
    wds_modify_profile_settings_resp_msg_v01 rsp;
    int rc;

    /* memset the response structure to zero */
    memset(&req,0,sizeof(req));
    memset(&rsp,0,sizeof(rsp));

    req.profile.profile_type = WDS_PROFILE_TYPE_3GPP_V01;
    printf("input index:\n");
    if(scanf("%hhu", &req.profile.profile_index) == 0)
    {
        scanf("%*s");
        return -1;
    }
    
    req.profile_name_valid = 1;
    printf("input profile name:\n");
    if(scanf("%s", req.profile_name) == 0)
    {
        scanf("%*s");
        return -1;
    }

    req.pdp_type_valid = 1;
    req.pdp_type=0;

    req.apn_name_valid = 1;
    printf("input apn name:\n");
    if(scanf("%s", req.apn_name) == 0)
    {
        scanf("%*s");
        return -1;
    }

    req.authentication_preference_valid = 1;
    printf("input authentication:\n");
    if(scanf("%hhu", &req.authentication_preference) == 0)
    {
        scanf("%*s");
        return -1;
    }

    req.username_valid = 1;
    printf("input username:\n");
    if(scanf("%s", req.username) == 0)
    {
        scanf("%*s");
        return -1;
    }

    req.password_valid = 1;
    printf("input password:\n");
    if(scanf("%s", req.password) == 0)
    {
        scanf("%*s");
        return -1;
    }

    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_WDS_MODIFY_PROFILE_SETTINGS_REQ_V01,
                                  &req,
                                  sizeof(req),
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {
        printf("modify profile\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
        
        if(rsp.extended_error_code_valid)
        {
            printf("extended_error_code\n");
            printf("%d\n", rsp.extended_error_code);
        }

    }

   /* we can check for standard response here */
    return rc;
}

static int get_msisdn(qmi_client_type user_handle)
{
    dms_get_msisdn_resp_msg_v01 rsp;
    int rc;

    /* memset the response structure to zero */
    memset(&rsp,0,sizeof(rsp));

    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_DMS_GET_MSISDN_REQ_V01,
                                  NULL,
                                  0,
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {
        printf("get msisdn\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
        
        printf("voice_number\n");
        printf("%s\n", rsp.voice_number);
        if(rsp.mobile_id_number_valid)
        {
            printf("mobile_id_number\n");
            printf("%s\n", rsp.mobile_id_number);
        }
        if(rsp.imsi_valid)
        {
            printf("imsi\n");
            printf("%s\n", rsp.imsi);
        }
    }

   /* we can check for standard response here */
    return rc;

}

static int get_sninfo(qmi_client_type user_handle)
{
    dms_get_device_serial_numbers_resp_msg_v01 rsp;
    int rc;

    /* memset the response structure to zero */
    memset(&rsp,0,sizeof(rsp));

    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_DMS_GET_DEVICE_SERIAL_NUMBERS_REQ_V01,
                                  NULL,
                                  0,
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {
        printf("get sn info\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
        
        if(rsp.imei_valid)
        {
            printf("imei\n");
            printf("%s\n", rsp.imei);
        }

        if(rsp.meid_valid)
        {
            printf("meid\n");
            printf("%s\n", rsp.meid);
        }

        if(rsp.imeisv_svn_valid)
        {
            printf("imeisv_svn\n");
            printf("%s\n", rsp.imeisv_svn);
        }
    }

   /* we can check for standard response here */
    return rc;

}

static int set_modem(qmi_client_type user_handle)
{
    dms_set_operating_mode_req_msg_v01 req;
	dms_set_operating_mode_resp_msg_v01 rsp;
    int rc;

    /* memset the response structure to zero */
    memset(&rsp,0,sizeof(rsp));
    memset(&req,0,sizeof(req));

    printf("input mode 'online(0),lowpower(1),ftm(2),offline(3),reset(4),shutdown(5)'?\n");
    if(scanf("%d", &req.operating_mode) == 0)
    {
        scanf("%*s");
        return -1;
    }
    if(req.operating_mode < 0 || req.operating_mode > 5)
    {
        printf("err arg\n");
        return -1;
    }

    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_DMS_SET_OPERATING_MODE_REQ_V01,
                                  &req,
                                  sizeof(req),
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {
        printf("set_modem\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
    }

   /* we can check for standard response here */
    return rc;

}

static int set_family(qmi_client_type user_handle, wds_ip_family_enum_v01 family)
{
    int rc;
    wds_set_client_ip_family_pref_req_msg_v01 req;
    wds_set_client_ip_family_pref_resp_msg_v01 rsp;

    /* memset the response structure to zero */
    memset(&req,0,sizeof(req));
    memset(&rsp,0,sizeof(rsp));

    req.ip_preference = family;
    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_WDS_SET_CLIENT_IP_FAMILY_PREF_REQ_V01,
                                  &req,
                                  sizeof(req),
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {
        printf("set family\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
    }

   /* we can check for standard response here */
    return rc;
}
unsigned long pkt_data_handle = 0;
static int dial_v4(qmi_client_type user_handle)
{
    int rc;
    wds_start_network_interface_req_msg_v01 req;
    wds_start_network_interface_resp_msg_v01 rsp;

    /* memset the response structure to zero */
    memset(&req,0,sizeof(req));
    memset(&rsp,0,sizeof(rsp));


    if(set_family(user_handle, WDS_IP_FAMILY_IPV4_V01) != 0)
    {
        printf("set family failed\n");
    }

    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_WDS_START_NETWORK_INTERFACE_REQ_V01,
                                  &req,
                                  sizeof(req),
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {
        printf("start network\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
        printf("pkt_data_handle %08X\n", rsp.pkt_data_handle);
        if(rsp.resp.result == QMI_RESULT_SUCCESS_V01)
        {
            pkt_data_handle = rsp.pkt_data_handle;
        }
    }

   /* we can check for standard response here */
    return rc;
}

static int stop_v4(qmi_client_type user_handle)
{
    int rc;
    wds_stop_network_interface_req_msg_v01 req;
    wds_stop_network_interface_resp_msg_v01 rsp;

    /* memset the response structure to zero */
    memset(&req,0,sizeof(req));
    memset(&rsp,0,sizeof(rsp));
    req.pkt_data_handle = pkt_data_handle;

    rc = qmi_client_send_msg_sync(user_handle,
                                  QMI_WDS_STOP_NETWORK_INTERFACE_REQ_V01,
                                  &req,
                                  sizeof(req),
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(rc == QMI_NO_ERR)
    {
        printf("stop network\n");
        printf("%d, 0x%x\n", rsp.resp.result,rsp.resp.error);
    }

   /* we can check for standard response here */
    return rc;
}

int main(int argc, char *argv[])
{
    qmi_client_error_type rc;
    qmi_client_type nas_user_handle;
    qmi_client_type dms_user_handle;
    qmi_client_type uim_user_handle;
    qmi_client_type wds_user_handle_v4;
    qmi_client_type wds_user_handle_v6;
    int option;
    int run = 1;
    qmi_client_type notifier;
    qmi_cci_os_signal_type os_params;
    uint32_t num_services, num_entries=0;
    qmi_service_info info[10];

   /*Get the nas service object */
    nas_service_obj = nas_get_service_object_v01();
    /*Get the dms service object */
    dms_service_obj = dms_get_service_object_v01();
    uim_service_obj = uim_get_service_object_v01();
    wds_service_obj = wds_get_service_object_v01();

    rc = qmi_client_notifier_init(dms_service_obj, &os_params, &notifier);
  /* Check if the service is up, if not wait on a signal */
  while(1)
  {
    rc = qmi_client_get_service_list( dms_service_obj, NULL, NULL, &num_services);
    printf("PING: qmi_client_get_service_list() returned %d num_services = %d\n", rc, num_services);
    if(rc == QMI_NO_ERR)
      break;
    /* wait for server to come up */
    QMI_CCI_OS_SIGNAL_WAIT(&os_params, 0);
  };
  num_entries = num_services;
  /* The server has come up, store the information in info variable */
  rc = qmi_client_get_service_list( dms_service_obj, info, &num_entries, &num_services);
  printf("PING: qmi_client_get_service_list() returned %d num_entries = %d num_services = %d\n", rc, num_entries, num_services);

  /* Check if the service is up, if not wait on a signal */
  while(1)
  {
    rc = qmi_client_get_service_list( wds_service_obj, NULL, NULL, &num_services);
    printf("PING: qmi_client_get_service_list() returned %d num_services = %d\n", rc, num_services);
    if(rc == QMI_NO_ERR)
      break;
    /* wait for server to come up */
    QMI_CCI_OS_SIGNAL_WAIT(&os_params, 0);
  };
  num_entries = num_services;
 
  /* Check if the service is up, if not wait on a signal */
  while(1)
  {
    rc = qmi_client_get_service_list( nas_service_obj, NULL, NULL, &num_services);
    printf("PING: qmi_client_get_service_list() returned %d num_services = %d\n", rc, num_services);
    if(rc == QMI_NO_ERR)
      break;
    /* wait for server to come up */
    QMI_CCI_OS_SIGNAL_WAIT(&os_params, 0);
  };
  num_entries = num_services;
  /* The server has come up, store the information in info variable */
  rc = qmi_client_get_service_list( nas_service_obj, info, &num_entries, &num_services);
  printf("PING: qmi_client_get_service_list() returned %d num_entries = %d num_services = %d\n", rc, num_entries, num_services);

  /* Check if the service is up, if not wait on a signal */
  while(1)
  {
    rc = qmi_client_get_service_list( uim_service_obj, NULL, NULL, &num_services);
    printf("PING: qmi_client_get_service_list() returned %d num_services = %d\n", rc, num_services);
    if(rc == QMI_NO_ERR)
      break;
    /* wait for server to come up */
    QMI_CCI_OS_SIGNAL_WAIT(&os_params, 0);
  };
  num_entries = num_services;
  /* The server has come up, store the information in info variable */
  rc = qmi_client_get_service_list( uim_service_obj, info, &num_entries, &num_services);
  printf("PING: qmi_client_get_service_list() returned %d num_entries = %d num_services = %d\n", rc, num_entries, num_services);

   
   /* Initialize a connection to first QMI control port */
    rc = qmi_client_init(&info[0],
                        dms_service_obj,
                        dms_unsol_ind,
                        NULL,
                        NULL,
                        &dms_user_handle
                        );

    if (rc != QMI_NO_ERR ) {
         printf("Error: connection not Initialized...Error Code:%d\n",rc);
         return 0;
    }

   /* Initialize a connection to first QMI control port */
    rc = qmi_client_init(&info[0],
                        wds_service_obj,
                        wds_unsol_ind,
                        NULL,
                        NULL,
                        &wds_user_handle_v4
                        );

    if (rc != QMI_NO_ERR ) {
         printf("Error: connection not Initialized...Error Code:%d\n",rc);
         return 0;
    }
    
   /* Initialize a connection to first QMI control port */
    rc = qmi_client_init(&info[1],
                        wds_service_obj,
                        wds_unsol_ind,
                        NULL,
                        NULL,
                        &wds_user_handle_v6
                        );

    if (rc != QMI_NO_ERR ) {
         printf("Error: connection not Initialized...Error Code:%d\n",rc);
         return 0;
    }
    
    /* Initialize a connection to first QMI control port */
    rc = qmi_client_init(&info[0],
                        nas_service_obj,
                        nas_unsol_ind,
                        NULL,
                        NULL,
                        &nas_user_handle
                        );

    if (rc != QMI_NO_ERR ) {
         printf("Error: connection not Initialized...Error Code:%d\n",rc);
         return 0;
    }
    
    /* Initialize a connection to first QMI control port */
    rc = qmi_client_init(&info[0],
                        uim_service_obj,
                        uim_unsol_ind,
                        NULL,
                        NULL,
                        &uim_user_handle
                        );

    if (rc != QMI_NO_ERR ) {
         printf("Error: connection not Initialized...Error Code:%d\n",rc);
         return 0;
    }
    while(run)
    {
        printf("1.get signal\n");
        printf("2.get service\n");
        printf("3.get netlist\n");
        printf("4.set selmod\n");
        printf("5.get selmod\n");
        printf("6.set nettype\n");
        printf("7.get nettype\n");
        printf("8.get system selection\n");
        printf("9.set roam\n");
        printf("10.get roam\n");
        printf("11.get sim status\n");
        printf("12.get pin status\n");
        printf("13.enable pin\n");
        printf("14.disable pin\n");
        printf("15.unlock pin\n");
        printf("16.unlock puk\n");
        printf("17.change pin\n");
        printf("18.set dms report\n");
        printf("19.set nas report\n");
        printf("20.get sim status2\n");
        printf("21.get imsi\n");
        printf("22.get iccid\n");
        printf("23.get msisdn\n");
        printf("24.dial v4\n");
        printf("25.stop v4\n");
        printf("26.get ipinfo by wds\n");
        printf("27.get profile\n");
        printf("28.create profile\n");
        printf("29.modify profile\n");
        printf("30.set sys sel\n");
        printf("31.get sig info\n");
        printf("32.set modem\n");
        printf("33.get sn info\n");
        printf("34.dail v4 from port1\n");
        printf("35.stop v4 from port1\n");
        printf("36.get ip info of v4 from port1\n");   
        printf("-1.exit\n");
        if(scanf("%d", &option) == 0)
        {
            scanf("%*s");            
            continue;
        }        
        switch(option)
        {
            case 1:
            {
                rc = get_signal(nas_user_handle);
                break;
            }
            case 2:
            {
                rc = get_service(nas_user_handle);
                break;
            }
            case 3:
            {
                rc = get_netlist(nas_user_handle);
                break;
            }
            case 4:
            {
                rc = set_selmod(nas_user_handle);
                break;
            }
            case 5:
            {
                rc = get_selmod(nas_user_handle);
                break;
            }
            case 6:
            {
                rc = set_nettype(nas_user_handle);
                break;
            }
            case 7:
            {
                rc = get_nettype(nas_user_handle);
                break;
            }
            case 8:
            {
                rc = get_system_selection(nas_user_handle);
                break;
            }
            case 9:
            {
                rc = set_roam(nas_user_handle);
                break;
            }
            case 10:
            {
                rc = get_roam(nas_user_handle);
                break;
            }
            case 11:
            {
                rc = get_sim_status(dms_user_handle);
                break;
            }
            case 12:
            {
                rc = get_pin_status(dms_user_handle);
                break;
            }
            case 13:
            {
                rc = enable_pin(dms_user_handle);
                break;
            }
            case 14:
            {
                rc = disable_pin(dms_user_handle);
                break;
            }
            case 15:
            {
                rc = unlock_pin(dms_user_handle);
                break;
            }
            case 16:
            {
                rc = unlock_puk(dms_user_handle);
                break;
            }
            case 17:
            {
                rc = change_pin(dms_user_handle);
                break;
            }
            case 18:
            {
                rc = set_dms_report(dms_user_handle);
                break;
            }
            case 19:
            {
                //rc = set_nas_report(nas_user_handle);
                rc = wm_reg_sig_ind(nas_user_handle);
                break;
            }
            case 20:
            {
                rc = get_sim_status2(uim_user_handle);
                break;
            }
            case 21:
            {
                rc = get_imsi(dms_user_handle);
                break;
            }
            case 22:
            {
                rc = get_iccid(dms_user_handle);
                break;
            }
            case 23:
            {
                rc = get_msisdn(dms_user_handle);
                break;
            }
            case 24:
            {
                rc = dial_v4(wds_user_handle_v4);
                break;
            }
            case 25:
            {
                rc = stop_v4(wds_user_handle_v4);
                break;
            }
            case 26:
            {
                rc = get_ipinfo_bywds(wds_user_handle_v4);
                break;
            }
            case 27:
            {
                rc = get_profile(wds_user_handle_v4);
                break;
            }
            case 28:
            {
                rc = create_profile(wds_user_handle_v4);
                break;
            }
            case 29:
            {
                rc = modify_profile(wds_user_handle_v4);
                break;
            }
            case 30:
            {
                rc = set_system_selection(nas_user_handle);
                break;
            }
            case 31:
            {
                rc = get_sig_info(nas_user_handle);
                break;
            }
            case 32:
            {
                rc = set_modem(dms_user_handle);
                break;
            }
            case 33:
            {
                rc = get_sninfo(dms_user_handle);
                break;
            }
            case 34:
            {
                rc = dial_v4(wds_user_handle_v6);
                break;
            }
            case 35:
            {
                rc = stop_v4(wds_user_handle_v6);
                break;
            }
            case 36:
            {
                rc = get_ipinfo_bywds(wds_user_handle_v6);
                break;
            }
            case -1:
            {
                run = 0;
                break;
            }
        }
        printf("rc = %d\n", rc);
    }

    {

        /* Release the client */
        rc = qmi_client_release(nas_user_handle);
        if (rc < 0 ) {
            printf("Release not successful \n");
        }else
        {
            printf("Qmi client release successful \n");
        }
        rc = qmi_client_release(dms_user_handle);
        if (rc < 0 ) {
            printf("Release not successful \n");
        }else
        {
            printf("Qmi client release successful \n");
        }
        rc = qmi_client_release(uim_user_handle);
        if (rc < 0 ) {
            printf("Release not successful \n");
        }else
        {
            printf("Qmi client release successful \n");
        }
        rc = qmi_client_release(wds_user_handle_v4);
        if (rc < 0 ) {
            printf("Release not successful \n");
        }else
        {
            printf("Qmi client release successful \n");
        }
        rc = qmi_client_release(wds_user_handle_v6);
        if (rc < 0 ) {
            printf("Release not successful \n");
        }else
        {
            printf("Qmi client release successful \n");
        }
        printf("Qmi release done.........\n");

    }

   
   return 0;
}
