PROJ1=write_onebit
PROJ2=write_multbits
PROJ3=read_oneline
PROJ4=read_mullines
PROJ5=read_onelineauto

CC=cc
SRC1=write_onebit.c
SRC2=write_multbits.c
SRC3=read_oneline.c
SRC4=read_mullines.c
SRC5=read_onelineauto.c
LIBS=-lgpiod
CFLAGS=-D CONSUMER=\"$(PROJ)\"

all:
	$(CC) $(SRC1) $(LIBS) $(CFLAGS) -o $(PROJ1)
	$(CC) $(SRC2) $(LIBS) $(CFLAGS) -o $(PROJ2)
	$(CC) $(SRC3) $(LIBS) $(CFLAGS) -o $(PROJ3)
	$(CC) $(SRC4) $(LIBS) $(CFLAGS) -o $(PROJ4)
	$(CC) $(SRC5) $(LIBS) $(CFLAGS) -o $(PROJ5)

clean:
	rm $(PROJ1)
	rm $(PROJ2)
	rm $(PROJ3)
	rm $(PROJ4)
	rm $(PROJ5)
	
