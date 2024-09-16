CFLAGS=-g -Wall
CROSS_COMPILE=arm-linux-
#CROSS_COMPILE=
CC=$(CROSS_COMPILE)gcc
AR=$(CROSS_COMPILE)ar
RANLIB=$(CROSS_COMPILE)ranlib
LIBS=-L ./ -lcgic



all: libcgic.a  capture

install: libcgic.a
	cp libcgic.a /usr/local/lib
	cp cgic.h /usr/local/include
	@echo libcgic.a is in /usr/local/lib. cgic.h is in /usr/local/include.

libcgic.a: cgic.o cgic.h
	rm -f libcgic.a
	$(AR) rc libcgic.a cgic.o
	$(RANLIB) libcgic.a

#mingw32 and cygwin users: replace .cgi with .exe


capture: capture.o libcgic.a
	$(CC) $(CFLAGS) capture.o -o capture ${LIBS}

clean:
	rm -f *.o *.a cgictest.cgi capture

ubuntu_boa:
	sudo cp *.cgi /boa/cgi-bin/ 
	sudo cp capture /boa/cgi-bin/ 

fs4412_boa:
	sudo cp *.cgi /opt/4412/rootfs/boa/cgi-bin/ 
	sudo cp capture /opt/4412/rootfs/boa/cgi-bin/ 
	sudo cp libcgic.a /opt/4412/rootfs/lib/ 

