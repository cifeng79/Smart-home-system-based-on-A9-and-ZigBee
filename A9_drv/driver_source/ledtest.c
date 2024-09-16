/*************************************************************************
#	 FileName	: test.c
#	 Author		: fengjunhui 
#	 Email		: 18883765905@163.com 
#	 Created	: 2018年07月03日 星期二 15时26分08秒
 ************************************************************************/

#include<stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "chrdev.h"

int fsled_control(int led_fd, unsigned char led_control_cmd);
int fswaterled_control(int led_fd, int times);
int fsled_close_all(int led_fd);

int main(int argc, const char *argv[])
{
	int i = 0,j = 0;
	int led_fd;
	led_desc_t led;

	led_fd = open("/dev/fsled0", O_RDWR,0777);
	if(led_fd == -1){
		printf("open failed .\n");
	}
	printf("led_fd :%d.\n",led_fd);

	fswaterled_control(led_fd,2);
	fsled_control(led_fd,5);

	close(led_fd);
	return 0;
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
			shift_count ++;
			printf("if shift_count :%d.\n",shift_count);
			led.which = shift_count; //led2 3 4 5 灯
			ioctl(led_fd,FSLEDON,&led);
			usleep(50000);
		}else {
			shift_count ++;
			printf("else shift_count :%d.\n",shift_count);
			led.which = shift_count; //led2 3 4 5 灯
			ioctl(led_fd,FSLEDOFF,&led);
			usleep(50000);
		}
		led_control_cmd >>= 1;
	}

	return 0;
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




