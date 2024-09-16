/*************************************************************************
	#	 FileName	: led_ctl.h
	#	 Author		: tanghaiyang 
	#	 Email		: tanghaiyang0709@163.com
	#	 Created	: 2024年09月04日 星期三
 ************************************************************************/

#ifndef __CHRDEV_H_
#define __CHRDEV_H_

typedef struct led_desc{
	int which;
	int state;
}led_desc_t;

#define mytype 'f'

#define FSLEDON    _IOW(mytype,1,led_desc_t)
#define FSLEDOFF   _IOW(mytype,0,led_desc_t)

#endif 


