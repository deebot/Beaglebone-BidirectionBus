/* Copyright (c) 2020 Deepankar Maithani
   All rights reserved.
   uses rpmsg components from https://github.com/dinuxbg/pru-gcc-examples
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pru_cfg.h>
#include <pru_intc.h>
#include <rsc_types.h>
#include <pru_virtqueue.h>
#include <pru_rpmsg.h>
#include "resource_table_0.h"

/*Pin configuration*/
#define CLOCK	5   //2.34
#define MOSI    2   //2.32
//#define MISO    7   //1.29
#define S0		1   //1.33
#define S1  	4   //1.31
#define OE1   	3   //2.30
#define OE2 	6   //2.28
#define Q0     ((uint32_t)1<< 15)   //1.29 input pin//2.18
#define HOST_INT			((uint32_t) 1 << 30)
#define PISO_MODE 0
#define SIPO_MODE 1

/* Variables to gather data*/
volatile uint8_t decimalInput= 31;
volatile uint8_t dataInput= 0b11110111;  //
volatile uint32_t readVal;
volatile uint16_t temp=0;
volatile uint8_t *ptr;


/* The PRU-ICSS system events used for RPMsg are defined in the Linux device tree
 * PRU0 uses system event 16 (To ARM) and 17 (From ARM)
 */
#define TO_ARM_HOST			16
#define FROM_ARM_HOST			17
#define CHAN_NAME					"rpmsg-pru-gpio"
#define CHAN_DESC					"Channel 30"
#define CHAN_PORT					30

/*
 * Used to make sure the Linux drivers are ready for RPMsg communication
 * Found at linux-x.y.z/include/uapi/linux/virtio_config.h
 */
#define VIRTIO_CONFIG_S_DRIVER_OK	4

char payload[RPMSG_BUF_SIZE];
char payloadcheck[RPMSG_BUF_SIZE];

char  testVal[RPMSG_BUF_SIZE]="1";
static void delay_us(unsigned int us)
{
	/* assume cpu frequency is 200MHz */
	__delay_cycles (us * (1000 / 5));
}

const unsigned int period_us = 250 * 1000;

char DatabitReads(volatile uint8_t  *x, char n)
{
   return (*x & (1 << n)) ? 1 : 0;
}
/*
char DatabitReadss(volatile uint16_t  *x, char n)
{
   return (*x & (1 << n)) ? 1 : 0;
}*/
char DatabitRead(volatile char  *x, char n)
{
   return (*x & (1 << n)) ? 1 : 0;
}
uint16_t DatabitReadint(volatile char  *x, char n)
{
   return (*x & (1 << n)) ? 1 : 0;
}

/* CONFIGURE FOR LOADING
 *  set OE1 HIGH
 *  set OE2 HIGH
 *  set S0 HIGH
 *  set S1 LOW
*/
static void config_SIPOMode()
{
write_r30(read_r30()|(1<<OE1));
write_r30(read_r30()|(1<<OE2));
write_r30(read_r30()|(1<<S0));
write_r30(~(1<<S1)&read_r30());
write_r30(~(1<<CLOCK)&read_r30());// set the clock pin low

}

/* Set all 8 bits in the register to 0.Requires config_SIPO to be run first. */
static void clear_Register()
{
	for(int i=0;i<8;i++){
		write_r30(~(1<<MOSI)&read_r30()); // Mount the Value
		write_r30(~(1<<CLOCK)&read_r30());//set the clock Low
		delay_us (period_us); // Wait for some time
		write_r30(read_r30()|(1<<CLOCK));// Set clock High
		delay_us (period_us);
	}
}

/* Load the value which will is stored in the variable. Requires config_SIPO to be run first.
 * Eventually this data will come from userspace through a GPIO chip driver payload =[110000011]
 * */

static void load_Value()
{
	for(int i=1;i<8;i++){
		write_r30(DatabitRead(&payload[0],i) & 1 ? (read_r30()|(1<<MOSI)) : ~(1<<MOSI)&read_r30()); // Mount the Value
		write_r30(~(1<<CLOCK)&read_r30());//set the clock Low
		delay_us (period_us); // Wait for some time
		write_r30(read_r30()|(1<<CLOCK));// Set clock High
		delay_us (period_us);
	}
	write_r30(DatabitRead(&payload[1],0) & 1 ? (read_r30()|(1<<MOSI)) : ~(1<<MOSI)&read_r30());
	write_r30(~(1<<CLOCK)&read_r30());//set the clock Low
	delay_us (period_us); // Wait for some time
	write_r30(read_r30()|(1<<CLOCK));// Set clock High
	delay_us (period_us);

}


/* CONFIGURE TO PUSHOUT Data to make the data available on the outputs
 * set OE1 LOW
 * set OE2 LOW
 * set S0 LOW
 * set S1 LOW
*/
static void push_Out()
{
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
static void config_PISOMode()
{
	write_r30(~(1<<OE1)&read_r30());  // set OE1 to low
	write_r30(~(1<<OE2)&read_r30());// set OE2 to low
	write_r30(read_r30()|(1<<S0)); // set S0 to HIGH
	write_r30(read_r30()|(1<<S1)); //set S1 TO HIGH
	write_r30(~(1<<CLOCK)&read_r30());// set the clock pin low
	delay_us(period_us);
	write_r30(read_r30()|(1<<CLOCK)); //High

}

/*READ INPUTS */
static uint32_t read_Inputs()
{

	uint32_t incoming2=0x00000000;
	uint32_t value=0x0E20;
	write_r30(~(1<<S0)&read_r30());//S0 set to LOW    //SHIFT LEFT MODE
	write_r30(read_r30()|(1<<S1)); // S1 set to HIGH

	for(int i=0;i<8;i++){
		write_r30(read_r30()|(1<<CLOCK));// Set clock High
		uint32_t output = (read_r31() &Q0)>>(uint32_t)15;
		incoming2=incoming2|(output<<i);

		//incoming2=  incoming2| ((read_r31()& Q0)>>(uint32_t)15)<<i ;                        	//11111111  & 10000000  10000000>>8   00000100
		//incoming2
		delay_us(period_us);
		write_r30(~(1<<CLOCK)&read_r30());//set the clock Low
	}

	return incoming2 ;//incoming;
}

/*static uint8_t convertbinary(uint8_t no)
{
	uint8_t re=0;
	int i=1;
	while(no!=0)
			{
				re=no%2;
				no=no/2;
				dataInput=dataInput+(re*i);
				i=i*10;
			}
}*/


static void handle_mailbox_interrupt(struct pru_rpmsg_transport *transport)
{
	uint16_t src, dst, len;

	/* Clear the event status */
	CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST;

	/* Receive all available messages, multiple messages can be sent per kick */
	while (pru_rpmsg_receive(transport, &src, &dst, payload, &len) == PRU_RPMSG_SUCCESS) {

		char flag=(DatabitRead(&payload[0],0) & 1);

	/*if(flag==SIPO_MODE)
		{
		config_SIPOMode();
		clear_Register();
		load_Value();
		push_Out();
	}*/
	//	else if(flag==PISO_MODE){

			config_PISOMode();
		//	clear_Register();
			readVal=read_Inputs();
			//char *c;
			//c=&payloadcheck;
			//volatile uint8_t testval =0b10101010;
			//for(int i=0;i<8;i++)
			//{
			//c[i]= DatabitReads(&testval, i);
		//	}
		//	ptr=&readVal;
		//	sprintf(payloadcheck,"%d",readVal);
			//char *c;
			//c=payloadcheck;
			//c[0]='m';
			//c[1]='1';
	//	}
			itoa(readVal,payloadcheck,2);
		pru_rpmsg_send(transport, dst, src,
			      payloadcheck,  strlen(payloadcheck));
	}
}

int main(void)
{
	struct pru_rpmsg_transport transport;
	volatile uint8_t *status;

	/* allow OCP master port access by the PRU so the PRU
	 * can read external memories */
	CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

	/* clear the status of the PRU-ICSS system event that the ARM will use to 'kick' us */
	CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST;

	/* Make sure the Linux drivers are ready for RPMsg communication */
	status = &resourceTable.rpmsg_vdev.status;
	while (!(*status & VIRTIO_CONFIG_S_DRIVER_OK))
		;

	/* Initialize the RPMsg transport structure */
	pru_rpmsg_init(&transport,
		       &resourceTable.rpmsg_vring0,
		       &resourceTable.rpmsg_vring1,
		       TO_ARM_HOST,
		       FROM_ARM_HOST);

	/* Create the RPMsg channel between the PRU and ARM user space using the transport structure. */
	while (pru_rpmsg_channel(RPMSG_NS_CREATE, &transport, CHAN_NAME, CHAN_DESC, CHAN_PORT) != PRU_RPMSG_SUCCESS)
		;


	while (1) {
		/* Check bit 30 of register R31 to see
		 * if the mailbox interrupt has occurred */
		if (read_r31() & HOST_INT) {
			handle_mailbox_interrupt(&transport);
		}
	}

	return 0;
}
