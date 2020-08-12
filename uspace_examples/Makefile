PROJ1=gpiochip_singlbit
PROJ2=gpiochip_multibit
PROJ3=gpiochip_singlbitinput
PROJ4=gpiochip_multibitinput
#PROJ5=modes
CC=cc
SRC1=gpio_singlbit.c
SRC2=gpio_multibit.c
SRC3=gpio_singlbitinput.c
SRC4=gpio_multibitinput.c
#SRC5=Modes_test.c
LIBS=-lgpiod
CFLAGS=-D CONSUMER=\"$(PROJ)\"

all:
	$(CC) $(SRC1) $(LIBS) $(CFLAGS) -o $(PROJ1)
	$(CC) $(SRC2) $(LIBS) $(CFLAGS) -o $(PROJ2)
	$(CC) $(SRC3) $(LIBS) $(CFLAGS) -o $(PROJ3)
	$(CC) $(SRC4) $(LIBS) $(CFLAGS) -o $(PROJ4)
#	$(CC) $(SRC5) $(LIBS) $(CFLAGS) -o $(PROJ5)

clean:
	rm $(PROJ1)
	rm $(PROJ2)
	rm $(PROJ3)
	rm $(PROJ4)
#	rm $(PROJ5)
	
