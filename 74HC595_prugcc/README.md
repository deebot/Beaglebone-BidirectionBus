# 74HC595_prugcc

## Firmware
To build the PRU firmware:

	cd 74hc595_prugcc/pru
	make

## Remoteproc host driver

Install firmware:


	sudo cp out/pru-core0.elf /lib/firmware/am335x-pru0-fw
	sudo cp out/pru-core1.elf /lib/firmware/am335x-pru1-fw
	sync

In order to control the shift register you'll need to configure the pin mux

	sudo config-pin P2.34 pruout
	sudo config-pin P2.32 pruout
	sudo config-pin P2.30 pruout
	sudo config-pin P2.28 pruout


### Start the PRU cores

	cd /sys/class/remoteproc/remotepoc1
	echo start > state

	cd /sys/class/remoteproc/remoteproc2
	echo start > state
###  Stop the Core

	cd /sys/class/remoteproc/remotepoc1
	echo stop > state

	cd /sys/class/remoteproc/remoteproc2
	echo stop > state

Core need to be stoped and started for new firmware to be fucntional
