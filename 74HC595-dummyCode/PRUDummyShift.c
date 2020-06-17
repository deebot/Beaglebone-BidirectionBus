#include <stdio.h>
#include <stdint.h>
#include <time.h>
#define CLOCK 6     // P2.28
#define MOSI 3      //  P2.30
#define LATCH 2   //P2.32


//uint32_t m =0b100101100; //300
uint32_t data =0xA1; //300 0b10100001
volatile uint32_t r30=0x00000 ; //300
//volatile uint32_t r31 ;
void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;

    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}

void bitWrite(volatile uint32_t *x, char n, char value) {
   if (value)
   {

      *x |= (1 << n);
     // printf("i am here");
       //printf("inside the bitwrite print val %d \n",*x);

   }
   else
      *x &= ~(1 << n);
}

char bitRead(volatile uint32_t  *x, char n) {
   return (*x & (1 << n)) ? 1 : 0;
}

int main()
{


	for(int i=0;i<8;i++)// Read 8 bits one by one and shift it inside the shift register
	{
		bitWrite(&r30, MOSI,bitRead(&data,i));// MOunt the bit on the Pin
		printf("Loaded value for bit%d \n",i);
		printf("The value loaded is %d \n",bitRead(&data,i));
		bitWrite(&r30, CLOCK,1); // Make clock pin go high
		printf("Clock goes high \n");
		delay(2);// Let the clock settle
		bitWrite(&r30, CLOCK,0);
		printf("Clock goes Low \n");



	}
	bitWrite(&r30, LATCH,1);// Move data to output


return 0;
}
