#ifndef __DATA_GLOBAL__H__
#define __DATA_GLOBAL__H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <termios.h>
#include <syscall.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <linux/input.h>


/*********************************************************
  data_global.h : 

  ȫ�ֵĺ궨��#define
  ȫ�ֵ��̺߳�������
  ȫ�ֵ��豸�ڵ�����
  ȫ�ֵ���Ϣ���з��ͺ����ⲿextern����
  ȫ�ֵ���Ϣ���д��ݵĽṹ����Ϣ����

 *********************************************************/
extern pthread_mutex_t mutex_client_request,
	   mutex_refresh,
	   mutex_fan,
	   mutex_transfer,
	   mutex_sms,
	   mutex_buzzer,
	   mutex_led,
	   mutex_seg;

extern pthread_cond_t cond_client_request,
	   cond_refresh,
	   cond_fan,
	   cond_transfer,
	   cond_sms,
	   cond_buzzer,
	   cond_led,
	   cond_seg;

extern unsigned char cmd_led;
extern unsigned char cmd_seg;
extern unsigned char cmd_buzzer;
extern unsigned char cmd_fan;

/*********************************************************
  ȫ�ֵĺ궨��
 *********************************************************/

#define MONITOR_NUM   1
#define QUEUE_MSG_LEN  32

#define		GPRS_DEV   		 "/dev/ttyUSB0"
#define		ZIGBEE_DEV 		 "/dev/ttyUSB1"
#define		BEEPER_DEV 		 "/dev/fsbeeper0"
#define		LED_DEV    		 "/dev/fsled0"
#define		ADC_DEV    		 "/dev/fsadc0"
#define		MPU6050_DEV    	 "/dev/fsmpu60500"


/*********************************************************
  ȫ�ֵĽṹ������
 *********************************************************/

typedef  unsigned char uint8_t;
typedef  unsigned short uint16_t;
typedef  unsigned int uint32_t;

//���ǵ��ڴ���������
struct makeru_zigbee_info{
	uint8_t head[3]; //��ʶλ: 'm' 's' 'm'  makeru-security-monitor  
	uint8_t type;	 //��������  'z'---zigbee  'a'---a9
	float temperature; //�¶�
	float humidity;  //ʪ��
	float tempMIN;//�¶�����
	float tempMAX;//�¶����� 
	float humidityMIN;   //ʪ������
	float humidityMAX;   //ʪ������
	uint32_t reserved[2]; //������չλ��Ĭ�����0
};

struct makeru_a9_info{
	uint8_t head[3]; //��ʶλ: 'm' 's' 'm'  makeru-security-monitor  
	uint8_t type;	 //��������  'z'---zigbee  'a'---a9
	float adc;
	short gyrox;   //����������
	short gyroy;
	short gyroz;
	short  aacx;  //���ټ�����
	short  aacy;
	short aacz;
	uint32_t reserved[2]; //������չλ��Ĭ�����0
};

struct makeru_env_data{
	struct makeru_a9_info       a9_info;    
	struct makeru_zigbee_info   zigbee_info;
	uint32_t reserved[2]; //������չλ��Ĭ�����0
};

//���м���������Ϣ�ṹ��
struct env_info_client_addr
{
	struct makeru_env_data  monitor_no[MONITOR_NUM];	//����  �ϼ�---�¼�
};

struct shm_addr
{
	int shm_status;
	struct env_info_client_addr sm_all_env_info;
};


/*********************************************************
  ȫ�ֵ��ⲿ�̺߳�������
 *********************************************************/

extern void *pthread_client_request (void *arg); //����CGI �ȵ�����
extern void *pthread_refresh(void *arg);              //ˢ�¹����ڴ������߳�
extern void *pthread_sqlite(void *arg);                 //���ݿ��̣߳��������ݿ������
extern void *pthread_transfer(void *arg);           //����ZigBee�����ݲ�����
extern void *pthread_sms(void *arg);                //���Ͷ����߳�
extern void *pthread_buzzer(void *arg);          //�����������߳�
extern void *pthread_led(void *arg);                 //led�ƿ����߳�
extern void *pthread_seg(void *seg);
extern void *pthread_fan(void *seg);

extern int send_msg_queue(long type,unsigned char text);

/*********************************************************
  ȫ�ֵ���Ϣ���д��ݵĽṹ������
 *********************************************************/

//��Ϣ���нṹ��
struct msg
{
	long type;//����Ϣ���н�����Ϣʱ�����жϵ���Ϣ����
	long msgtype;//�������Ϣ����
	unsigned char text[QUEUE_MSG_LEN];//��Ϣ����
};

#endif 


