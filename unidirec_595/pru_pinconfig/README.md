## Automatically Configuring enhanced PRU pins on Boot
Follow the steps given below to achieve automatic configuration

- Make the script executatble and copy to suitable location

	chmod +x PRUconfig.h
	chmod +x PRUdefault.h
	sudo cp PRUconfig.h /usr/bin
	sudo cp PRUdefault.h /usr/bin

- Copy the unit file to suitable location

	sudo cp pinconfig_595.service /lib/systemd/system
	
-  To start the service. Pins will be configured in enhanced PRU I/O mode
	
	sudo systemctl start pinconfig_595.service
	
- To stop the service. The pins go back to default state
	
	sudo systemctl stop pinconfig_595.service
	
- To enable the service to be executed on boot

	sudo systemctl enable pinconfig_595.service
	
- To disable the service to be executed on boot

	sudo systemctl disable pinconfig_595.service
	
- To check status of service

	sudo systemctl status pinconfig_595.service
	
- Changes take effect on restart or daemonreload using following commang

	sudo systemctl daemon-reload
	
	