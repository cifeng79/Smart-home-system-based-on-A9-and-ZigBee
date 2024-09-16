#include "data_global.h"
#include "linuxuart.h"

extern unsigned char cmd_seg; 

void *pthread_fan(void *arg)
{
	char cmd_buf[4] = {0};//风扇控制命令
	int fan_fd = open_port("/dev/ttyUSB0");//根据后期实际设备名修改
	if(fan_fd < 0)
	{
		perror("open fan failed");
		exit(1);
	}
	/* 设置串口参数 */
	set_com_config(fan_fd, 115200, 8, 'N', 1);
	printf("pthread_fan pthread is start\n");
	while(1)
	{
		pthread_mutex_lock(&mutex_fan);
		pthread_cond_wait(&cond_fan, &mutex_fan);
		sizeof(cmd_buf, 4);//清空缓冲区
		switch(cmd_fan)
		{
		case 0x20:
			strcpy(cmd_buf,"1\n");
			write(fan_fd, &cmd_buf, 2);
			printf("风扇已关闭\n");
			sleep(2);
			break;
		case 0x21:
			strcpy(cmd_buf,"0\n");
			write(fan_fd, &cmd_buf, 2);
			printf("风扇1 已开启");
			sleep(2);
			break;
		case 0x22:
			strcpy(cmd_buf,"2\n");
			write(fan_fd, &cmd_buf, sizeof(cmd_buf)/sizeof(cmd_buf[0]));
			printf("风扇2 暂未支持\n");
			sleep(2);
			break;
		case 0x23:
			strcpy(cmd_buf,"3\n");
			write(fan_fd, &cmd_buf, sizeof(cmd_buf)/sizeof(cmd_buf[0]));
			printf("风扇3 暂未支持\n");
			sleep(2);
			break;
		default:
			printf("错误指令！\n");

		}
		pthread_mutex_unlock(&mutex_fan);
	}
}

