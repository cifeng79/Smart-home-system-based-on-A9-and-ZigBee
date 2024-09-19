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
	char seg[N];
	char segvalue[N];
	char sto_no[2];
	int seg_no;
	int msgid;
	struct msg msg_buf;
	memset(&msg_buf,0,sizeof(msg_buf));

	cgiFormString("seg",seg,N);
	cgiFormString("segvalue",segvalue,N);
	cgiFormString("store",sto_no,2);
	seg_no = atoi(segvalue);
	

	if((key = ftok("/tmp", 'g')) < 0)
	{
		perror("ftok");
		exit(1);
	}

	if((msgid = msgget(key, 0666)) < 0)
	{
		perror("msgget");
		exit(1);
	}

	bzero (msg_buf.text, sizeof (msg_buf.text));


	if (seg[0] == '1')
	{
		if(0 <= seg_no <= 15)
		{
			msg_buf.text[0] = ((sto_no[0] - 48)) << 6 | (3 << 4) | (seg_no << 0);
		}
		else
		{
			cgiHeaderContentType("text/html\n\n"); 
			fprintf(cgiOut, "<HTML><HEAD>\n"); 
			fprintf(cgiOut, "<TITLE>My CGI</TITLE></HEAD>\n"); 
			fprintf(cgiOut, "<BODY>"); 

			fprintf(cgiOut, "<H2>控制指令不支持</H2>");

			//fprintf(cgiOut, "<a href='.html'>返回</a>"); 
			fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"1;url=../a9_zigbee%d.html\">", sto_no[0]);
			fprintf(cgiOut, "</BODY>\n"); 
			fprintf(cgiOut, "</HTML>\n"); 
		}
	}
	else if(seg[0] == '0')
	{
		msg_buf.text[0] = ((sto_no[0] - 48)) << 6 | (3 << 4) | (15 << 0);
	}

	msg_buf.type = 1L;
	msg_buf.msgtype = 3L;
	msgsnd(msgid, &msg_buf,sizeof(msg_buf)-sizeof(long),0);

	sto_no[0] -= 48;

	cgiHeaderContentType("text/html\n\n"); 
	fprintf(cgiOut, "<HTML><HEAD>\n"); 
	fprintf(cgiOut, "<TITLE>My CGI</TITLE></HEAD>\n"); 
	fprintf(cgiOut, "<BODY>"); 

	fprintf(cgiOut, "<H2>send sucess</H2>");

	//fprintf(cgiOut, "<a href='.html'>返回</a>"); 
	fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"1;url=../a9_zigbee%d.html\">", sto_no[0]);
	fprintf(cgiOut, "</BODY>\n"); 
	fprintf(cgiOut, "</HTML>\n"); 


	return 0; 
} 
