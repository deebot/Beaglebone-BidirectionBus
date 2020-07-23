PROJ1=gpiochip_singlbit
PROJ2=gpiochip_multibit
CC=cc
SRC1=gpio_singlbit.c
SRC2=gpio_multibit.c
LIBS=-lgpiod
CFLAGS=-D CONSUMER=\"$(PROJ)\"

all:
	$(CC) $(SRC1) $(LIBS) $(CFLAGS) -o $(PROJ1)
	$(CC) $(SRC2) $(LIBS) $(CFLAGS) -o $(PROJ2)

clean:
	rm $(PROJ1)
	rm $(PROJ2)
	
