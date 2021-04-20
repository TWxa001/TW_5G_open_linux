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

#define MAX_CALL_ID_NUM 2

typedef struct {
	call_state_enum_v02       call_state[MAX_CALL_ID_NUM];
	call_type_enum_v02        call_type[MAX_CALL_ID_NUM];
	call_direction_enum_v02   call_direction[MAX_CALL_ID_NUM];
	uint8_t                   call_id[MAX_CALL_ID_NUM];	
	//if only one call the current_call_id is current call id ,
	//else the conversation call id the  the current_call_id.
	uint8_t                   current_call_id; 
	char                      call_number[MAX_CALL_ID_NUM][QMI_VOICE_NUMBER_MAX_V02];
} qmi_simple_ril_voice_info_t;

//static qmi_simple_ril_voice_info_t qmi_simple_ril_voice_info[8];
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
  CALL_STATE_SETUP_V02 = 0x0A, ----only volte.
 * */
static char voice_call_status[64] = {0};
static char voice_call_type[64] = {0};
static char voice_call_direction[64] = {0};
static char voice_call_end_reason[64] = {0};

/*********************************************************************
 * 
 *  function name:   print_hex
 *  nots: to print the buffer as hex data.
 *  author: qiaoyongkang
 *  datetime: 2015.05.20
 * 
 **********************************************************************/
void print_hex(
   void *      pBuffer,
   int         bufSize )
{
   char * pPrintBuf;
   int   pos;
   int status;
   
   pPrintBuf = malloc( bufSize * 3 + 1 );
   if (pPrintBuf == NULL)
   {
      printf( "Unable to allocate buffer\n" );
      return;
   }
   memset( pPrintBuf, 0 , bufSize * 3 + 1 );
   
   for (pos = 0; pos < bufSize; pos++)
   {
      status = snprintf( (pPrintBuf + (pos * 3)), 
                         4, 
                         "%02X ", 
                         *(unsigned char *)(pBuffer + pos) );
      if (status != 3)
      {
         printf( "snprintf error %d\n", status );
         free( pPrintBuf );
         return;
      }
   }
   
   printf( "   : %s\n", pPrintBuf );

   free( pPrintBuf );
   pPrintBuf = NULL;
   return;   
}
/*********************************************************************
 * 
 *  function name:   print_voice_all_call_status_ind
 *  nots: to print all qmi voice indication info for debug..
 *  author: qiaoyongkang
 *  datetime: 2015.05.20
 * 
 **********************************************************************/
void print_voice_all_call_status_ind(voice_all_call_status_ind_msg_v02 *status_ind)
{
	int i=0;
	printf("call_info_len-----%u\n",status_ind->call_info_len);
	for(i=0;i<QMI_VOICE_CALL_INFO_MAX_V02;i++){
		printf("call_info-----%d\n",i);
		print_hex(&(status_ind->call_info[i]),sizeof(voice_call_info2_type_v02));
	}
	
	printf("remote_party_number_valid-----%d\n",status_ind->remote_party_number_valid);
	printf("remote_party_number_len-----%d\n",status_ind->remote_party_number_len);
	for(i=0;i<QMI_VOICE_REMOTE_PARTY_NUMBER_ARRAY_MAX_V02;i++){
		printf("remote_party_number-----%d\n",i);
		print_hex(&(status_ind->remote_party_number[i]),sizeof(voice_remote_party_number2_type_v02));
	}
	
	printf("remote_party_name_valid-----%d\n",status_ind->remote_party_name_valid);
	printf("remote_party_name_len-----%d\n",status_ind->remote_party_name_len);
	for(i=0;i<QMI_VOICE_REMOTE_PARTY_NAME_ARRAY_MAX_V02;i++){
		printf("remote_party_name-----%d\n",i);
		print_hex(&(status_ind->remote_party_name[i]),sizeof(voice_remote_party_name2_type_v02));
	}
	
	printf("alerting_type_valid-----%d\n",status_ind->alerting_type_valid);
	printf("alerting_type_len-----%d\n",status_ind->alerting_type_len);
	for(i=0;i<QMI_VOICE_ALERTING_TYPE_ARRAY_MAX_V02;i++){
		printf("alerting_type-----%d\n",i);
		print_hex(&(status_ind->alerting_type[i]),sizeof(voice_alerting_type_type_v02));
	}
	
	printf("srv_opt_valid-----%d\n",status_ind->srv_opt_valid);
	printf("srv_opt_len-----%d\n",status_ind->srv_opt_len);
	for(i=0;i<QMI_VOICE_CALL_END_REASON_ARRAY_MAX_V02;i++){
		printf("srv_opt-----%d\n",i);
		print_hex(&(status_ind->srv_opt[i]),sizeof(voice_srv_opt_type_v02));
	}
	
	printf("call_end_reason_valid-----%d\n",status_ind->call_end_reason_valid);
	printf("call_end_reason_len-----%d\n",status_ind->call_end_reason_len);
	for(i=0;i<QMI_VOICE_SRV_OPT_ARRAY_MAX_V02;i++){
		printf("call_end_reason-----%d\n",i);
		print_hex(&(status_ind->call_end_reason[i]),sizeof(voice_call_end_reason_type_v02));
	}
	
	printf("alpha_id_valid-----%d\n",status_ind->alpha_id_valid);
	printf("alpha_id_len-----%d\n",status_ind->alpha_id_len);
	for(i=0;i<QMI_VOICE_ALPHA_IDENT_ARRAY_MAX_V02;i++){
		printf("alpha_id-----%d\n",i);
		print_hex(&(status_ind->alpha_id[i]),sizeof(voice_alpha_ident_with_id_type_v02));
	}
	
	printf("conn_party_num_valid-----%d\n",status_ind->conn_party_num_valid);
	printf("conn_party_num_len-----%d\n",status_ind->conn_party_num_len);
	for(i=0;i<QMI_VOICE_CONNECTED_PARTY_ARRAY_MAX_V02;i++){
		printf("conn_party_num-----%d\n",i);
		print_hex(&(status_ind->conn_party_num[i]),sizeof(voice_conn_num_with_id_type_v02));
	}	
		
	printf("diagnostic_info_valid-----%d\n",status_ind->diagnostic_info_valid);
	printf("diagnostic_info_len-----%d\n",status_ind->diagnostic_info_len);
	for(i=0;i<QMI_VOICE_DIAGNOSTIC_INFO_ARRAY_MAX_V02;i++){
		printf("diagnostic_info-----%d\n",i);
		print_hex(&(status_ind->diagnostic_info[i]),sizeof(voice_diagnostic_info_with_id_type_v02));
	}
			
	printf("called_party_num_valid-----%d\n",status_ind->called_party_num_valid);
	printf("called_party_num_len-----%d\n",status_ind->called_party_num_len);
	for(i=0;i<QMI_VOICE_CALLED_PARTY_ARRAY_MAX_V02;i++){
		printf("called_party_num-----%d\n",i);
		print_hex(&(status_ind->called_party_num[i]),sizeof(voice_num_with_id_type_v02));
	}
				
	printf("redirecting_party_num_valid-----%d\n",status_ind->redirecting_party_num_valid);
	printf("redirecting_party_num_len-----%d\n",status_ind->redirecting_party_num_len);
	for(i=0;i<QMI_VOICE_REDIRECTING_PARTY_ARRAY_MAX_V02;i++){
		printf("redirecting_party_num-----%d\n",i);
		print_hex(&(status_ind->redirecting_party_num[i]),sizeof(voice_num_with_id_type_v02));
	}
					
	printf("alerting_pattern_valid-----%d\n",status_ind->alerting_pattern_valid);
	printf("alerting_pattern_len-----%d\n",status_ind->alerting_pattern_len);
	for(i=0;i<QMI_VOICE_ALERTING_PATTERN_ARRAY_MAX_V02;i++){
		printf("alerting_pattern-----%d\n",i);
		print_hex(&(status_ind->alerting_pattern[i]),sizeof(voice_alerting_pattern_type_v02));
	}
						
	printf("audio_attrib_valid-----%d\n",status_ind->audio_attrib_valid);
	printf("audio_attrib_len-----%d\n",status_ind->audio_attrib_len);
	for(i=0;i<QMI_VOICE_CALL_ATTRIBUTES_ARRAY_MAX_V02;i++){
		printf("audio_attrib-----%d\n",i);
		print_hex(&(status_ind->audio_attrib[i]),sizeof(voice_call_attributes_type_v02));
	}
}
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
 *  function name:   qmi_voice_get_call_end_reason_str
 *  nots: tranform the call end reason code to strings.
 *  author: qiaoyongkang
 *  //cdma
 *      case CALL_END_CAUSE_REL_NORMAL_V02:  //0x19
 *  datetime: 2015.05.22
 * 
 **********************************************************************/
char* qmi_voice_get_call_end_reason_str(int endreason)
    {
    char* res;
    switch (endreason)
        {
		//wcdma
        case CALL_END_CAUSE_NORMAL_CALL_CLEARING_V02:  //0x91
            res = "user rejected.";
            break;

        case CALL_END_CAUSE_USER_ALERTING_NO_ANSWER_V02:  //0x94
            res = " user does not answer.";
            break;
            
        case CALL_END_CAUSE_NORMAL_UNSPECIFIED_V02:  //0x9C
            res = " user does not online.";
            break;
            
        default:
            res = "UNKNOWN";
            break;
        }
    strcpy(voice_call_end_reason,res);
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
 *  function name:   qmi_nas_decode_service_str
 *  nots: tranform the nas service state code to strings.
 *  author: qiaoyongkang
 *  datetime: 2015.05.26
 * 
 **********************************************************************/

void qmi_nas_decode_service_str(nas_serving_system_type_v01* service_ind)
 {
	 int i = 0;
    /**<   Registration state of the mobile. Values: \n
       - 0x00 -- NOT_REGISTERED        -- Not registered; mobile is not currently 
                                          searching for a new network to provide 
                                          service \n
       - 0x01 -- REGISTERED            -- Registered with a network \n
       - 0x02 -- NOT_REGISTERED_SEARCHING -- Not registered, but mobile is currently 
                                             searching for a new network to provide 
                                             service \n
       - 0x03 -- REGISTRATION_DENIED   -- Registration denied by the visible 
                                          network \n
       - 0x04 -- REGISTRATION_UNKNOWN  -- Registration state is unknown
  */  
    switch (service_ind->registration_state)
    {
        case NAS_NOT_REGISTERED_V01: 
            printf("service state:not registered.\n");
            break;

        case NAS_REGISTERED_V01: 
            printf("service state:registered.\n");
            break;

        case NAS_NOT_REGISTERED_SEARCHING_V01: 
            printf("service state:Not registered and searching.\n");
            break;

        case NAS_REGISTRATION_DENIED_V01: 
            printf("service state:Registration denied.\n");
            break;
        
		case NAS_REGISTRATION_UNKNOWN_V01:			
        default:
            printf("service state:UNKNOWN.\n");
            break;
    } 
      /**<   Circuit-switched domain attach state of the mobile. Values: \n
       - 0x00 -- CS_UNKNOWN  -- Unknown or not applicable \n
       - 0x01 -- CS_ATTACHED -- Attached \n
       - 0x02 -- CS_DETACHED -- Detached
  */
    switch (service_ind->cs_attach_state)
    {
        case NAS_CS_ATTACHED_V01: 
            printf("cs attached state:attached.\n");
            break;

        case NAS_CS_DETACHED_V01: 
            printf("cs attached state:dettached.\n");
            break;

        case NAS_CS_UNKNOWN_V01: 			
        default:
            printf("cs attached state:UNKNOWN.\n");
            break;
    }  
      /**<   Packet-switched domain attach state of the mobile. Values: \n
       - 0x00 -- PS_UNKNOWN  -- Unknown or not applicable \n
       - 0x01 -- PS_ATTACHED -- Attached \n
       - 0x02 -- PS_DETACHED -- Detached
  */ 
    switch (service_ind->cs_attach_state)
    {
        case NAS_PS_ATTACHED_V01: 
            printf("ps attached state:attached.\n");
            break;

        case NAS_PS_DETACHED_V01: 
            printf("ps attached state:dettached.\n");
            break;

        case NAS_PS_UNKNOWN_V01: 			
        default:
            printf("ps attached state:UNKNOWN.\n");
            break;
    } 
   /**<   Type of selected radio access network. Values: \n
       - 0x00 -- SELECTED_NETWORK_UNKNOWN -- Unknown \n
       - 0x01 -- SELECTED_NETWORK_3GPP2   -- 3GPP2 network \n
       - 0x02 -- SELECTED_NETWORK_3GPP    -- 3GPP network
  */   
    switch (service_ind->selected_network)
    {
        case NAS_SELECTED_NETWORK_3GPP2_V01: 
            printf("current selected network is:3GPP2.\n");
            break;

        case NAS_SELECTED_NETWORK_3GPP_V01: 
            printf("current selected network is:3GPP.\n");
            break;

        case NAS_SELECTED_NETWORK_UNKNOWN_V01: 			
        default:
            printf("current selected network is:UNKNOWN.\n");
            break;
    } 
      /**<   Radio interface currently in use. Values: \n
       -0x00 -- RADIO_IF_NO_SVC      -- None (no service) \n
       -0x01 -- RADIO_IF_CDMA_1X     -- 
        cdma2000\textsuperscript{\textregistered} 1X             \n
       -0x02 -- RADIO_IF_CDMA_1XEVDO -- 
        cdma2000\textsuperscript{\textregistered} HRPD (1xEV-DO) \n
       -0x03 -- RADIO_IF_AMPS        -- AMPS \n
       -0x04 -- RADIO_IF_GSM         -- GSM \n
       -0x05 -- RADIO_IF_UMTS        -- UMTS \n
       -0x08 -- RADIO_IF_LTE         -- LTE 
  */   
    for(i=0;i<service_ind->radio_if_len;i++)
    {
		switch (service_ind->radio_if[i])
		{
			case NAS_RADIO_IF_NO_SVC_V01: 
				printf(" radio nas %d status:no service.\n",i);
				break;

			case NAS_RADIO_IF_CDMA_1X_V01: 
				printf(" radio nas %d status:CDMA.\n",i);
				break;
				
			case NAS_RADIO_IF_CDMA_1XEVDO_V01: 
				printf(" radio nas %d status:CDMA 1X.\n",i);
				break;

			case NAS_RADIO_IF_AMPS_V01: 
				printf(" radio nas %d status:AMPS.\n",i);
				break;
				
			case NAS_RADIO_IF_GSM_V01: 
				printf(" radio nas %d status:GSM.\n",i);
				break;	
				
			case NAS_RADIO_IF_UMTS_V01: 
				printf(" radio nas %d status:UMTS.\n",i);
				break;

			case NAS_RADIO_IF_WLAN_V01: 
				printf(" radio nas %d status:WLAN.\n",i);
				break;
				
			case NAS_RADIO_IF_GPS_V01: 
				printf(" radio nas %d status:GPS.\n",i);
				break;	
				
			case NAS_RADIO_IF_LTE_V01: 
				printf(" radio nas %d status:LTE.\n",i);
				break;

			case NAS_RADIO_IF_TDSCDMA_V01: 
				printf(" radio nas %d status:TDSCDMA.\n",i);
				break;
				
			case NAS_RADIO_IF_NO_CHANGE_V01: 		
			default:
				printf(" radio nas %d status: UNKNOWN.\n",i);
				break;
		} 
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
	memset(qmi_simple_ril_voice_info.call_number[0],0,QMI_VOICE_NUMBER_MAX_V02);
    strncpy(req.calling_number,call_number,strlen(call_number));
	strncpy(qmi_simple_ril_voice_info.call_number[0],call_number,strlen(call_number));
    req.call_type_valid = 1;
    req.call_type = CALL_TYPE_VOICE_V02;

    ret = qmi_client_send_msg_sync(qmi_user_handle,
                                  QMI_VOICE_DIAL_CALL_REQ_V02,
                                  &req,
                                  sizeof(req),
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(ret == QMI_NO_ERR)
    {
        printf("do voice call   successful.\n");        
    } else
    {
		printf(" do voice call  failed. error:0x%x\n", ret);
	}
    return ret;
}
/*********************************************************************
 * 
 *  function name:   cb_start_sent_dtmf
 *  nots:  call back of start send dtmf.
 *  author: qiaoyongkang
 *  datetime: 2015.05.22
 * 
 **********************************************************************/
void cb_start_sent_dtmf (qmi_client_type              user_handle,
				   unsigned long                msg_id,
                   void                         *resp_c_struct,
				   int                          resp_c_struct_len,
				   void                         *resp_cb_data,
				   qmi_client_error_type        tranp_err )
{
	voice_start_cont_dtmf_resp_msg_v02*  resp = NULL;
	
    if( tranp_err ==  QMI_NO_ERR )
    {
		resp = (voice_start_cont_dtmf_resp_msg_v02*)resp_c_struct;
        if( QMI_NO_ERR == resp->resp.result)
        {
            if(resp->call_id_valid)
            {				
				printf("start_sent_dtmf reponse received from modem. Call ID: %d\n",resp->call_id);
				return;
			}else
			{
				printf("start_sent_dtmf reponse received from modem\n");
				return;
			}
        }else
        {
			printf("start_sent_dtmf reponse failed, Error code: %d\n", resp->resp.result);
			return;
		}
    }else
    {
		 printf("Transport Error in call back for start_sent_dtmf. Error code: %d\n", tranp_err);
	}
}
/*********************************************************************
 * 
 *  function name:   cb_stop_sent_dtmf
 *  nots:  call back of stop send dtmf.
 *  author: qiaoyongkang
 *  datetime: 2015.05.22
 * 
 **********************************************************************/
void cb_stop_sent_dtmf (qmi_client_type              user_handle,
				   unsigned long                msg_id,
                   void                         *resp_c_struct,
				   int                          resp_c_struct_len,
				   void                         *resp_cb_data,
				   qmi_client_error_type        tranp_err )
{
	voice_stop_cont_dtmf_resp_msg_v02*  resp = NULL;
	
    if( tranp_err ==  QMI_NO_ERR )
    {
		resp = (voice_stop_cont_dtmf_resp_msg_v02*)resp_c_struct;
        if( QMI_NO_ERR == resp->resp.result)
        {
            if(resp->call_id_valid)
            {				
				printf("stop_sent_dtmf reponse received from modem. Call ID: %d\n",resp->call_id);
				return;
			}else
			{
				printf("stop_sent_dtmf reponse received from modem\n");
				return;
			}
        }else
        {
			printf("stop_sent_dtmf reponse failed, Error code: %d\n", resp->resp.result);
			return;
		}
    }else
    {
		 printf("Transport Error in call back for stop_sent_dtmf. Error code: %d\n", tranp_err);
	}
   

}
/*********************************************************************
 * 
 *  function name:   voice_call_start_sent_dtmf
 *  nots:  send a voice call dtmf code to modem side..
 *  author: qiaoyongkang
 *  datetime: 2015.05.22
 * 
 **********************************************************************/
int voice_call_start_sent_dtmf(qmi_client_type qmi_user_handle,uint8_t call_id,uint8_t digit)
{
	voice_start_cont_dtmf_req_msg_v02  req;  //QMI_VOICE_START_CONT_DTMF_REQ_V02
	voice_start_cont_dtmf_resp_msg_v02 rsp;  //QMI_VOICE_START_CONT_DTMF_RESP_V02  
	
	qmi_txn_handle       txn_handle;

    int ret;

    /* memset zero */
    memset(&rsp,0,sizeof(rsp));
    memset(&req,0,sizeof(req));
	
	// from the qxdm log, the call type should set as CALL_TYPE_VOICE
	// log file: volte call send a call. qiao 2015.05.08

    req.cont_dtmf_info.call_id = call_id;
    req.cont_dtmf_info.digit = digit;

    ret = qmi_client_send_msg_async(qmi_user_handle,
                                  QMI_VOICE_START_CONT_DTMF_REQ_V02,
                                  &req,
                                  sizeof(req),
                                  &rsp,
                                  sizeof(rsp),
                                  &cb_start_sent_dtmf,
                                  NULL,
                                  &txn_handle);

    if(ret == QMI_NO_ERR)
    {
        printf("send dtmf code %x  successful.\n",digit);        
    } else
    {
		printf("send dtmf code. error:0x%x\n", ret);
	}
    return ret;
}
/*********************************************************************
 * 
 *  function name:   voice_call_stop_sent_dtmf
 *  nots:  stop to send a voice call dtmf code to modem side..
 *  author: qiaoyongkang
 *  datetime: 2015.05.22
 * 
 **********************************************************************/
int voice_call_stop_sent_dtmf(qmi_client_type qmi_user_handle,uint8_t call_id)
{
	voice_stop_cont_dtmf_req_msg_v02  req;  //QMI_VOICE_STOP_CONT_DTMF_REQ_V02
	voice_stop_cont_dtmf_resp_msg_v02 rsp;  //QMI_VOICE_STOP_CONT_DTMF_RESP_V02  
	
	qmi_txn_handle       txn_handle;

    int ret;

    /* memset zero */
    memset(&rsp,0,sizeof(rsp));
    memset(&req,0,sizeof(req));
	
	// from the qxdm log, the call type should set as CALL_TYPE_VOICE
	// log file: volte call send a call. qiao 2015.05.08

    req.call_id = call_id;

    ret = qmi_client_send_msg_async(qmi_user_handle,
                                  QMI_VOICE_STOP_CONT_DTMF_REQ_V02,
                                  &req,
                                  sizeof(req),
                                  &rsp,
                                  sizeof(rsp),
                                  &cb_stop_sent_dtmf,
                                  NULL,
                                  &txn_handle);

    if(ret == QMI_NO_ERR)
    {
        printf("stop to send dtmf code over call id %x  successful.\n",call_id);        
    } else
    {
		printf("stop to send dtmf code. error:0x%x\n", ret);
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
	int i=0;

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

    if(ret == QMI_NO_ERR)
    {                
		for(i=0;i<MAX_CALL_ID_NUM;i++)
		{
			if(call_id == qmi_simple_ril_voice_info.call_id[i])
			{				
				break;
			}			
		}
		printf("the call id=%d number=%s has ended  successful.\n",call_id,qmi_simple_ril_voice_info.call_number[i]);
		memset(qmi_simple_ril_voice_info.call_number[i],0,QMI_VOICE_NUMBER_MAX_V02);
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

    int ret,i;

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

    if(ret == QMI_NO_ERR)
    {
		for(i=0;i<MAX_CALL_ID_NUM;i++)
		{
			if(call_id == qmi_simple_ril_voice_info.call_id[i])
			{				
				break;
			}			
		}
		printf("the call id=%d number=%s is answered successful.\n",call_id,qmi_simple_ril_voice_info.call_number[i]);              
    } else
    {
		printf("to answer a call failed. error:0x%x\n", ret);
	}
    return ret;	
}
/*********************************************************************
 * 
 *  function name:   qmi_do_voice_manage_calls
 *  nots:  to mange  a voice call hold/resume.
 *  author: qiaoyongkang
 *  datetime: 2015.05.20
 * 
 **********************************************************************/
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

		if(ret == QMI_NO_ERR)
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
	int ret,i; 
	ret = qmi_do_voice_manage_calls(qmi_user_handle,call_id, 0x03); // HOLD_ACTIVE_ACCEPT_WAITING_OR_HELD
	for(i=0;i<MAX_CALL_ID_NUM;i++)
	{
		if(call_id == qmi_simple_ril_voice_info.call_id[i])
		{				
			break;
		}			
	}
	printf("the call id=%d number=%s is hold  successful.\n",call_id,qmi_simple_ril_voice_info.call_number[i]);  
	
	return ret;
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

    if(ret == QMI_NO_ERR)
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
 *  function name:   voice_reg_dtmf_ind
 *  nots: send a request to register a voice dtfm indication,so 
 *   that you can get the dtfm indication from modem side.
 *  author: qiaoyongkang
 *  datetime: 2015.05.26
 *   it is not avaliable now......
 * 
 **********************************************************************/
int voice_reg_dtmf_ind(qmi_client_type qmi_user_handle)
{
	voice_indication_register_req_msg_v02  req;  //QMI_VOICE_INDICATION_REGISTER_REQ_V02
	voice_indication_register_resp_msg_v02 rsp;  //QMI_VOICE_INDICATION_REGISTER_RESP_V02 
    
    int ret;

    /* memset zero */
    memset(&rsp,0,sizeof(rsp));
    memset(&req,0,sizeof(req));
	
	// from the qxdm log, it seems only registered the handover event.
	// log file: volte call send a call. qiao 2015.05.08
    req.reg_dtmf_events_valid = 1;
    req.reg_dtmf_events = 1;

    ret = qmi_client_send_msg_sync(qmi_user_handle,
                                  QMI_VOICE_INDICATION_REGISTER_REQ_V02,
                                  &req,
                                  sizeof(req),
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(ret == QMI_NO_ERR)
    {
        printf("set voice_reg_dtmf_ind successful.\n");        
    } else
    {
		printf("set voice_reg_dtmf_ind failed. error:0x%x\n", ret);
	}
    return ret;
}

/*********************************************************************
 * 
 *  function name:   voice_call_redirect_req
 *  nots: send a request to register a voice dtfm indication,so 
 *   that you can get the dtfm indication from modem side.
 *  author: qiaoyongkang
 *  datetime: 2015.05.27
 * 
 **********************************************************************/
int voice_call_redirect_req(qmi_client_type qmi_user_handle,
		voice_service_enum_v02 voice_service,
		voice_reason_enum_v02 reason,
		char* number)
{
	voice_set_sups_service_req_msg_v02  req;  //QMI_VOICE_SET_SUPS_SERVICE_REQ_V02
	voice_set_sups_service_resp_msg_v02 rsp;  //QMI_VOICE_SET_SUPS_SERVICE_RSEP_V02 
    
    int ret;

    /* memset zero */
    memset(&rsp,0,sizeof(rsp));
    memset(&req,0,sizeof(req));	
	
    req.supplementary_service_info.voice_service = voice_service;
    req.supplementary_service_info.reason = reason;
    
    if( number!=NULL && strlen(number)>0 )
    {
		req.number_valid = 1;
		strcpy(req.number,number);
	}

    ret = qmi_client_send_msg_sync(qmi_user_handle,
                                  QMI_VOICE_SET_SUPS_SERVICE_REQ_V02,
                                  &req,
                                  sizeof(req),
                                  &rsp,
                                  sizeof(rsp),
                                  WDS_MSG_DEFAULT_TIMEOUT);

    if(ret == QMI_NO_ERR)
    {
        printf("set voice_reg_dtmf_ind successful.\n");        
    } else
    {
		printf("set voice_reg_dtmf_ind failed. error:0x%x\n", ret);
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
    char     tmp_call_number[QMI_VOICE_NUMBER_MAX_V02];
    voice_call_end_reason_type_v02  call_end_reason;

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
							memset(&qmi_simple_ril_voice_info,0,sizeof(qmi_simple_ril_voice_info_t));
							break;
						case 1:  //single call,simple output;
							qmi_simple_ril_voice_info.call_state[0] = voice_status_ind.call_info[0].call_state;
							qmi_voice_get_call_state_str(qmi_simple_ril_voice_info.call_state[0]);
							printf("current call status is %s.\n",voice_call_status);
							
							qmi_simple_ril_voice_info.call_type[0] = voice_status_ind.call_info[0].call_type;
							qmi_voice_get_call_type_str(qmi_simple_ril_voice_info.call_type[0]);
							printf("current call type is %s.\n",voice_call_type);
							
						//	print_hex(voice_status_ind.remote_party_number[0],sizeof());
						
							qmi_simple_ril_voice_info.call_direction[0] = voice_status_ind.call_info[0].direction;
							qmi_voice_get_call_type_str(qmi_simple_ril_voice_info.call_direction[0]);
							printf("current call direction is %d.\n",qmi_simple_ril_voice_info.call_direction[0]);	
							
							qmi_simple_ril_voice_info.call_id[0] = voice_status_ind.call_info[0].call_id;
							qmi_voice_get_call_type_str(qmi_simple_ril_voice_info.call_id[0]);	
							qmi_simple_ril_voice_info.current_call_id = qmi_simple_ril_voice_info.call_id[0];											
							printf("current call id is %d.\n",qmi_simple_ril_voice_info.call_id[0]);							
							
							if(CALL_STATE_INCOMING_V02 == voice_status_ind.call_info[0].call_state)
							{
							//	print_voice_all_call_status_ind(&voice_status_ind);								
								if(voice_status_ind.remote_party_number_valid){									
									memset(qmi_simple_ril_voice_info.call_number[0],0,QMI_VOICE_NUMBER_MAX_V02);
									strncpy(qmi_simple_ril_voice_info.call_number[0], voice_status_ind.remote_party_number[0].number,voice_status_ind.remote_party_number[0].number_len);
									printf("the incoming call number is %s.\n",qmi_simple_ril_voice_info.call_number[0]);
								}
							}
							
							if(CALL_STATE_END_V02 == voice_status_ind.call_info[0].call_state)
							{
								if(voice_status_ind.call_end_reason_valid)
								{
									for(i=0;i<voice_status_ind.call_end_reason_len;i++){
										memcpy(&call_end_reason,&(voice_status_ind.call_end_reason[i]),sizeof(voice_call_end_reason_type_v02));
										printf("the call id=%d, end reason is :0x%x, %s...\n",call_end_reason.call_id,call_end_reason.call_end_reason,qmi_voice_get_call_end_reason_str(call_end_reason.call_end_reason));
									}
								}															
							}
							
							break;
						default:   //multiple calls;
							printf("we got %d calls current.\n",voice_status_ind.call_info_len);
							for(i=0;i<voice_status_ind.call_info_len;i++)
							{
								if(i>=MAX_CALL_ID_NUM)
									return;
								
								qmi_simple_ril_voice_info.call_state[i] = voice_status_ind.call_info[i].call_state;
								qmi_voice_get_call_state_str(qmi_simple_ril_voice_info.call_state[i]);
								printf("the %d call status is %s.\n",i,voice_call_status);
								
								qmi_simple_ril_voice_info.call_type[i] = voice_status_ind.call_info[i].call_type;
								qmi_voice_get_call_type_str(qmi_simple_ril_voice_info.call_type[i]);
								printf("the %d call type is %s.\n",i,voice_call_type);
								
							//	print_hex(voice_status_ind.remote_party_number[0],sizeof());
							
								qmi_simple_ril_voice_info.call_direction[i] = voice_status_ind.call_info[i].direction;
								qmi_voice_get_call_type_str(qmi_simple_ril_voice_info.call_direction[i]);
								printf("the %d  call direction is %d.\n",i,qmi_simple_ril_voice_info.call_direction[i]);	
								
								// call id changed, so we need switch it.
								if((qmi_simple_ril_voice_info.call_id[i] != voice_status_ind.call_info[i].call_id) && (i==0))
								{
									memcpy(tmp_call_number,qmi_simple_ril_voice_info.call_number[0],QMI_VOICE_NUMBER_MAX_V02);
									memcpy(qmi_simple_ril_voice_info.call_number[0],qmi_simple_ril_voice_info.call_number[1],QMI_VOICE_NUMBER_MAX_V02);
									memcpy(qmi_simple_ril_voice_info.call_number[1],tmp_call_number,QMI_VOICE_NUMBER_MAX_V02);
								}
								
								qmi_simple_ril_voice_info.call_id[i] = voice_status_ind.call_info[i].call_id;
								qmi_voice_get_call_type_str(qmi_simple_ril_voice_info.call_id[i]);	
								printf("the %d call id is %d.\n",i,qmi_simple_ril_voice_info.call_id[i]);
								
								if(CALL_STATE_CONVERSATION_V02 == qmi_simple_ril_voice_info.call_state[i])
									qmi_simple_ril_voice_info.current_call_id = qmi_simple_ril_voice_info.call_id[i];																		
													
								if (CALL_STATE_WAITING_V02 == qmi_simple_ril_voice_info.call_state[i])
								{									
									printf("a new call %d is incoming and waiting,the call id=%d.\n",i,qmi_simple_ril_voice_info.call_id[i]);
									
									memset(qmi_simple_ril_voice_info.call_number[i],0,QMI_VOICE_NUMBER_MAX_V02);
									strncpy(qmi_simple_ril_voice_info.call_number[i], voice_status_ind.remote_party_number[i].number,voice_status_ind.remote_party_number[i].number_len);
									printf("the incoming call number is %s.\n",qmi_simple_ril_voice_info.call_number[i]);
								}
								
								if(CALL_STATE_END_V02 == qmi_simple_ril_voice_info.call_state[i] )
									qmi_simple_ril_voice_info.current_call_id = qmi_simple_ril_voice_info.call_id[(i+1)%2];																	
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
                case QMI_VOICE_DTMF_IND_V02:
                {
					printf("get a voice dtmf ind from network.\n");
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
/*********************************************************************
 * 
 *  function name:   wm_reg_sig_ind
 *  nots: set nas service indication registion request. 
 *   status
 *  author: qiaoyongkang
 *  datetime: 2015.05.26
 * 
 **********************************************************************/
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
    req.req_serving_system = 1;
    req.req_serving_system_valid = 1;
    
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
/*********************************************************************
 * 
 *  function name:   nas_unsol_ind
 *  nots:  nas service indication 
 *   status
 *  author: qiaoyongkang
 *  datetime: 2015.05.26
 * 
 **********************************************************************/
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
            printf("nas msg id 0x%x\n", msg_id);
            switch(msg_id)
            {
                case QMI_NAS_SERVING_SYSTEM_IND_MSG_V01:
                {
                    memcpy(&service_ind,decoded_payload,decoded_payload_len);
                    printf("nas service ind\n");                    
                    qmi_nas_decode_service_str(&(service_ind.serving_system));
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
  //  uim_service_obj = uim_get_service_object_v01();

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

	if(-1 != access(qmidev[0], F_OK))
	{
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
	}
#if 0	
	if(-1 != access(qmidev[0], F_OK))
	{
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
		
		rc = wm_reg_sig_ind(nas_user_handle);
		if (rc != QMI_NO_ERR ) {
			 printf("set nas status indication failed code=:%d\n",rc);
			 return 0;
		}
	}
#endif
	if(-1 != access(qmidev[0], F_OK))
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
		
		rc = voice_reg_dtmf_ind(voice_user_handle0);
		if (rc != QMI_NO_ERR ) {
			 printf("Error: register voice call dtmf indication failed...Error Code:%d\n",rc);        
		} 
		printf("access to %s ok, and initilize set indication request ok.\n",qmidev[0]);
    }else
    {
		printf(".....access %s failed.\n",qmidev[0]);
	}

 	if(-1 != access(qmidev[1], F_OK))
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
		

		 
		printf("access to %s ok, and initilize set indication request ok.\n",qmidev[1]);
		
     }else
    {
		printf(".....access %s failed.\n",qmidev[1]);
	}   
  

    while(run)
    {
        printf("1.dail a number 18609284351 from handle0.\n");
        printf("11.dail a number 18609284351 from handle1.\n");
        printf("2.answer a call from handle0.\n");
        printf("22.answer a call from handle1..\n");
        printf("3.reject or end a call from handle0\n");
        printf("33.reject or end a call from handle1.\n");
        printf("4. hold the current call from handle0\n");
        printf("5. send a dtmf code (1) from handle0.\n");
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
                rc = voice_dail_call(voice_user_handle0,"18609284351");
                break;
            }
            case 2:
            {
                rc = voice_call_answer(voice_user_handle0,qmi_simple_ril_voice_info.current_call_id);
                break;
            }
            case 3:
            {
                rc = voice_call_end_request(voice_user_handle0,qmi_simple_ril_voice_info.current_call_id);
                break;
            }
            case 4:
            {
				if(qmi_simple_ril_voice_info.call_id[1]!=0){
					//must support multi call is doing multi call
					rc = voice_call_hold(voice_user_handle0,qmi_simple_ril_voice_info.current_call_id);
				}else
				{
					printf("failed.must support multi call is doing multi call.\n");
				}
                break;
            }
            case 5:
            {
				u_int8_t  number = '1';
				voice_call_start_sent_dtmf(voice_user_handle0,qmi_simple_ril_voice_info.current_call_id,number);
				usleep(300*1000);
				voice_call_stop_sent_dtmf(voice_user_handle0,qmi_simple_ril_voice_info.current_call_id);
                break;
            }
            case 11:
            {
                rc = voice_dail_call(voice_user_handle1,"18609284351");
                break;
            }
            case 22:
            {
                rc = voice_call_answer(voice_user_handle1,qmi_simple_ril_voice_info.current_call_id);
                break;
            }
            case 33:
            {
                rc = voice_call_end_request(voice_user_handle1,qmi_simple_ril_voice_info.current_call_id);
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
		if(-1 != access(qmidev[0], F_OK))
		{
			rc = qmi_client_release(dms_user_handle);
			if (rc < 0 ) {
				printf("Release not successful \n");
			}else
			{
				printf("Qmi client release successful \n");
			}
		}
#if 0		
        /* Release the client */
       if(-1 != access(qmidev[0], F_OK))
		{ 
			rc = qmi_client_release(voice_user_handle0);
			if (rc < 0 ) {
				printf("Release not successful \n");
			}else
			{
				printf("Qmi client release successful \n");
			}
		}
        if(-1 != access(qmidev[1], F_OK))
		{ 
			rc = qmi_client_release(voice_user_handle1);
			if (rc < 0 ) {
				printf("Release not successful \n");
			}else
			{
				printf("Qmi client release successful \n");
			}
		}
#endif
        printf("Qmi release done.........\n");

    }

   
   return 0;
}
