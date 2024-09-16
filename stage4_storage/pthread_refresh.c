#include "data_global.h"
#include "sem.h"

#define N 1024  //for share memory

extern struct env_info_client_addr sm_all_env_info;

void *pthread_refresh(void *arg)
{
	int home_id = 1;
	struct shm_addr *shm_buf;
	
	key_t shm_key,
		  sem_mutex_key,
		  sem_empty_key,
		  sem_full_key;
	int shmid,
		sem_mutex_id,
		sem_empty_id,
		sem_full_id;
	
	/* 信号量 key*/
	if((sem_mutex_key = ftok("/tmp",'v')) < 0)
	{
		perror("semkey ftok failed");
		exit(1);
	}
	sem_empty_key = sem_mutex_key + 1;
	sem_full_key = sem_mutex_key + 2;
	/* 创建信号量 */
	sem_mutex_id = semget(sem_mutex_key, 1, IPC_CREAT|IPC_EXCL|0666);
	if(sem_mutex_id == -1) {
		if(errno == EEXIST) {
			sem_mutex_id = semget(sem_mutex_key, 1, 0666);
		} else {
			perror("failed to sem_mutex_id");
			exit(0);
		}
	}	
	sem_empty_id = semget(sem_empty_key, 1, IPC_CREAT|IPC_EXCL|0666);
	if(sem_empty_id == -1) {
		if(errno == EEXIST) {
			sem_empty_id = semget(sem_empty_key, 1, 0666);
		} else {
			perror("failed to sem_empty_id");
			exit(0);
		}
	}
	sem_full_id = semget(sem_full_key, 1, IPC_CREAT|IPC_EXCL|0666);
	if(sem_full_id == -1) {
		if(errno == EEXIST) {
			sem_full_id = semget(sem_full_key, 1, 0666);
		} else {
			perror("failed to semget");
			exit(0);
		}
	}

	/*初始化信号量*/
	init_sem(sem_mutex_id, 0, 1);
	init_sem(sem_empty_id, 0, 1);//可写
	init_sem(sem_full_id, 0, 0);//可读
	
	/* 共享内存 */
	if((shm_key = ftok("/tmp",'a')) < 0){
		perror("shm ftok failed");
		exit(1);
	}

	shmid = shmget(shm_key, N, IPC_CREAT | 0666);
	if(shmid < 0) {
		perror("shmget");
		exit(1);
	}
	
	if((shm_buf = (struct shm_addr *)shmat(shmid,NULL,0)) == (void *)-1)
	{
		perror("fail to shmat");
		exit(1);
	}
	bzero (shm_buf, sizeof (struct shm_addr));
	
	printf("pthread_refresh ......>>>>>>>\n");

	while(1) {
		printf("a9_info.adc  : %f.\n",sm_all_env_info.monitor_no[home_id].a9_info.adc);
		printf("a9_info.gyrox: %d.\n",sm_all_env_info.monitor_no[home_id].a9_info.gyrox);
		printf("a9_info.gyroy: %d.\n",sm_all_env_info.monitor_no[home_id].a9_info.gyroy);
		printf("a9_info.gyroz: %d.\n",sm_all_env_info.monitor_no[home_id].a9_info.gyroz);
		printf("a9_info.aacx : %d.\n",sm_all_env_info.monitor_no[home_id].a9_info.aacx );
		printf("a9_info.aacy : %d.\n",sm_all_env_info.monitor_no[home_id].a9_info.aacy );
		printf("a9_info.aacz : %d.\n",sm_all_env_info.monitor_no[home_id].a9_info.aacz );
		sem_p(sem_empty_id, 0);
		sem_p(sem_mutex_id, 0);
		shm_buf->shm_status = 1;
		shm_buf->sm_all_env_info.monitor_no[home_id] = sm_all_env_info.monitor_no[home_id];
		sem_v(sem_mutex_id, 0);
		sem_v(sem_full_id, 0);
		pthread_cond_signal(&cond_transfer);
	}
}

