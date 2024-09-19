
#include "data_global.h"
#include "pthread_led.h"

int fsled_control(int led_fd, unsigned char led_control_cmd);

//数码管线程
void *pthread_seg(void *arg)
{
	printf("pthread_seg\n");

	int tmp;
	int seg_fd = -1;
	
	seg_fd = open(LED_DEV, O_RDWR);
	if(seg_fd == -1)
	{
		printf("open failed\n");
	}
	printf("seg_fd: %d.\n", seg_fd);

	while(1)
	{
		pthread_mutex_lock(&mutex_seg);
		pthread_cond_wait(&cond_seg, &mutex_seg);
		
		tmp = cmd_seg & 0xf0;
		if(!(tmp ^ 0x70)) {
			fsled_control(seg_fd, cmd_seg);
		}
		pthread_mutex_unlock(&mutex_seg);

	}

}


int fsled_control(int led_fd, unsigned char led_control_cmd)
{
	int i = 0;
	led_desc_t led;
	led_control_cmd &= 0x0f;
	int shift_count = 1; //第0位，第1 - 3位 

	printf("led_control_cmd = %d.\n",led_control_cmd);
	fsled_close_all(led_fd);
	sleep(3);
	while(led_control_cmd){
		if(shift_count >= 5)
			break;
		if((led_control_cmd & 0x1) == 1){ //第0位开始 = LED2
			shift_count ++;  // = 2  LED2 
			printf("if shift_count :%d.\n",shift_count);
			led.which = shift_count; //led2 3 4 5 灯
			ioctl(led_fd,FSLEDON,&led);
			usleep(50000);  //让驱动响应的时间
		}else {
			shift_count ++;
			printf("else shift_count :%d.\n",shift_count);
			led.which = shift_count; //led2 3 4 5 灯
			printf("open all led\n");
			ioctl(led_fd,FSLEDOFF,&led);
			usleep(50000);
			printf("close all led\n");
		}
		led_control_cmd >>= 1;
	}

	return 0;
}


