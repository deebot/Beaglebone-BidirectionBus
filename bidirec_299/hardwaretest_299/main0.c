/*  Copyright (c) 2020, Deepankar Maithani
 *
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
/*To reduce pins tie together OE1 AND OE2  to GND. And also one of S0 S1 can be tied to GND*/
#define CLOCK	5   //2.34
#define MOSI    2   //2.32
#define S0		1   //1.33
#define S1  	4   //1.31
#define OE1   	3   //2.30
#define OE2 	6   //2.28
#define Q0     ((uint32_t)1<< 15)   //2.18

volatile uint8_t dataInput= 0b10101010;
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
/* CONFIGURE FOR LOADING
 *  set OE1 HIGH
 *  set OE2 HIGH
 *  set S0 HIGH
 *  set S1 LOW
*/
static void config_SIPOMode(){
write_r30(read_r30()|(1<<OE1));
write_r30(read_r30()|(1<<OE2));
write_r30(read_r30()|(1<<S0));
write_r30(~(1<<S1)&read_r30());
write_r30(~(1<<CLOCK)&read_r30());// set the clock pin low
}



/* Set all 8 bits in the register to 0.Requires config_SIPO to be run first. */
static void clear_Register(){
	for(int i=0;i<8;i++){
		write_r30(~(1<<MOSI)&read_r30()); // Mount the Value
		write_r30(~(1<<CLOCK)&read_r30());//set the clock Low
		delay_us (period_us); // Wait for some time
		write_r30(read_r30()|(1<<CLOCK));// Set clock High
		delay_us (period_us);
	}
}

/* Load the value which will is stored in the variable. Requires config_SIPO to be run first.
 * Eventually this data will come from userspace through a GPIO chip driver
 * */
static void load_Value(){
	for(int i=0;i<8;i++){
		write_r30(DatabitRead(&dataInput,i) & 1 ? (read_r30()|(1<<MOSI)) : ~(1<<MOSI)&read_r30()); // Mount the Value
		write_r30(~(1<<CLOCK)&read_r30());//set the clock Low
		delay_us (period_us); // Wait for some time
		write_r30(read_r30()|(1<<CLOCK));// Set clock High
		delay_us (period_us);
	}

}

/* CONFIGURE TO PUSHOUT Data to make the data available on the outputs
 * set OE1 LOW
 * set OE2 LOW
 * set S0 LOW
 * set S1 LOW
*/
static void push_Out(){
	write_r30(~(1<<OE1)&read_r30());
	write_r30(~(1<<OE2)&read_r30());
	write_r30(~(1<<S0)&read_r30());
	write_r30(~(1<<S1)&read_r30());
}

/*CONFIGURE TO SCAN INPUTS
 * set OE1 LOW
 * set OE2 LOW
 * set S0 HIGH
 * set S1 HIGH
 */
static void config_PISOMode(){
	write_r30(~(1<<OE1)&read_r30());  // set OE1 to low
	write_r30(~(1<<OE2)&read_r30());// set OE2 to low
	write_r30(read_r30()|(1<<S0)); // set S0 to HIGH
	write_r30(read_r30()|(1<<S1)); //set S1 TO HIGH
	write_r30(~(1<<CLOCK)&read_r30());// set the clock pin low
	delay_us(period_us);
	write_r30(read_r30()|(1<<CLOCK)); //High

}

/*READ INPUTS */
static uint8_t read_Inputs(){
	uint8_t incoming=0b00000000;
	write_r30(~(1<<S0)&read_r30());//S0 set to LOW    //SHIFT LEFT MODE
	write_r30(read_r30()|(1<<S1)); // S1 set to HIGH


	for(int i=0;i<8;i++){
		write_r30(read_r30()|(1<<CLOCK));// Set clock High
		incoming= incoming | (((read_r31()& Q0)>>15)<<i) ;
		write_r30(~(1<<CLOCK)&read_r30());//set the clock Low

	}
	return incoming;
}

int main(void)
{
	config_SIPOMode();
	clear_Register();
	load_Value();
	push_Out();
	delay_us(period_us*4);
	dataInput= 0b01010101;
	load_Value();
	push_Out();

	return 0;
}


