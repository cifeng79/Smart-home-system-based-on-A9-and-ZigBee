#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>
#include <sys/sem.h>
#include <unistd.h>
#include "sem.h"
#include "cgic.h"
#include "data_global.h"

#define N 100

#define MONITOR_NUM 1

char status[2][6] = {"Close", "Open"};
char fan_status[4][6] = {"Close", "One", "Two", "Three"};

int cgiMain()
{
	struct shm_addr *shm_buf;
	
	key_t shm_key,
		  sem_mutex_key,
		  sem_empty_key,
		  sem_full_key;
	int shmid,
		sem_mutex_id,
		sem_empty_id,
		sem_full_id;
	/* 信号量Key */
	if((sem_mutex_key = ftok("/tmp",'v')) <0)
	{
		perror("semkey ftok failed");
		exit(1);
	}	
	sem_empty_key = sem_mutex_key + 1;
	sem_full_key = sem_mutex_key + 2;
	/* 创建信号量 */
	sem_mutex_id = semget(sem_mutex_key, 1, 0666);
	sem_empty_id = semget(sem_empty_key, 1, 0666);
	sem_full_id = semget(sem_full_key, 1, 0666);
	if(sem_mutex_id == -1 | sem_empty_id == -1 | sem_full_id == -1)
	{
		perror("shmget failed");
		exit(1);
	}
	/*申请共享内存*/
	if((shm_key = ftok("/tmp",'a')) < 0)
	{
		perror("shm ftok failed");
		exit(1);
	}

	if((shmid = shmget(shm_key, N, 0666 )) == -1)
	{
		perror("shmget");
		exit(1);
	}
	/* 共享内存映射 */
	if((shm_buf = (struct shm_addr*)shmat(shmid, NULL, 0)) == (void*)-1 )
	{
		perror("shmat");
		exit(1);
	}

	sem_p(sem_full_id, 0);
	sem_p(sem_mutex_id, 0);
	cgiHeaderContentType("text/html");
	fprintf(cgiOut, "<head><meta http-equiv=\"refresh\" content=\"1\"><style><!--body{line-height:50%}--></style> </head>");
	fprintf(cgiOut, "<HTML>\n");
	fprintf(cgiOut, "<BODY bgcolor=\"#666666\">\n");
	//fprintf(cgiOut, "<h1><font color=\"#FF0000\">HOME_ID #%d:</font></H2>\n ", shm_buf->shm_status);
	if (shm_buf->shm_status == 1)
	{
		fprintf(cgiOut, "<script>function show(){var date =new Date(); var now = \"\"; now = date.getFullYear()+\"年\"; now = now + (date.getMonth()+1)+\"月\"; now = now + date.getDate()+\"日\"; now = now + date.getHours()+\"时\"; now = now + date.getMinutes()+\"分\";now = now + date.getSeconds()+\"秒\"; document.getElementById(\"nowDiv\").innerHTML = now; setTimeout(\"show()\",1000);} </script> \n ");
		fprintf(cgiOut, "<h2><font face=\"Broadway\"><font color=\"#00FAF0\">Home1 Real-time Environment Info:</font></font></H2>\n ");
		fprintf(cgiOut, "<h2 align=center><font color=\"#cc0033\"><body onload=\"show()\"> <div id=\"nowDiv\"></div></font></h2> \n "); 	
		fprintf(cgiOut, "<h4>ZIGBEE数据显示部分</H4>\n ");
		fprintf(cgiOut, "<h4>Temperature:\t%0.2f</H4>\n ", shm_buf->sm_all_env_info.monitor_no[shm_buf->shm_status].zigbee_info.temperature );
		fprintf(cgiOut, "<h4>Humidity:\t%0.2f</H4>\n ", shm_buf->sm_all_env_info.monitor_no[shm_buf->shm_status].zigbee_info.humidity);

		fprintf(cgiOut, "<h4>A9数据显示部分</H4>\n ");
		fprintf(cgiOut, "<h4>Adc:\t%0.2f</H4>\n ", shm_buf->sm_all_env_info.monitor_no[shm_buf->shm_status].a9_info.adc);
		fprintf(cgiOut, "<h4>GYROX:\t%d</H4>\n ", shm_buf->sm_all_env_info.monitor_no[shm_buf->shm_status].a9_info.gyrox);
		fprintf(cgiOut, "<h4>GYROY:\t%d</H4>\n ", shm_buf->sm_all_env_info.monitor_no[shm_buf->shm_status].a9_info.gyroy);
		fprintf(cgiOut, "<h4>GYROZ:\t%d</H4>\n ", shm_buf->sm_all_env_info.monitor_no[shm_buf->shm_status].a9_info.gyroz);
		fprintf(cgiOut, "<h4>AACX :\t%d</H4>\n ", shm_buf->sm_all_env_info.monitor_no[shm_buf->shm_status].a9_info.aacx);
		fprintf(cgiOut, "<h4>AACY :\t%d</H4>\n ", shm_buf->sm_all_env_info.monitor_no[shm_buf->shm_status].a9_info.aacy);
		fprintf(cgiOut, "<h4>AACZ :\t%d</H4>\n ", shm_buf->sm_all_env_info.monitor_no[shm_buf->shm_status].a9_info.aacz);
		fprintf(cgiOut, "<h4>A9-RESERVED[0]:\t%d</H4>\n ", shm_buf->sm_all_env_info.monitor_no[shm_buf->shm_status].a9_info.reserved[0]);
		fprintf(cgiOut, "<h4>A9-RESERVED[1]:\t%d</H4>\n ", shm_buf->sm_all_env_info.monitor_no[shm_buf->shm_status].a9_info.reserved[1]);

		fprintf(cgiOut, "<h4>STM32数据显示部分</H4>\n ");
		fprintf(cgiOut, "<h4>......</H4>\n ");
	}
	else
	{
		fprintf(cgiOut, "<h2><font face=\"Broadway\"><font color=\"#FFFAF0\">Close!</font></font></H2>\n ");	
	}
	//	fprintf(cgiOut, "<h3>:</H3>\n ");	
	fprintf(cgiOut, "</BODY></HTML>\n");	
	sem_v(sem_mutex_id, 0);
	sem_v(sem_empty_id, 0);
	return 0;
}


