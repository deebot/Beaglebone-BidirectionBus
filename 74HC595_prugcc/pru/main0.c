/* Copyright (c) 2020, Deepankar Maithani
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.
   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. */

#include <pru/io.h>
#include <stdint.h>

#include "resource_table_0.h"
#define CLOCK  5   //2.34
#define MOSI   2    //2.32
#define LATCH  3    //2.30
#define CLEARR  6   //2.28  pin
//uint32_t readVal;
volatile uint8_t dataInput= 0b10100000;  // 0x55
volatile uint8_t bitVal;


static void delay_us(unsigned int us)
{
	/* assume cpu frequency is 200MHz */
	__delay_cycles (us * (1000 / 5));
}

const unsigned int period_us = 250 * 1000;


char DatabitRead(volatile uint8_t  *x, char n) {
   return (*x & (1 << n)) ? 1 : 0;
}
int main(void)

{

	write_r30(~(1<<CLEARR)&read_r30());// clear all data   by Low on pin
	delay_us (period_us);
	write_r30(read_r30()|(1<<CLEARR));// set the clear pin High
	write_r30(~(1<<LATCH)&read_r30());// set the latch pin low
	write_r30(~(1<<CLOCK)&read_r30());// set the clock pin low




	for(int i=0;i<8;i++)
	{

		write_r30(DatabitRead(&dataInput,i) & 1 ? (read_r30()|(1<<MOSI)) : ~(1<<MOSI)&read_r30()); // Mount the Value
		write_r30(read_r30()|(1<<CLOCK));// Set clock High
	    delay_us (period_us); // Wait for some time
		write_r30(~(1<<CLOCK)&read_r30());//set the clock Low
		delay_us (period_us);

	}
	write_r30(read_r30()|(1<<LATCH));// Set the latch pin High to push data to output

	//delay_us (period_us);
	//delay_us (period_us);
	//delay_us (period_us);
	//write_r30(~(1<<CLEARR)&read_r30());// clear all data






	return 0;

}


