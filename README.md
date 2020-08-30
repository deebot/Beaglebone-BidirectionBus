# Beaglebone-BidirectionBus
Bidirectional communication  allows the Beaglebone  to  take inputs and control multiple outputs from the PRU. 


Details

GSoC 2020 project under BeagleBoard.org

    Name: Deepankar Maithani
    Wiki:  https://elinux.org/BeagleBoard/GSoC/Deepankar
   
    Mentors: Abhishek Kumar,Jason kridner, Saketh, RMA, ds2
    Organisation: BeagleBoard.org

## Requirements
 - pru-gcc : Required to compile the PRU firmware. Install using following commands
 
	sudo apt-get update
	sudo apt-get install gcc-pru
	
More info can be found here [gnupru](https://github.com/dinuxbg/gnupru)
 - libgpiod: Required to compile the userspace code. Current version used is <em>1.4.1-2rcnee3~buster+20190906</em>
 	
	sudo apt-get install -y libgpiod-dev
 
 - Linux version used for development is (not a strict requirement):   <em> Linux beaglebone 4.19.94-ti-r42 #1buster SMP PREEMPT Tue Mar 31 19:38:29 UTC 2020 armv7l GNU/Linux </em>
 
 
 - Linux Headers: Required to build the kernel Module

	apt-cache search linux-headers-$(uname -r)
	sudo apt install linux-headers-$(uname -r)

## Folder Description



* **unidirec_595** : It contains code to establish unidirectional communication between ARM and PRU

    * **hardwaretest_595**: It contains pru firmware that can be run to test if all the wirings are correctly made and IC is working fine
    * **pru_pinconfig**:It contains shell script and systemd service file to set modes on the enhanced PRU pins
    * **rpmsg_595**:Contains pru firmware to establish communication between ARM and PRU. The user can send data from userspace to the shift register outputs
    * [ReadMe](https://github.com/deebot/Beaglebone-BidirectionBus/tree/dev/unidirec_595): Explains the usage of code in the folder
    

* **biidirec_299** : It contains code to establish Bidirectional communication between ARM and PRU

    * **hardwaretest_299**: It contains pru firmware that can be run to test if all the wirings are correctly made and IC is working fine
    * **pru_pinconfig**:It contains shell script and systemd service file to set modes on the enhanced PRU pins
    * **rpmsg_299**:Contains pru firmware to establish communication between ARM and PRU. The user can send data from userspace to the shift register outputs  or can read the shiftregister pins as input
    * [ReadMe](https://github.com/deebot/Beaglebone-BidirectionBus/blob/dev/bidirec_299/README.md): Explains the usage of code in the folder

	
* **driver** :It contains the loadable kernel module and make file to build it.

    * [ReadMe](https://github.com/deebot/Beaglebone-BidirectionBus/blob/dev/driver/README.md): Explains the usage of code in the folder


* **References** : It contains documents like TRM, SRM and data sheets


* **uspace_examples** : It contains codes that runs in the userspace to fetch or send data. It uses libgpiod.

## Usage and other Details.

Refer to the  [Wiki](https://github.com/deebot/Beaglebone-BidirectionBus/wiki)  and ReadMe files inside the folders

## Some Related Video Links

- [GSOC Introductory Video](https://www.youtube.com/watch?v=cKxx8r4FRqY)
- [74hc595 Interfacing with PocketBeagle](https://www.youtube.com/watch?v=R-aSw67c1D4)
- [Sending data using GPIOchip driver](https://www.youtube.com/watch?v=gZFyMHmyDzk)
- [Reading inputs from 74HC299 on pocketBeagle](https://www.youtube.com/watch?v=Nn79KsYPg94)


## Some Related BlogPosts
- [Overview of GPIO subsystem on Beaglebon](https://deebot.github.io/BeagleboneGPIO.html)
- [Logging Boot Messages on a pocketbeagle](https://deebot.github.io/BootSerialLogging.html)
- [Mapping PRU pins with R30 and R31](https://deebot.github.io/PRURegisterMapping.html)
- [Working with Rpmsg on PRU0 and PRU1](https://deebot.github.io/rpmsgPRU.html)
- [Shared Memory and OCMC](https://deebot.github.io/sharedMemory.html)


  
