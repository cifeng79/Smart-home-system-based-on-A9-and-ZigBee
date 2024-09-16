#include "data_global.h"
#include "linuxuart.h"
unsigned char cmd_led;
unsigned char cmd_buzzer;
unsigned char cmd_seg;
unsigned char cmd_fan;


extern char recive_phone[12] ;
extern char center_phone[12] ;


struct msg msgbuf;

void *pthread_client_request(void *arg)
{
	int msgid;
	key_t msg_key;

	/* 申请Key值 */
	if((msg_key = ftok("/tmp",'g')) < 0){
		perror("ftok failed .\n");
		exit(-1);
	}
	/* 创建消息队列 */
	msgid = msgget(msg_key,IPC_CREAT|IPC_EXCL|0666);
	if(msgid == -1)	{
		if(errno == EEXIST){
			msgid = msgget(msg_key,0777);
			printf("消息队列存在\n");
		}else{
			perror("fail to msgget");
			exit(1);
		}
	}
	
	printf("pthread_client_request\n");
	
	while(1){
		pthread_mutex_lock(&mutex_client_request);
		bzero(&msgbuf,sizeof(struct msg));
		printf("wait form client request...\n");
		int ret = msgrcv (msgid, &msgbuf, sizeof (msgbuf) - sizeof (long), 1L, 0);
		if (ret == -1) {
			printf("msgrcv failed: %s\n", strerror(errno));
		} else {
			printf("Received message: %ldL\n", msgbuf.msgtype);
			printf ("text[0] = %#x\n", msgbuf.text[0]);
		}
		
		switch(msgbuf.msgtype){
			case 1L:
				pthread_mutex_lock(&mutex_led);
				cmd_led = msgbuf.text[0];
				pthread_mutex_unlock(&mutex_led);
				pthread_cond_signal(&cond_led);
				break;

			case 2L:
				pthread_mutex_lock(&mutex_buzzer);
				cmd_buzzer = msgbuf.text[0];
				pthread_mutex_unlock(&mutex_buzzer);
				pthread_cond_signal(&cond_buzzer);
				break;
			case 3L:
				pthread_mutex_lock(&mutex_seg);
				cmd_seg = msgbuf.text[0];
				pthread_mutex_unlock(&mutex_seg);
				pthread_cond_signal(&cond_seg);
				break;
			case 4L://风扇控制
				pthread_mutex_lock(&mutex_fan);
				printf("hello fan\n");
				cmd_fan = msgbuf.text[0];
#if 0
				//get data from zigbee
				read(fan_fd,&buf,sizeof(buf));
				printf("sizeof(buf) = %d.\n",sizeof(buf));
				printf(">>>>>>%s\n",buf);
				sleep(1);
#endif
				pthread_cond_signal(&cond_fan);
				pthread_mutex_unlock(&mutex_fan);
				break;
			
			case 5L://温湿度最佳值设置
				printf("set env data\n");
				printf("temMAX: %d\n",*((int *)&msgbuf.text[1]));
				printf("temMIN: %d\n",*((int *)&msgbuf.text[5]));
				printf("humMAX: %d\n",*((int *)&msgbuf.text[9]));
				printf("humMAX: %d\n",*((int *)&msgbuf.text[13]));
				printf("illMAX: %d\n",*((int *)&msgbuf.text[17]));
				printf("illMAX: %d\n",*((int *)&msgbuf.text[21]));

				break;
			/*拓展接口*/
			case 6L:
				break;
			case 7L:
				break;
			case 8L:
				break;
			case 9L:
				break;
			case 10L://GPRS控制
				{
					int i = 0 , j = 0 ;
					for(i = 0 ; i < 11; i++){
						recive_phone[i] = msgbuf.text[i]; 	
					}
					recive_phone[i] = '\0';
					printf("recive:%s\n",recive_phone);
					for(j = 0 ;msgbuf.text[i] != '\0' && j < 12; i++, j++)
					{
						center_phone[j] =  msgbuf.text[i];
					}
					center_phone[j] = '\0';
					printf("center:%s\n",center_phone);
#if 0
					pthread_mutex_lock (&mutex_slinklist);
					sqlite_InsertLinknode (ENV_UPDATE, all_info_RT, sto_no, 0);//0,0鍒嗗埆鏄粨搴撳彿鍜岃揣鐗╃绫诲彿
					pthread_mutex_unlock (&mutex_slinklist);
					pthread_cond_signal (&cond_sqlite);
#endif 
				}
				break;

			default:
				break;
		}
		pthread_mutex_unlock(&mutex_client_request);
	}
	return NULL;
}


