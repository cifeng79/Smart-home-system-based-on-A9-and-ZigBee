#include <stdio.h> 
#include "cgic.h" 
#include <string.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define N 8

struct msg
{
	long type;
	long msgtype;
	unsigned char text[N];
};


int cgiMain() 
{ 
	key_t key;
	char sto_no[2];
	char led_mode[N];
	char led_status[N];
	int msgid;
	struct msg msg_buf;
	memset(&msg_buf,0,sizeof(msg_buf));
	
	/* 获取html消息 */
	cgiFormString("store",sto_no,2);
	cgiFormString("ledvalue",led_mode,N);
	cgiFormString("ledstatus",led_status,N);


	/* 申请Key值 */
	if((key = ftok("/tmp", 'g')) < 0)
	{
		perror("ftok");
		exit(1);
	}
	/* 创建消息队列 */
	if((msgid = msgget(key, 0666)) < 0)
	{
		perror("msgget");
		exit(1);
	}
	
	bzero (msg_buf.text, sizeof (msg_buf.text));
	/* 判断灯光模式 */
	if (led_mode[0] == '0')
	{
		msg_buf.text[0] = ((sto_no[0] - 48)) << 6 | (0x0 << 4) | (0 << 0);
	}
	else if(led_mode[0] == '1')
	{
		msg_buf.text[0] = ((sto_no[0] - 48)) << 6 | (0x0 << 4) | (1 << 0);
	}
	else if(led_mode[0] == '2')
	{
		msg_buf.text[0] = ((sto_no[0] - 48)) << 6 | (0x0 << 4) | (2 << 0);
	}
	else if(led_mode[0] == '3')
	{
		msg_buf.text[0] = ((sto_no[0] - 48)) << 6 | (0x0 << 4) | (3 << 0);
	}
	else if(led_mode[0] == '4')
	{
		msg_buf.text[0] = ((sto_no[0] - 48)) << 6 | (0x0 << 4) | (4 << 0);
	}
	else if(led_mode[0] == '5')
	{
		msg_buf.text[0] = ((sto_no[0] - 48)) << 6 | (0x0 << 4) | (5 << 0);
	}
	else if(led_mode[0] == '6')
	{
		msg_buf.text[0] = ((sto_no[0] - 48)) << 6 | (0x0 << 4) | (6 << 0);
	}
	else
	{
		cgiHeaderContentType("text/html\n\n"); 
		fprintf(cgiOut, "<HTML><HEAD>\n"); 
		fprintf(cgiOut, "<TITLE>My CGI</TITLE></HEAD>\n"); 
		fprintf(cgiOut, "<BODY>"); 

		fprintf(cgiOut, "<H2>send failed</H2>");

		//fprintf(cgiOut, "<a href='.html'>返回</a>"); 
		fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"1;url=../a9_zigbee1.html\">");
		fprintf(cgiOut, "</BODY>\n"); 
		fprintf(cgiOut, "</HTML>\n");
		
		return -1;
	}

	msg_buf.type = 1L;
	msg_buf.msgtype = 1L;
	msgsnd(msgid, &msg_buf,sizeof(msg_buf)-sizeof(long),0);

	sto_no[0] -= 48;

	cgiHeaderContentType("text/html\n\n"); 
	fprintf(cgiOut, "<HTML><HEAD>\n"); 
	fprintf(cgiOut, "<TITLE>My CGI</TITLE></HEAD>\n"); 
	fprintf(cgiOut, "<BODY>"); 

	fprintf(cgiOut, "<H2>send sucess</H2>");

	//fprintf(cgiOut, "<a href='.html'>返回</a>"); 
	fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"1;url=../a9_zigbee1.html\">");
	fprintf(cgiOut, "</BODY>\n"); 
	fprintf(cgiOut, "</HTML>\n"); 


	return 0; 
} 
