# Beaglebone-BidirectionBus
A bidirectionalBus allows the Beaglebone  to  take inputs and control multiple outputs from the PRU. 


Details

GSoC 2020 project under BeagleBoard.org

    Name: Deepankar Maithani
    Wiki:  https://elinux.org/BeagleBoard/GSoC/Deepankar
   
    Mentors: Abhishek Kumar,Jason kridner, Saketh, RMA, ds2
    Organisation: BeagleBoard.org


## Folder Description



* **unidirec_595** : It contains code to establish unidirectional communication between ARM and PRU

    * **hardwaretest_595**: It contains pru firmware that can be run to test if all the wirings are correctly made and IC is working fine
    * **pru_pinconfig**:It contains shell script and systemd service file to set modes on the enhanced PRU pins
    * **rpmsg_595**:Contains pru firmware to establish communication between ARM and PRU. The user can send data from userspace to the shift register outputs
    

* **biidirec_299** : It contains code to establish unidirectional communication between ARM and PRU

    * **hardwaretest_299**: It contains pru firmware that can be run to test if all the wirings are correctly made and IC is working fine
    * **pru_pinconfig**:It contains shell script and systemd service file to set modes on the enhanced PRU pins
    * **rpmsg_299**:Contains pru firmware to establish communication between ARM and PRU. The user can send data from userspace to the shift register outputs  or can read the shiftregister pins as input
    
* **driver** : It contains the loadable kernel module and make file to build it.

* **References** : It contains documents like TRM, SRM and data sheets

* **uspace_examples** : It contains codes that runs in the userspace to fetch or send data. It uses libgpiod.

## Usage and other Details.

Refer to the  [Wiki](https://github.com/deebot/Beaglebone-BidirectionBus/wiki)  and ReadMe files inside the folders



  
