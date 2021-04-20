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
qmi_idl_service_object_type voice_service_obj;

/*qmi message library handle*/
qmi_idl_service_object_type nas_service_obj;
qmi_idl_service_object_type dms_service_obj;
qmi_idl_service_object_type uim_service_obj;

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


typedef struct qmi_simple_ril_voice_info_tp
{
	int ringing_call_id;
	int existing_call_id;
	char call_num[QMI_VOICE_NUMBER_MAX_V02];
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
	strncpy(qmi_simple_ril_voice_info.call_num,call_number,strlen(call_number));
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
 *  function name:   voice_call_end_request
 *  nots:  to end a voice call neither it is calling nor it has not started. 
 *  author: qiaoyongkang
 *  datetime: 2015.05.12
 * 
 **********************************************************************/
int voice_call_end_request(qmi_client_type qmi_user_handle,uint8_t call_id)
{
	voice_end_call_req_msg_v02  req;  //QMI_VOICE_END_CALL_REQ_V02
	voice_end_call_resp_msg_v02 rsp;  //QMI_VOICE_END_CALL_RESP_V02  

    int ret;

    /* memset zero */
    memset(&rsp,0,sizeof(rsp));
    memset(&req,0,sizeof(req));
	
	// from the qxdm log, the only parameter is call id
	// log file: volte call send a call. qiao 2015.05.08    
    req.call_id = call_id;

    ret = qmi_client_send_msg_sync(qmi_user_handle,
                                  QMI_VOICE_END_CALL_REQ_V02,
                                  &req,
                                  sizeof(req),
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(ret != QMI_NO_ERR)
    {
        printf("to end a call successful.\n");        
		memset(qmi_simple_ril_voice_info.call_num,0,QMI_VOICE_NUMBER_MAX_V02);
    } else
    {
		printf("to end a call failed. error:0x%x\n", ret);
	}
    return ret;	
}
/*********************************************************************
 * 
 *  function name:   voice_call_answer
 *  nots:  to answer a voice all.
 *  author: qiaoyongkang
 *  datetime: 2015.05.12
 * 
 **********************************************************************/
int voice_call_answer(qmi_client_type qmi_user_handle,uint8_t call_id)
{
	voice_answer_call_req_msg_v02  req;  //QMI_VOICE_ANSWER_CALL_REQ_V02
	voice_answer_call_resp_msg_v02 rsp;  //QMI_VOICE_ANSWER_CALL_RESP_V02  

    int ret;

    /* memset zero */
    memset(&rsp,0,sizeof(rsp));
    memset(&req,0,sizeof(req));
	
	// from the qxdm log, the only parameter is call id
	// log file: volte call send a call. qiao 2015.05.08    
    req.call_id = call_id;

    ret = qmi_client_send_msg_sync(qmi_user_handle,
                                  QMI_VOICE_ANSWER_CALL_REQ_V02,
                                  &req,
                                  sizeof(req),
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(ret != QMI_NO_ERR)
    {
        printf("to answer a call successful.\n");        
    } else
    {
		printf("to answer a call failed. error:0x%x\n", ret);
	}
    return ret;	
}
static int qmi_do_voice_manage_calls(qmi_client_type qmi_user_handle,uint8_t call_id, uint8_t action)
 {
    int ret;

    voice_manage_calls_req_msg_v02  req;  //QMI_VOICE_MANAGE_CALLS_REQ_V02
    voice_manage_calls_resp_msg_v02 rsp;  //QMI_VOICE_MANAGE_CALLS_RESP_V02
    
        /* memset zero */
    memset(&rsp,0,sizeof(rsp));
    memset(&req,0,sizeof(req));
    
    printf("%s: call id=%d, action=%d ", __func__,call_id, (int)action);
    if (call_id)	{
		req.call_id = call_id;
		req.call_id_valid = 1;
		req.sups_type = action;
  /**<   Supplementary service type during the call. Values: \n
       - 0x01 -- SUPS_TYPE_RELEASE_HELD_OR_ WAITING -- 
                 Release is held or waiting \n
       - 0x02 -- SUPS_TYPE_RELEASE_ACTIVE_ ACCEPT_HELD_OR_WAITING -- 
                 Release is active and accepting held or waiting \n
       - 0x03 -- SUPS_TYPE_HOLD_ACTIVE_ ACCEPT_WAITING_OR_HELD -- 
                 Hold is active and accepting waiting or held \n
       - 0x04 -- SUPS_TYPE_HOLD_ALL_EXCEPT_ SPECIFIED_CALL -- 
                 Hold all calls except a specified one \n
       - 0x05 -- SUPS_TYPE_MAKE_ CONFERENCE_CALL -- 
                 Make a conference call \n
       - 0x06 -- SUPS_TYPE_EXPLICIT_CALL_ TRANSFER -- 
                 Explicit call transfer \n
       - 0x07 -- SUPS_TYPE_CCBS_ACTIVATION -- 
                 Activate completion of calls to busy subscriber \n
       - 0x08 -- SUPS_TYPE_END_ALL_CALLS -- 
                 End all calls \n
       - 0x09 -- SUPS_TYPE_RELEASE_ SPECIFIED_CALL -- 
                 Release a specified call \n
       - 0x0A -- SUPS_TYPE_LOCAL_HOLD -- 
                 Put all active calls on local hold \n
       - 0x0B -- SUPS_TYPE_LOCAL_UNHOLD --  
                 Retrieve locally held calls
  */	
		ret = qmi_client_send_msg_sync(qmi_user_handle,
								  QMI_VOICE_MANAGE_CALLS_REQ_V02,
								  &req,
								  sizeof(req),
								  &rsp,
								  sizeof(rsp),
								  WDS_MSG_DEFAULT_TIMEOUT);

		if(ret != QMI_NO_ERR)
		{
			printf("do %s successful.\n",__func__);        
		} else
		{
			printf("do %s failed. error:0x%x\n", __func__,ret);
		}
		return ret;		
	}	
}

/*********************************************************************
 * 
 *  function name:   voice_call_hold
 *  nots:  to hold a voice all.
 *  author: qiaoyongkang
 *  datetime: 2015.05.19
 * 
 **********************************************************************/
int voice_call_hold(qmi_client_type qmi_user_handle,uint8_t call_id)
{
	return qmi_do_voice_manage_calls(qmi_user_handle,call_id, 0x03); // HOLD_ACTIVE_ACCEPT_WAITING_OR_HELD
}
/*********************************************************************
 * 
 *  function name:   voice_call_resume
 *  nots:  to esume a holding voice all.
 *  author: qiaoyongkang
 *  datetime: 2015.05.19
 * 
 **********************************************************************/
int voice_call_resume(qmi_client_type qmi_user_handle,uint8_t call_id)
{
	return qmi_do_voice_manage_calls(qmi_user_handle,call_id, 0x04); // HOLD_ALL_EXCEPT_SPECIFIED_CALL
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
/*********************************************************************
 * 
 *  function name:   voice_unsol_ind
 *  nots: receive voice call status indication, so we can know the call 
 *   status
 *  author: qiaoyongkang
 *  datetime: 2015.05.08
 * 
 **********************************************************************/
static void voice_unsol_ind ( qmi_client_type                user_handle,
                            unsigned int                  msg_id,
                            void                          *ind_buf,
                            unsigned int                   ind_buf_len,
                            void                           *ind_cb_data
							)
{
    uint decoded_payload_len = 0;
    qmi_client_error_type rc;    
    void* decoded_payload;
    int i=0;
    voice_all_call_status_ind_msg_v02 voice_status_ind;

    qmi_idl_get_message_c_struct_len(voice_service_obj,
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
            printf("voice msg id %u\n", msg_id);
            switch(msg_id)
            {
                case QMI_VOICE_ALL_CALL_STATUS_IND_V02:
                {
                    memcpy(&voice_status_ind,decoded_payload,decoded_payload_len);
                    printf("voice event  QMI_VOICE_ALL_CALL_STATUS_IND_V02 ind\n");
                    switch(voice_status_ind.call_info_len)    
                    {
						case 0:  //no calls,no output;
							qmi_simple_ril_voice_info.ringing_call_id = 0;
							qmi_simple_ril_voice_info.existing_call_id = 0;
							break;
						case 1:  //single call,simple output;
							qmi_voice_get_call_state_str(voice_status_ind.call_info[0].call_state);
							printf("current call status is %s.\n",voice_call_status);
							qmi_voice_get_call_type_str(voice_status_ind.call_info[0].call_type);
							printf("current call type is %s.\n",voice_call_type);
							qmi_simple_ril_voice_info.ringing_call_id = voice_status_ind.call_info[0].call_id;
							if(CALL_STATE_INCOMING_V02 == voice_status_ind.call_info[0].call_state)
							{
								qmi_simple_ril_voice_info.ringing_call_id = voice_status_ind.call_info[0].call_id;
								if(voice_status_ind.remote_party_number_valid){
									memset(qmi_simple_ril_voice_info.call_num,0,QMI_VOICE_NUMBER_MAX_V02);
									strncpy(qmi_simple_ril_voice_info.call_num, voice_status_ind.remote_party_number[0].number,voice_status_ind.remote_party_number[0].number_len);
									printf("the incoming call number is %s.\n",qmi_simple_ril_voice_info.call_num);
								}
							}
							qmi_simple_ril_voice_info.existing_call_id = voice_status_ind.call_info[0].call_id;
							break;
						default:   //multiple calls;
							printf("we got %d calls current.\n",voice_status_ind.call_info_len);
							for(i=0;i<voice_status_ind.call_info_len;i++)
							{
								qmi_voice_get_call_state_str(voice_status_ind.call_info[i].call_state);
								printf("the %d call status is %s.\n",i+1,voice_call_status);					
								if (CALL_STATE_INCOMING_V02 == voice_status_ind.call_info[i].call_state)
								{
									qmi_simple_ril_voice_info.ringing_call_id = voice_status_ind.call_info[i].call_id;
								}
								qmi_simple_ril_voice_info.existing_call_id = voice_status_ind.call_info[i].call_id;									
							}
							break;
					}              
                    break;
                }
                case QMI_VOICE_USSD_IND_V02:
                {                 
                    printf("voice event report QMI_VOICE_USSD_IND_V02 ind\n");                                     
                    break;
                }
                default:
					printf("known voice event report\n");
					break;
            }
        }
        free(decoded_payload);
    }
}

int main(int argc, char *argv[])
{
   qmi_client_error_type rc;
    qmi_client_type voice_user_handle0;
    qmi_client_type voice_user_handle1;
    qmi_client_type nas_user_handle;
    qmi_client_type dms_user_handle;
    qmi_client_type uim_user_handle;
    int option;
    int run = 1;
    qmi_client_type notifier;
    qmi_cci_os_signal_type os_params;
    uint32_t num_services, num_entries=0;
    qmi_service_info info[10];

    voice_service_obj = voice_get_service_object_v02();
       /*Get the nas service object */
    nas_service_obj = nas_get_service_object_v01();
    /*Get the dms service object */
    dms_service_obj = dms_get_service_object_v01();
    uim_service_obj = uim_get_service_object_v01();

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
		rc = qmi_client_get_service_list( voice_service_obj, NULL, NULL, &num_services);
		printf("PING: qmi_client_get_service_list() returned %d num_services = %d\n", rc, num_services);
		if(rc == QMI_NO_ERR)
		  break;
		/* wait for server to come up */
		QMI_CCI_OS_SIGNAL_WAIT(&os_params, 0);
	  };
	  num_entries = num_services;
	  /* The server has come up, store the information in info variable */
	  rc = qmi_client_get_service_list( voice_service_obj, info, &num_entries, &num_services);
	  printf("PING: voice..qmi_client_get_service_list() returned %d num_entries = %d num_services = %d\n", rc, num_entries, num_services);

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

//	if(access(qmidev[0], F_OK))
	if(1)
    {
				///* Initialize a connection to first QMI control port */
		rc = qmi_client_init(&info[0],
							voice_service_obj,
							voice_unsol_ind,
							NULL,
							NULL,
							&voice_user_handle0
							);

		if (rc != QMI_NO_ERR ) {
			 printf("Error: connection %s not Initialized...Error Code:%d\n",qmidev[0],rc);
			 return 0;
		}
		
		rc = voice_reg_call_status_ind(voice_user_handle0);
		if (rc != QMI_NO_ERR ) {
			 printf("Error: register voice call indication failed...Error Code:%d\n",rc);        
		} 
		printf("access to %s ok, and initilize set indication request ok.\n",qmidev[0]);
    }else
    {
		printf(".....access %s failed.\n",qmidev[0]);
	}

 //	if(access(qmidev[1], F_OK))
	if(1)
    {   
			///* Initialize a connection to first QMI control port */
		rc = qmi_client_init(&info[1],
							voice_service_obj,
							voice_unsol_ind,
							NULL,
							NULL,
							&voice_user_handle1
							);

		if (rc != QMI_NO_ERR ) {
			 printf("Error: connection %s not Initialized...Error Code:%d\n",qmidev[1],rc);
			 return 0;
		}
		
		 rc = voice_reg_call_status_ind(voice_user_handle1);
		if (rc != QMI_NO_ERR ) {
			 printf("Error: register voice call indication failed...Error Code:%d\n",rc);        
		}  
		printf("access to %s ok, and initilize set indication request ok.\n",qmidev[0]);
		
     }else
    {
		printf(".....access %s failed.\n",qmidev[1]);
	}
    
 
    
  

    while(run)
    {
        printf("1.dail a number 18066851341 from handle0.\n");
        printf("11.dail a number 18609284351 from handle1.\n");
        printf("2.answer a call.\n");
        printf("22.answer a call.\n");
        printf("3.reject or end a call\n");
        printf("4.reject or end a call\n");
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
                rc = voice_dail_call(voice_user_handle0,"18066851341");
                break;
            }
            case 2:
            {
                rc = voice_call_answer(voice_user_handle0,qmi_simple_ril_voice_info.ringing_call_id);
                break;
            }
            case 3:
            {
                rc = voice_call_end_request(voice_user_handle0,qmi_simple_ril_voice_info.ringing_call_id);
                break;
            }
            case 11:
            {
                rc = voice_dail_call(voice_user_handle1,"18609284351");
                break;
            }
            case 22:
            {
                rc = voice_call_answer(voice_user_handle1,qmi_simple_ril_voice_info.ringing_call_id);
                break;
            }
            case 33:
            {
                rc = voice_call_end_request(voice_user_handle1,qmi_simple_ril_voice_info.ringing_call_id);
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
		
		rc = qmi_client_release(dms_user_handle);
        if (rc < 0 ) {
            printf("Release not successful \n");
        }else
        {
            printf("Qmi client release successful \n");
        }
        /* Release the client */
       //  if(access(qmidev[0], F_OK))
       	if(1)
		{ 
			rc = qmi_client_release(voice_user_handle0);
			if (rc < 0 ) {
				printf("Release not successful \n");
			}else
			{
				printf("Qmi client release successful \n");
			}
		}
        //if(access(qmidev[1], F_OK))
        if(1)
		{ 
			rc = qmi_client_release(voice_user_handle1);
			if (rc < 0 ) {
				printf("Release not successful \n");
			}else
			{
				printf("Qmi client release successful \n");
			}
		}
        printf("Qmi release done.........\n");

    }

   
   return 0;
}
