obj-m:=jiffies.o  
simplemodule-objs:=module  
KDIR:=/lib/modules/$(shell uname -r)/build  
MAKE:=make  
default:  
	$(MAKE) -C $(KDIR) M=$(shell pwd) modules  
clean:  
	$(MAKE) -C $(KDIR) M=$(shell pwd) clean  