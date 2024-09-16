
#include "data_global.h"

struct env_info_client_addr sm_all_env_info;

/*********************************************************
   data_global.c :
        全局的id和key信息声明
        全局的消息队列发送函数声明
 *********************************************************/
pthread_mutex_t mutex_client_request = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_refresh = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_fan = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_transfer = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_sms = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_buzzer = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_led = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_seg = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t cond_client_request = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_refresh = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_fan = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_transfer = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_sms = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_buzzer = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_led = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_seg = PTHREAD_COND_INITIALIZER;

int msgid;
int shmid;
int semid;

key_t key;
key_t shm_key;
key_t sem_key;

unsigned char cmd_led = 0;
unsigned char cmd_seg = 0;
unsigned char cmd_buzzer = 0;
unsigned char cmd_fan = 0;



char recive_phone[12] =  {0};
char center_phone[12] =  {0};

int send_msg_queue(long type,unsigned char text)
{
	struct msg msgbuf;
	msgbuf.type = 1L;
	msgbuf.msgtype = type;
	msgbuf.text[0] = text;

	if(msgsnd(msgid,&msgbuf,sizeof(msgbuf) - sizeof(long),0) == -1){
		perror("fail to msgsnd type2");
		exit(1);
	}

	return 0;
}








