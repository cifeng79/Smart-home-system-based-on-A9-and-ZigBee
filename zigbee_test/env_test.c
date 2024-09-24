#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "linuxuart.h"

int main(int argc, char *argv[])
{
    /* 打开设备 */
    int fd = open_port("/dev/ttyUSB1");
    if(fd < 0)
    {
        perror("open port failed");
        exit(0);
    }
    /* 设置串口参数 */
    set_com_config(fd, 115200, 8, 'N', 1);
    while(1) {
        char cmdbuf[4] = {0};
        printf("please input your choice: 0:off fan 1:on fan 2:get environment info\n");
        scanf("%s", cmdbuf);

        char buf[32];
        memset(buf, 0, sizeof(buf)); // 清空缓冲区

        // 发送指令
        write(fd, cmdbuf, strlen(cmdbuf));

        // 等待协调器响应
        usleep(100000); // 延迟100毫秒

        // 读取响应
        int n = read(fd, buf, sizeof(buf) - 1);
        if (n > 0) {
            buf[n] = '\0'; // 确保字符串以null结尾
            printf("Received: %s\n", buf);
        } else {
            printf("Failed to read from device\n");
        }
    }
    close(fd);
    return 0;
}
