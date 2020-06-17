#include <stdio.h>
#include <stdint.h>
#define CLOCK 6     // P2.28
#define MOSI 3      //  P2.30
#define LATCH 2   //P2.32


//uint32_t m =0b100101100; //300
uint32_t data =0x00001; //300
volatile uint32_t r30=0x00000 ; //300
volatile uint32_t r31 ;


void bitWrite(volatile uint32_t *x, char n, char value) {
   if (value)
   {

      *x |= (1 << n);
     // printf("i am here");
       printf("inside the bitwrite print val %d \n",*x);

   }
   else
      *x &= ~(1 << n);
}

char bitRead(volatile uint32_t  *x, char n) {
   return (*x & (1 << n)) ? 1 : 0;
}

int main()
{
//bitWrite(&data,0,1);
printf("Value BEFORE edit %d \n",bitRead(&r30,0));
bitWrite(&r30, 0,bitRead(&data,0));
printf("Value AFTER edit %d \n",bitRead(&r30,0));

return 0;
}
