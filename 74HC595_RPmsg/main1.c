/*
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 	* Redistributions of source code must retain the above copyright
 * 	  notice, this list of conditions and the following disclaimer.
 *
 * 	* Redistributions in binary form must reproduce the above copyright
 * 	  notice, this list of conditions and the following disclaimer in the
 * 	  documentation and/or other materials provided with the
 * 	  distribution.
 *
 * 	* Neither the name of Texas Instruments Incorporated nor the names of
 * 	  its contributors may be used to endorse or promote products derived
 * 	  from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pru_cfg.h>
#include <pru_intc.h>
#include <rsc_types.h>
#include <pru_virtqueue.h>
#include <pru_rpmsg.h>
#include <stdint.h>
#include "resource_table_1.h"


#include <pru/io.h>


/* Host-1 Interrupt sets bit 31 in register R31 */
#define HOST_INT			((uint32_t) 1 << 30)
#define CLOCK  5   //2.34
#define MOSI   2    //2.32
#define LATCH  3    //2.30
#define CLEARR  6   //2.28  pin


/* The PRU-ICSS system events used for RPMsg are defined in the Linux device tree
 * PRU0 uses system event 16 (To ARM) and 17 (From ARM)
 * PRU1 uses system event 18 (To ARM) and 19 (From ARM)
 */
#define TO_ARM_HOST			16
#define FROM_ARM_HOST			17


#define CHAN_NAME					"rpmsg-pru"

#define CHAN_DESC					"Channel 30"
#define CHAN_PORT					30

/*
 * Used to make sure the Linux drivers are ready for RPMsg communication
 * Found at linux-x.y.z/include/uapi/linux/virtio_config.h
 */
#define VIRTIO_CONFIG_S_DRIVER_OK	4

char payload[RPMSG_BUF_SIZE];

static void delay_us(unsigned int us)
{
	/* assume cpu frequency is 200MHz */
	__delay_cycles (us * (1000 / 5));
}
const unsigned int period_us = 250 * 1000;


char DatabitRead(volatile uint8_t  *x, char n) {
   return (*x & (1 << n)) ? 1 : 0;
}



static void handle_mailbox_interrupt(struct pru_rpmsg_transport *transport)
{
	uint16_t src, dst, len;

	/* Clear the event status */
	CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST;

	/* Receive all available messages, multiple messages can be sent per kick */
	while (pru_rpmsg_receive(transport, &src, &dst, payload, &len) == PRU_RPMSG_SUCCESS) {
		/* Echo the message back to the same address
		 * from which we just received */

		/* there is no room in IRAM for iprintf */
		//itoa(d_mm, payload, 10);
		uint8_t i = atoi(payload);
		uint8_t dataInput=atoi(itoa(i, payload, 2));
		//uint8_t dataInput=0b11111111;
		for(int i=0;i<8;i++)
			{

				write_r30(DatabitRead(&dataInput,i) & 1 ? (read_r30()|(1<<MOSI)) : ~(1<<MOSI)&read_r30()); // Mount the Value
				write_r30(read_r30()|(1<<CLOCK));// Set clock High
			    delay_us (period_us); // Wait for some time
				write_r30(~(1<<CLOCK)&read_r30());//set the clock Low
				delay_us (period_us);

			}
			write_r30(read_r30()|(1<<LATCH));// Set the latch pin High to push data to output


		pru_rpmsg_send(transport, dst, src,
			       payload, strlen(payload) + 1);
		//}
	}
}

int main(void)
{

	write_r30(~(1<<CLEARR)&read_r30());// clear all data   by Low on pin
	delay_us (period_us);
	write_r30(read_r30()|(1<<CLEARR));// set the clear pin High
	write_r30(~(1<<LATCH)&read_r30());// set the latch pin low
	write_r30(~(1<<CLOCK)&read_r30());// set the clock pin low

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
		/* Check bit 31 of register R31 to see
		 * if the mailbox interrupt has occurred */
		if (read_r31() & HOST_INT) {
			handle_mailbox_interrupt(&transport);
		}
	}

	return 0;
}
