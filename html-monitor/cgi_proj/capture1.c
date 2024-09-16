#include <stdio.h>
#include <string.h>
#include "cgic.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define N 2

int cgiMain()
{
	int ret;
	int fd;
	char buf[12];
	char mode[N];
	char sto_no[2];
	
	cgiFormString("mode",mode,N);
	cgiFormString("store",sto_no,2);
	
	fd = open("/tmp/webcom",O_WRONLY);
	if(fd < 0)
	{
		perror("open");
		return -1;
	}

	switch(mode[0])
	{
	case '1':
		write(fd, "snapshot1", sizeof(buf));
		break;
	case '3':
		write(fd, "snapshot3", sizeof(buf));
		break;
	case '5':
		write(fd, "snapshot5", sizeof(buf));
		break;
	case '7':
		write(fd, "snapshot7", sizeof(buf));
		break;
	case '9':
		write(fd, "snapshot9", sizeof(buf));
		break;
	default:
		{
			cgiHeaderContentType("text/html\n\n"); 
			fprintf(cgiOut, "<HTML><HEAD>\n"); 
			fprintf(cgiOut, "<TITLE>My CGI</TITLE></HEAD>\n"); 
			fprintf(cgiOut, "<BODY>"); 

			fprintf(cgiOut, "<H2>无效指令</H2>");

			//fprintf(cgiOut, "<a href='.html'>返回</a>"); 
			fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"1;url=../vodeo1.html\">");
			fprintf(cgiOut, "</BODY>\n"); 
			fprintf(cgiOut, "</HTML>\n"); 
			return -1;
		}
	}

	cgiHeaderContentType("text/html\n\n"); 
	fprintf(cgiOut, "<HTML><HEAD>\n"); 
	fprintf(cgiOut, "<TITLE>My CGI</TITLE></HEAD>\n"); 
	fprintf(cgiOut, "<BODY>"); 

	fprintf(cgiOut, "<H2>send sucess</H2>");

	//fprintf(cgiOut, "<a href='.html'>返回</a>"); 
	fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"1;url=../video1.html\">");
	fprintf(cgiOut, "</BODY>\n"); 
	fprintf(cgiOut, "</HTML>\n"); 
	return 0;
}
