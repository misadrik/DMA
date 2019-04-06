obj-m := modules.o
modules-objs:= mymode.o

KDIR:0=/lib/modules/`uname -r`/build
PWD:=$(shell pwd)

defalut:
make -C $(KDIR) M=$(PWD) modules

clean:
rm -rf *.o .* .cmd *.ko *.mod.c .tmp_versions