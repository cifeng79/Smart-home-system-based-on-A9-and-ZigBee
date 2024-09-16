#include "data_global.h"
#include "pthread_buzzer.h"

void *pthread_buzzer(void *arg)
{
	printf("pthread_buzzer\n");
	
	int buzzer_fd = open(BEEPER_DEV,O_RDWR | O_NONBLOCK);
	if(buzzer_fd == -1)
	{
		perror("open buzzer failed\n");
	}
	printf("buzzer_fd:%d.\n",buzzer_fd);

	while(1) {
		pthread_mutex_lock(&mutex_buzzer);
		pthread_cond_wait(&cond_buzzer, &mutex_buzzer);
		
		printf("Entering function %s at line %d\n",__func__, __LINE__);	
		if(cmd_buzzer == 0x50) {
			ioctl(buzzer_fd, BEEP_OFF);
			printf("蜂鸣器已关闭\n");
		}
		
		if(cmd_buzzer == 0x51) {
			beep_desc_t beeper;
			ioctl(buzzer_fd, BEEP_ON);
			printf("蜂鸣器已打开\n");
		}

		if(cmd_buzzer == 0x52) {
			printf("暂不支持该功能\n");
		}
		
		if(cmd_buzzer == 0x53) {
			printf("暂不支持该功能\n");
		}

		pthread_mutex_unlock(&mutex_buzzer);
	}

}

