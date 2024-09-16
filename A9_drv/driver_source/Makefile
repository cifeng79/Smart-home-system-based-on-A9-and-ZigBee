KERNELDIR :=/home/cifeng/fs4412/linux-3.14
PWD :=$(shell pwd)

CROSS_COMPILE=arm-linux-
CC=$(CROSS_COMPILE)gcc

obj-m += a9_led_drv.o
obj-m += a9_beep_drv.o
obj-m += a9_adc_drv.o
obj-m += a9_mpu6050_drv.o

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	cp /home/cifeng/securitymonitor/A9_drv/driver_source/*.ko /home/cifeng/securitymonitor/A9_drv/driver_modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

install:
	# sudo rm /opt/4412/rootfs/driver_modules/*.ko
	sudo cp /home/cifeng/securitymonitor/A9_drv/driver_modules/*.ko /opt/4412/rootfs/driver_modules
