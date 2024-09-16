
CROSS_COMPILE=arm-linux-
CC=$(CROSS_COMPILE)gcc
CFLAGS= -c -g
#LDFLAGS= -lpthread -L ./lib -lsqlite3
LDFLAGS= -lpthread

OBJS=main.o linuxuart.o data_global.o\
	 pthread_client_request.o pthread_buzzer.o pthread_led.o\
	 pthread_seg.o\
	 pthread_sms.o sem.o pthread_transfer.o pthread_refresh.o pthread_fan.o

all:monitor_obj install

monitor_obj :$(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)
	mv *o ./obj
$(OBJS):%.o:%.c
	$(CC) $(CFLAGS) $< -o $@

install:
	sudo rm /opt/4412/rootfs/monitor_obj
	sudo cp monitor_obj /opt/4412/rootfs/ 

.PHONY:clean
clean:
	rm *.o monitor_obj -rf 

