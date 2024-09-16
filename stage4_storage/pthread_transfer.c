#include "data_global.h"
#include "mpu6050.h"
#include "linuxuart.h"

extern struct env_info_client_addr  sm_all_env_info;

void *pthread_transfer(void *arg)
{
	int home_id = 1;
	
	int adc_sensor_data;
	struct mpu6050_data mpu6050_sensor_data;
	char zigbee_data[10] = {0};
	
	int adc_fd = open(ADC_DEV, O_RDWR);
	int mpu_fd = open(MPU6050_DEV, O_RDWR);
	if((adc_fd == -1) || (mpu_fd == -1)){
		perror("adc and mpu6050 open failed");
	}

	int zigbee_fd = open_port("/dev/ttyUSB0");
	if(zigbee_fd < 0)
	{
		perror("zigbee_fd failed");
		exit(0);
	}
	set_com_config(zigbee_fd, 115200, 8, 'N', 1);

	printf("adc_fd:%d\n", adc_fd);
	printf("mpu_fd:%d\n", mpu_fd);
	printf("zigbee_fd:%d\n", zigbee_fd);

	while(1) {
		pthread_mutex_lock(&mutex_transfer);
		pthread_cond_wait(&cond_transfer,&mutex_transfer);
		/* ZigBee 数据 */
		sm_all_env_info.monitor_no[home_id].zigbee_info.head[0]  = 'm';
		sm_all_env_info.monitor_no[home_id].zigbee_info.head[1]  = 's';
		sm_all_env_info.monitor_no[home_id].zigbee_info.head[2]  = 'm';
		sm_all_env_info.monitor_no[home_id].zigbee_info.head[3]  = 'z';
		/* get temp and shidu data */
		char cmd_buf[4] = {0};
		strcpy(cmd_buf,"22\n");
		write(zigbee_fd, &cmd_buf, 3);
		sleep(2);
		read(zigbee_fd, zigbee_data, 7);
		
		sm_all_env_info.monitor_no[home_id].zigbee_info.temperature = (zigbee_data[0]-0x30)*10 + (zigbee_data[1]-0x30);
		sm_all_env_info.monitor_no[home_id].zigbee_info.humidity  = (zigbee_data[4]-0x30)*10 + (zigbee_data[5]-0x30);
		
		sm_all_env_info.monitor_no[home_id].zigbee_info.tempMIN = 2.0;
		sm_all_env_info.monitor_no[home_id].zigbee_info.tempMAX = 20.0;
		sm_all_env_info.monitor_no[home_id].zigbee_info.humidityMIN  = 10.0;
		sm_all_env_info.monitor_no[home_id].zigbee_info.humidityMAX  = 30.0;
		sm_all_env_info.monitor_no[home_id].zigbee_info.reserved[0]  = 0.01;
		sm_all_env_info.monitor_no[home_id].zigbee_info.reserved[1]  = -0.01;
		
		/* A9 数据 */
		sm_all_env_info.monitor_no[home_id].a9_info.head[0]  = 'm';
		sm_all_env_info.monitor_no[home_id].a9_info.head[1]  = 's';
		sm_all_env_info.monitor_no[home_id].a9_info.head[2]  = 'm';
		sm_all_env_info.monitor_no[home_id].a9_info.head[3]  = 'a';
		/*get adc sensor data*/
		if(read(adc_fd, &adc_sensor_data, 4) == -1)
		{
			perror("read adc sensor data");
			close(adc_fd);
			pthread_mutex_unlock(&mutex_transfer);
		}
		sm_all_env_info.monitor_no[home_id].a9_info.adc = ((1.8 * adc_sensor_data) / 4096);
		/* get mpu6050 sensor data*/
		if(ioctl(mpu_fd, MPU6050_GYRO, &mpu6050_sensor_data) == -1)
		{
			perror("get gyro ioctl failed");
			close(mpu_fd);
			pthread_mutex_unlock(&mutex_transfer);
		}
		sm_all_env_info.monitor_no[home_id].a9_info.gyrox  =  (short)mpu6050_sensor_data.gyro.x;
		sm_all_env_info.monitor_no[home_id].a9_info.gyroy  =  (short)mpu6050_sensor_data.gyro.y;
		sm_all_env_info.monitor_no[home_id].a9_info.gyroz  =  (short)mpu6050_sensor_data.gyro.z;
		if(ioctl(mpu_fd, MPU6050_ACCEL, &mpu6050_sensor_data) == -1)
		{
			perror("get accel ioctl failed");
			close(mpu_fd);
			pthread_mutex_unlock(&mutex_transfer);
		}
		sm_all_env_info.monitor_no[home_id].a9_info.aacx   =  (short)mpu6050_sensor_data.accel.x;
		sm_all_env_info.monitor_no[home_id].a9_info.aacy   =  (short)mpu6050_sensor_data.accel.y;
		sm_all_env_info.monitor_no[home_id].a9_info.aacz   =  (short)mpu6050_sensor_data.accel.z;	
		sm_all_env_info.monitor_no[home_id].a9_info.reserved[0]  = 0.01;
		sm_all_env_info.monitor_no[home_id].a9_info.reserved[1]  = -0.01;
		
		pthread_mutex_unlock(&mutex_transfer);
		sleep(2);
	}
}
