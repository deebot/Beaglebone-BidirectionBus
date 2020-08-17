# 74HC595-ARM to PRU Unidirectional Communication
This code can configure 74HC595 in SIPO mode.

## <u>Connect and Test Circuit</u>

### Wiring diagram 

![Diagram](unidirec_595/image/WiringDiagram.png)

### Configure Overlays

	sudo nano /boot/uEnv.txt
	
As shown in the image uncomment the overlays to enable remoteproc and disable UiO.

![Diagram]((unidirec_595/image/uenv.png)

### Configure Enhanced PRU pins

	cd hardwaretest_595/pru_pinconfig
	chmod +x PRUconfig.sh
	./PRUconfig.sh
In order to configure pins automatically on boot refer to README in  pru_pinconfig folder
### Build and Install the firmware to test the connections
	
	cd hardwaretest_595
	make
	make install_PRU0

### Result	

If alternate LEDs connected to Q0 to Q7 glows. This means the connections are OK. and everything is configured.



## <u>Rpmsg Based Communication between the PRU and ARM</u>

To 74HC595 can do a serial input parallel output mode So  the data sent from the userspace on the ARM side can be send to the PRU from where PRU Will push it out.

### Build and insert Driver
	
	cd driver
	make
	sudo insmod gpiochip.ko
	
### Build the userspace code
	
	cd uspace_examples
	make
	
### Test Unidirectional Communication
	./gpiochip_multibit
If everything works fine you should see  the LEDs connected to the output glow in a specific pattern. To change the pattern change the values array.


	
### Debugging Tips

- If you see following error while executing userspace code "Open chip failed.Please check the gpiolib number in /dev"

	ls -l /dev | grep gpio
 
 	change the chipname pointer variable value if required.
 
- If the LEDs are not glowing at all

  Please check if the ground of beagleboard, shift registers are connected together.

 
 


	