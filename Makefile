obj-m += pad.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	chmod +x *.sh
	gcc gtapp.c -o gtapp `pkg-config --cflags --libs gtk+-3.0`


clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
