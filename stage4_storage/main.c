
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>

#include "data_global.h"


void release_pthread_resource(int signo);


pthread_t id_client_request,
		  id_refresh,
		  id_seg,
		  id_transfer,
		  id_sms,
		  id_buzzer,
		  id_led,
		  id_fan;


extern int msgid;
extern int shmid;
extern int semid;


int main(int argc, const char *argv[])
{

#if 0
	//初始化锁
	pthread_mutex_init(&mutex_client_request,NULL);
	pthread_mutex_init(&mutex_refresh,NULL);
	pthread_mutex_init(&mutex_fan,NULL);
	pthread_mutex_init(&mutex_transfer,NULL);
	pthread_mutex_init(&mutex_sms,NULL);
	pthread_mutex_init(&mutex_buzzer,NULL);
	pthread_mutex_init(&mutex_led,NULL);
	

	//初始化各种条件变量
	pthread_cond_init(&cond_client_request,NULL);
	pthread_cond_init(&cond_refresh,NULL);
	pthread_cond_init(&cond_fan,NULL);
	pthread_cond_init(&cond_transfer,NULL);
	pthread_cond_init(&cond_sms,NULL);
	pthread_cond_init(&cond_buzzer,NULL);
	pthread_cond_init(&cond_led,NULL);
#endif

	//等待接受信号，信号处理函数
	signal (SIGINT, release_pthread_resource);

	//创建线程
	pthread_create(&id_client_request,  NULL,pthread_client_request,NULL);   
	pthread_create(&id_refresh,		NULL,pthread_refresh,NULL);  			
	pthread_create(&id_seg,		NULL,pthread_seg,NULL);  			
	pthread_create(&id_transfer,	NULL,pthread_transfer,NULL); 
	pthread_create(&id_sms,			NULL,pthread_sms,NULL);  	
	pthread_create(&id_buzzer,		NULL,pthread_buzzer,NULL);	 
	pthread_create(&id_led,			NULL,pthread_led,NULL);  	
	pthread_create(&id_fan,         NULL, pthread_fan, NULL);
	//等待线程退出
	pthread_join(id_client_request,NULL);     printf ("pthread1\n");
	pthread_join(id_refresh,NULL);          	printf ("pthread2\n");
	pthread_join(id_seg,NULL);			printf ("pthread3\n");
	pthread_join(id_transfer,NULL);			printf ("pthread4\n");
	pthread_join(id_sms,NULL);				printf ("pthread5\n");
	pthread_join(id_buzzer,NULL);			printf ("pthread6\n");
	pthread_join(id_led,NULL);				printf ("pthread7\n");
	pthread_join(id_fan, NULL);             printf ("pthread8\n");

	return 0;
}


void release_pthread_resource(int signo)
{
	//释放锁和条件变量
	pthread_mutex_destroy (&mutex_client_request);   
	pthread_mutex_destroy (&mutex_refresh);   
	pthread_mutex_destroy (&mutex_fan);    
	pthread_mutex_destroy (&mutex_transfer);   
	pthread_mutex_destroy (&mutex_sms);   
	pthread_mutex_destroy (&mutex_buzzer);   
	pthread_mutex_destroy (&mutex_led);
	pthread_mutex_destroy (&mutex_fan);

 	pthread_cond_destroy (&cond_client_request);
 	pthread_cond_destroy (&cond_refresh);
 	pthread_cond_destroy (&cond_fan);
 	pthread_cond_destroy (&cond_transfer);
 	pthread_cond_destroy (&cond_sms);
 	pthread_cond_destroy (&cond_buzzer);
 	pthread_cond_destroy (&cond_led);
	pthread_cond_destroy (&cond_fan);

	//分离线程的资源
	 pthread_detach(id_client_request);
	 pthread_detach(id_refresh);
	 pthread_detach(id_seg);
	 pthread_detach(id_transfer);
	 pthread_detach(id_sms);
	 pthread_detach(id_buzzer);
	 pthread_detach(id_led);
	 pthread_detach(id_fan);

	 printf("all pthread is detached\n");
	 
	 msgctl (msgid, IPC_RMID, NULL);
	 shmctl (shmid, IPC_RMID, NULL);
	 semctl (semid, 1, IPC_RMID, NULL);

	 exit(0);
}



