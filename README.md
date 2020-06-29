# Beaglebone-BidirectionBus
This repo contains the  PCB files , device drivers and other firmware to implement  a bidirectional Bus with a Beaglebone.



Intuitive language for PRU which compiles down to PRU Assembly.
Details

GSoC 2020 project under BeagleBoard.org

    Name: Deepankar Maithani
    Wiki:  https://elinux.org/BeagleBoard/GSoC/Deepankar
   
    Mentors: Abhishek Kumar,Jason kridner, Saketh, RMA, ds2
    Organisation: BeagleBoard.org


## Folder Description
 - 74HC595-dummyCode : It contains code that can be used to test the bitwise operations and bit manupulation on a Ubuntu machine
 - 74HC595_prugcc  : It contains firemwre code to implement a unidirectional bus using 74HC595 and schematic diagram.
 - 74HC595_PRUGCC_RPMSG: It contains code to implement unidirectional bus, schematic, driver, userspace code and pru firmware
 - References :  Folder contains  Data sheets and important links related to topic found on internet
 - PreGSoC folder contains custom designed Eagle libraries for components needed on the Cape


