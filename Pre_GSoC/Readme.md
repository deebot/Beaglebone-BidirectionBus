Instructions to use systemd service to configure PRU pins (for related files refer folder: Systemd_pinconfig). Configuring this way donot require a system reload. Only systemctl daemon reload may be required. 

1. cp pruconfig.sh /usr/bin
2. sudo chmod ugo+x config-pru-pins.sh
3. sudo cp  capeconfig.service .../lib/systemd/system
4. sudo systemctl enable capeconfig.service

To configure without systemd used the /boot/uEnv.txt file to access uboot overlay and other settings.
