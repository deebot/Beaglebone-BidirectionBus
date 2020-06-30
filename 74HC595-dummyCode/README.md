# PRUDummyShift.c  and ReadWritePRU.c
This code are test codes  so as to test the bitwise operations on a linux system
since there are no print statements and no straighntforward way to debug register values. These codes are written to test these fuctions in more friendly linux envirenment.

## ReadWritePRU.c
	This code  writes oto dummy r30 register after reading bits from data variable

## PRUDummyShift.c
	This is the basic implementation of SPI bitbanging test implmentation which later made the basis of actual implmentation on shift register


## Build  and run the Code
	gcc -o ReadWritePRU.o   ReadWritePRU.c
	./ReadWritePRU

	gcc -o PRUDummyShift.o  PRUDummyShift.c
	./PRUDummyShift
