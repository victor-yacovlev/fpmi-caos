obj-m += hello.o hello-with-param.o

all:
	make -C /lib/modules/`uname -r`/build M=$(PWD) V=1 modules

clean:
	make -C /lib/modules/`uname -r`/build M=$(PWD) V=1 clean
