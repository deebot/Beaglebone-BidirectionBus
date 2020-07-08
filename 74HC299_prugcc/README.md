# 74HC299-BidirectionalBus
This code can configure 74HC299 in SIPO or PISO mode and reads or writes from the internal register inside the chip.

## Wiring diagram 

![Diagram](image/WiringDiagram.png)
## Firmware
To build the PRU firmware:

	cd 74HC299_prugcc/pru
	make

## Remoteproc host driver

First setup remoteproc driver by following [../REMOTEPROC.md](../REMOTEPROC.md).

Build and install firmware:

	
	sudo cp out/pru-core0.elf /lib/firmware/am335x-pru0-fw
	echo stop /sys/class/remoteproc/remoteproc1/state
	echo pru-core0.elf > /sys/class/remoteproc/remoteproc1/firmware
	echo start /sys/class/remoteproc/remoteproc1/state


or you can also use the following commands  to configure pin mux:

	sudo config-pin P2.34 pruout
	sudo config-pin P2.32 pruout
	sudo config-pin P2.30 pruout
	sudo config-pin P2.28 pruout
	sudo config-pin P2.18 pruin
	sudo config-pin P1.29 pruout
	sudo config-pin P1.31 pruout
	sudo config-pin P1.33 pruout

change the pin numbers as per your usage

## Acknowledgements
 * Beagleboard.org test debian image has been used for running the blinking LED example: http://beagleboard.org/latest-images/

