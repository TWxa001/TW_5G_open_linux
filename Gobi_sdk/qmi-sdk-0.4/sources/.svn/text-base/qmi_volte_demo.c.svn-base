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
		memset(qmi_simple_ril_voice_info,call_num,0,QMI_VOICE_NUMBER_MAX_V02);
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
    qmi_client_type voice_user_handle;
    int option;
    int run = 1;
    qmi_client_type notifier;
    qmi_cci_os_signal_type os_params;
    uint32_t num_services, num_entries=0;
    qmi_service_info info[10];

    voice_service_obj = voice_get_service_object_v02();

    rc = qmi_client_notifier_init(voice_service_obj, &os_params, &notifier);
	
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
                        voice_service_obj,
                        voice_unsol_ind,
                        NULL,
                        NULL,
                        &voice_user_handle
                        );

    if (rc != QMI_NO_ERR ) {
         printf("Error: connection not Initialized...Error Code:%d\n",rc);
         return 0;
    }
    
    rc = voice_reg_call_status_ind(voice_user_handle);
    if (rc != QMI_NO_ERR ) {
         printf("Error: register voice call indication failed...Error Code:%d\n",rc);        
    }    

    while(run)
    {
        printf("1.dail a number 10010.\n");
        printf("2.answer a call.\n");
        printf("3.reject or end a call\n");
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
                rc = voice_dail_call(voice_user_handle,"10010");
                break;
            }
            case 2:
            {
                rc = voice_call_answer(voice_user_handle,qmi_simple_ril_voice_info.ringing_call_id);
                break;
            }
            case 3:
            {
                rc = voice_call_end_request(voice_user_handle,qmi_simple_ril_voice_info.ringing_call_id);
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
        rc = qmi_client_release(voice_user_handle);
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
