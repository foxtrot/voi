KERN_PATH := /lib/modules/$(shell uname -r)/build

obj-m += voi.o
voi-objs := \
	src/voi.o \
	src/hooking/hooking.o \
	src/common/helpers.o \
	src/packet/packet.o \
	src/comms/netlink.o \
	src/filter/filter.o

all:
	make -C $(KERN_PATH) M=$(PWD) modules

insert:
	sudo rmmod voi.ko | true
	make -C $(KERN_PATH) M=$(PWD) modules
	sudo insmod voi.ko

clean:
	make -C $(KERN_PATH) M=$(PWD) clean
