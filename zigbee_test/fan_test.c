#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "linuxuart.h"

int main(int argc, char *argv[])
{
	/* 打开设备 */
	int fd = open_port("/dev/ttyUSB0");
	if(fd < 0)
	{
		perror("open fan failed");
		exit(0);
	}
	/* 设置串口参数 */
	set_com_config(fd, 115200, 8, 'N', 1);
	while(1) {
		char cmdbuf[4] = {0};
		printf("please input your choice: 0:off fan 1:on fan 22:get env\n");
		scanf("%s",cmdbuf);

		char buf[32];
#if 0
		//控制风扇
		write(fd, &cmdbuf, 2);
#else
		//获得环境信息
		write(fd, &cmdbuf, 3);
		read(fd, buf, sizeof(buf));
		printf(">>>>>>>%s\n",buf);

#endif
	}
	close(fd);
	return 0;
}

