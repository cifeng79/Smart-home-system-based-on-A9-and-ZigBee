
CROSS_COMPILE=arm-none-linux-gnueabi-
CC=$(CROSS_COMPILE)gcc

CFLAGS=-Wall -g
RANLIB=$(CROSS_COMPILE)ranlib
LDFLAGS= -L ./lib
LIB=-lcgic

OBJS= capture.cgi a9_beep.cgi zigbee_fan.cgi\
	  a9_led.cgi a9_seg.cgi \
	  env1.cgi setEnv.cgi capture1.cgi

run:libcgic.a $(OBJS) install

$(OBJS):%.cgi:%.c
	$(CC) $(AFLAGS) $< -o $@ $(LDFLAGS) $(LIB)

libcgic.a:cgic.o cgic.h
	rm -f ./lib/libcgic.a
	$(CROSS_COMPILE)ar -rsv libcgic.a cgic.o
	$(RANLIB) libcgic.a
	mv libcgic.a ./lib

cgic.o:cgic.c cgic.h
	$(CC) -c cgic.c -o cgic.o

install:
	$(CC) $(AFLAGS) login.c -o login.cgi
	$(CC) $(AFLAGS) capture1.c cgic.c -o cpature1.cgi
	mv *.cgi ./out/
	sudo cp  ./out/*.cgi  /opt/4412//rootfs/boa/cgi-bin

.PHONY:clean
clean:
	rm ./out/*.cgi

