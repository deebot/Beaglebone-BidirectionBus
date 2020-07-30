
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
#include "resource_table_0.h"

/*Pin configuration*/
#define  AD0	6   //2.28
#define  AD1    3   //2.30
#define  AD3    2   //1.32
#define  AD4	5   //1.34
#define WR     4    //1.31    1
#define io-mem	1	//1.33	1
#define RD     7  //1.29   0
#define ALE     1 //1.33



#define     ADDR_EN     (1<<ALE)  // ALE pin high
#define     DATA_EN     ~(1<<ALE) 		//  ALE pin lOW             10000000
#define  	IORD		 ~(1<<RD)
#define     IOWR   		  ~(1<<WR)
uint8_t address =0x05;	      //                AD4-AD3-AD2-AD1-AD0
uint8_t data=0x01;


/** Read data
 *
 *  Reads 1 byte
 *
 *  @return 8 bit data which is read from bus
 *  @param addr 8 bit address where the data should be read from
 *  @see write_data()
 */
void write_Address()    // 4 bit addreess bus
{
	write_r30(read_r30()|ADDR_EN));  //Sending Address
	write_r30(read_r30 &IOWR);  //io READ MODE

	for (int i=0;i<4;i++)
	{
		switch(i)
		case0:
		write_r30(DatabitRead(&address,i) & 1 ? (read_r30()|(1<<AD0)) : ~(1<<AD0)&read_r30());
		break;

		case1:
		write_r30(DatabitRead(&address,i) & 1 ? (read_r30()|(1<<AD1) : ~(1<<AD1)&read_r30());
		break;

		case2:
		write_r30(DatabitRead(&address,i) & 1 ? (read_r30()|(1<<AD2)) : ~(1<<AD2)&read_r30());
		break;

		case3:
		write_r30(DatabitRead(address,i) & 1 ? (read_r30()|(1<<AD3)) : ~(1<<AD3)&read_r30());
		break;

	}


}

void write_data()    //4 bit data bus
{
	write_r30(read_r30&DATA_EN); //data mode enabled
	write_r30(read_r30 &IOWR);  //io Write MODE



	for (int i=0;i<4;i++)
	{
		switch(i)
		case0:
		write_r30(DatabitRead(&data,i) & 1 ? (read_r30()|(1<<AD0)) : ~(1<<AD0)&read_r30());
		break;

		case1:
		write_r30(DatabitRead(&data,i) & 1 ? (read_r30()|(1<<AD1) : ~(1<<AD1)&read_r30());
		break;

		case2:
		write_r30(DatabitRead(&data,i) & 1 ? (read_r30()|(1<<AD2)) : ~(1<<AD2)&read_r30());
		break;

		case3:
		write_r30(DatabitRead(&data,i) & 1 ? (read_r30()|(1<<AD3)) : ~(1<<AD3)&read_r30());
		break;

	}


}



uint8_t read_data()    //4 bit data bus
{
	write_r30(read_r30&DATA_EN); //data mode enabled
	write_r30(read_r30 &IORD);  //io Write MODE

	   uint8_t data= read_r31&(1<<AD0)|read_r31&(1<<AD1)|read_r31&(1<<AD2)|read_r31&(1<<AD3);

	return data;


}


char DatabitReads(volatile uint8_t  *x, char n)
{
   return (*x & (1 << n)) ? 1 : 0;
}







int main(void)
{

}
