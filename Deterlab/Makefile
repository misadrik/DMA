obj-m += testmodule2.o
	CURRENT_PATH:=$(shell pwd)
	KDIR =/lib/modules/$(shell uname -r)/build
default:
	$(MAKE) -C $(KDIR) M=$(CURRENT_PATH) modules
clean:
	rm -rf *.o .*.cmd *.ko *.mod.c .tmp_versions
