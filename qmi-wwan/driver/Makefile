#
# Makefile for USB Network drivers by chenlei
#
obj-m := qmi_wwan.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
OUTPUTDIR=/lib/modules/`uname -r`/kernel/drivers/net/usb/

all: 
	$(MAKE) -C $(KDIR) M=$(PWD) modules

install: all
	mkdir -p $(OUTPUTDIR)
#	cp -f qmi_wwan.ko $(OUTPUTDIR)
	cp -f qmi_wwan_bm.ko $(OUTPUTDIR)
	depmod

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean


