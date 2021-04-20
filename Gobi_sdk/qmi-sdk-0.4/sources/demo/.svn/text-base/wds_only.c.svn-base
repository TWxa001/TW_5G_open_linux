#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "qmi_client.h"
#include "qmi_cci_target_ext.h"
#include "qmi_idl_lib.h"
#include "wireless_data_service_v01.h"
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define WDS_MSG_DEFAULT_TIMEOUT 20000

/********************************************************************
 *
 *         Name:  trim_str
 *  Description:  trim the string with a-z,0-9 A-Z,=,.,/ only
 *        Input:  in_out_str:input string
 *       Output:   in_out_str:trimed string
 *       Return:  0/-1
 *        Notes:  
 ********************************************************************/
static void trim_str(char* in_out_str)
{
    char buff[128]="";
    char* ptr=in_out_str;
    int i=0;
    while(*ptr)
    {    
        if((*ptr>='0'&&*ptr<='9')||(*ptr>='a'&&*ptr<='z')||(*ptr>='A'&&*ptr<='Z')||(*ptr=='_')||(*ptr=='=')||(*ptr=='.')||(*ptr=='/')||(*ptr==',')||(*ptr=='@'))
            buff[i++]=*ptr;
        ptr++;        
    }
    strcpy(in_out_str,buff);
}

/********************************************************************
 *
 *         Name:  read_ini_config
 *  Description:  read from a ini file the distinct zone distinct 
 *                configure name value
 *        Input:  file_name,ini file name,its example:
 *                ....[conf_zone]
 *                .... conf_name = conf_value
 *       Output:  conf_value
 *       Return:  0/-1
 *        Notes:  
 ********************************************************************/
int read_ini_config(char* file_name,char* conf_zone,char* conf_name, char* conf_value)
{
    FILE* ini_file=fopen(file_name,"r");
    if(ini_file==NULL)
    {
        return -1;
    }
    
    char buff[256];
    int  tag=0;
    while(fgets(buff,sizeof(buff),ini_file))//find zone 
    {
        if(strstr(buff,"[")&&strstr(buff,conf_zone))
        {
            tag=1;
            break;
        }
    }
    if(tag==0)//can not found
    {
        fclose(ini_file);
        return -1;
    }
    while((fgets(buff,sizeof(buff),ini_file)))
    {
        if(strstr(buff,conf_name))
        {
            tag=1;
            break;
        }
    }
    if(tag==0)//can not found
    {
        fclose(ini_file);
        return -1;
    }
    char* ptr = strstr(buff,"=");
    ptr++;
    strcpy(conf_value,ptr);    
    trim_str(conf_value);//remove dirty ascii code
    
    fclose(ini_file);
    return 0;
}

/*qmi message library handle*/
qmi_idl_service_object_type wds_service_obj;
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
    wds_pkt_srvc_status_ind_msg_v01 service_ind;

    /*Get C struct  length*/
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
        /*Decode ind message to C struct*/
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
            printf("handle wds msg id %u\n", msg_id);
            /*Handle different msg by  msg_id*/
            switch(msg_id)
            {
                case QMI_WDS_PKT_SRVC_STATUS_IND_V01:
                    memcpy(&service_ind, decoded_payload, decoded_payload_len);
                    printf("wds pkt srvc status ind:\n");
                    printf("status:0x%x\n",service_ind.status);
                    if(service_ind.call_end_reason_valid)
                        printf("call_end_reason:%d\n",service_ind.call_end_reason);
                    if(service_ind.verbose_call_end_reason_valid)
                        printf("verbose_call_end_reason:0x%x\n",service_ind.verbose_call_end_reason);
                    if(service_ind.ip_family_valid)
                        printf("ip_family:%d\n",service_ind.ip_family);
                    if(service_ind.tech_name_valid)
                        printf("tech_name:%d\n",service_ind.tech_name);
                    break;
                default:
                    printf("wds recv msg_id = %d\n", msg_id);
                    break;
            }
        }
        free(decoded_payload);
    }
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
    char buf[60];

    /* memset the response structure to zero */
    memset(&req,0,sizeof(req));
    memset(&rsp,0,sizeof(rsp));
    memset(buf, 0, sizeof(buf));

    read_ini_config("profile.ini", "profile", "apn", buf);
    if(buf[0] != 0) {
        req.apn_name_valid = 1;
        memcpy(req.apn_name, buf, strlen(buf));
    }
    else {
        printf("No apn\n");
    }
    memset(buf, 0, sizeof(buf));
    read_ini_config("profile.ini", "profile", "usrname", buf);
    if(buf[0] != 0) {
        req.username_valid = 1;
        memcpy(req.username, buf, strlen(buf));
    }
    memset(buf, 0, sizeof(buf));
    read_ini_config("profile.ini", "profile", "pwd", buf);
    if(buf[0] != 0) {
        req.password_valid = 1;
        memcpy(req.password, buf, strlen(buf));
    }
    printf("apn:%s\nuser name:%s\npassword:%s\n",req.apn_name, req.username, req.password);


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
        printf("pkt_data_handle %08x\n", rsp.pkt_data_handle);
        if(rsp.resp.result == QMI_RESULT_SUCCESS_V01)
        {
            pkt_data_handle = rsp.pkt_data_handle; //save handle for stop network
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
    req.pkt_data_handle = pkt_data_handle; //use pkt handle we get from dial resp
    req.disable_autoconnect_valid = 1;
    req.disable_autoconnect = 1;

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
    qmi_client_error_type rc;   //get return value
    qmi_client_type wds_user_handle;  //WDS client handler
    int option;
    qmi_cci_os_signal_type os_params; //os_params for QMI lib
    uint32_t num_services, num_entries=0;
    qmi_service_info info[10]; //save service address

    /*Get WDS service object*/
    wds_service_obj = wds_get_service_object_v01(); 

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
  /* The server has come up, store the information in info variable */
  rc = qmi_client_get_service_list( wds_service_obj, info, &num_entries, &num_services);
  printf("PING: qmi_client_get_service_list() returned %d num_entries = %d num_services = %d\n", rc, num_entries, num_services);

   /* Initialize a connection to first QMI control port */
    rc = qmi_client_init(&info[0],
                        wds_service_obj,
                        wds_unsol_ind,
                        NULL,
                        NULL,
                        &wds_user_handle
                        );

    if (rc != QMI_NO_ERR ) {
         printf("Error: connection not Initialized...Error Code:%d\n",rc);
         return 0;
    }

    while(1) {
        scanf("%d", &option);
        if(option == 1) {
            rc = dial_v4(wds_user_handle);
            printf("dial_v4,rc = %d\n", rc);
        } else if (option == 2) {
            rc = stop_v4(wds_user_handle);
            printf("stop_v4,rc = %d\n", rc);
        } else {
            break;
        }
    }

    {
        rc = qmi_client_release(wds_user_handle);
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
