Instructions to use systemd service to configure PRU pins

1. cp pruconfig.sh /usr/bin
2. sudo chmod ugo+x config-pru-pins.sh
3. sudo cp  capeconfig.service .../lib/systemd/system
4. sudo systemctl enable capeconfig.service
