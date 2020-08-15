# GPIOchip Driver for Rpmsg Communication
The gpiochip module provides a sysfs interface to communicate using rpmsg with the PRUs. 

## Required Headers for Building modules
If you have already built modules on your machine earlier you would be having the headers already installed. You might skip this step
	
	apt-cache search linux-headers-$(uname -r)
	sudo apt install linux-headers-$(uname -r)

## How to Build and Insert the Module
	make
	sudo insmod gpiochip.ko

## Using the module with RPmsg Code.
The gpiochip module is probed when the PRU code is started. It is very important to have the CHAN NAME set to " rpmsg-pru-gpio" to make sure this driver is used and not the character driver.
Also make sure that gpiochip module is loaded before starting the PRU.
	
	cd /sys/class/remoteproc/remoteproc1
	sudo start > state


## Removing the Module	
	sudo rmmod gpiochip.ko	
