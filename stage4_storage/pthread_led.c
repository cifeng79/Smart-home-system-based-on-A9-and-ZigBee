#include "data_global.h"
#include "pthread_led.h"

extern unsigned char cmd_seg; 

int fswaterled_control(int led_fd, int times);
int fsled_control(int led_fd,led_desc_t *led);
int fsled_close_all(int led_fd);
int fsled_open_all(int led_fd);

//A9LED模块线程
void *pthread_led(void *arg)
{
	printf("pthread_led\n");
	
	int led_fd = -1;
	led_fd = open(LED_DEV, O_RDWR);
	if(led_fd == -1){
		printf("open failed.\n");
	}
	printf("led_fd:%d.\n",led_fd);

	while(1){
		pthread_mutex_lock(&mutex_led);
		pthread_cond_wait(&cond_led,&mutex_led);
		printf("Entering function %s at line %d\n",__func__, __LINE__);	
		/* 关闭所有灯光 */
		if(cmd_led == 0x40) {
			fsled_close_all(led_fd);
			printf("所有灯光已关闭\n");
		}
		/* 开启所有灯光 */
		if(cmd_led == 0x41) {
			fsled_open_all(led_fd);
			printf("所有灯光已打开\n");
		}

		/* 开启LED2 */
		if(cmd_led == 0x42) {
			led_desc_t led;
			led.which = 2;
			ioctl(led_fd, FSLEDON, &led);
			printf("LED2 已经打开\n");
		}
		/* 开启LED3 */
		if(cmd_led == 0x43) {
			
			led_desc_t led;
			led.which = 3;
			ioctl(led_fd, FSLEDON, &led);
			printf("LED3 已经打开\n");
		}
		/* 开启LED4 */
		if(cmd_led == 0x44) {
			led_desc_t led;
			led.which = 4;
			ioctl(led_fd, FSLEDON, &led);
			printf("LED4 已经打开\n");
		}
		/* 开启LED5 */
		if(cmd_led == 0x45) {
			led_desc_t led;
			led.which = 5;
			ioctl(led_fd, FSLEDON, &led);
			printf("LED5 已经打开\n");
		}
		/* 开启流水灯 */
		if(cmd_led == 0x46) {
			fswaterled_control(led_fd,3);
		}
		
		pthread_mutex_unlock(&mutex_led);
	}
}

int fsled_close_all(int led_fd)
{
	int i = 0;
	led_desc_t led;	

	for(i = 2;i < 6;i ++){
		led.which = i;
		ioctl(led_fd,FSLEDOFF,&led);
		usleep(50000);
	}

	return 0;
}

int fsled_open_all(int led_fd)
{
	int i = 0;
	led_desc_t led;
	
	for(i = 2; i < 6; i++) {
		led.which = i;
		ioctl(led_fd, FSLEDON, &led);
		usleep(50000);
	}
}

int fswaterled_control(int led_fd, int times)
{
	int i = 0,j = 0;
	led_desc_t led;	

	for(j = 0;j < times;j ++){
		for(i = 2;i < 6;i ++){
			led.which = i;
			ioctl(led_fd,FSLEDON,&led);
			usleep(500000);

			led.which = i;
			ioctl(led_fd,FSLEDOFF,&led);
			usleep(500000);
		}
	}

	return 0;
}

