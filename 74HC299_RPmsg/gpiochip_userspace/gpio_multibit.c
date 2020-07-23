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


#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

#ifndef	CONSUMER
#define	CONSUMER	"Consumer"
#endif

int main(int argc, char **argv)
{
	char *chipname = "gpiochip5";
	unsigned int line_num[] = {0,1,2,3,4,5,6,7};
	unsigned int val;
	unsigned int total_lines=8;
	struct gpiod_chip *chip;
	struct gpiod_line *line;
	struct gpiod_line_bulk *bulklines;
	int values[]={1,1,1,1,0,0,1,1};
	int i, ret;
	chip = gpiod_chip_open_by_name(chipname);
	if (!chip) {
		perror("Open chip failed.Please check the gpiolib number in /dev\n");
		goto end;
	}

	ret=gpiod_chip_get_lines(chip,
			 line_num, total_lines,
			 bulklines) ;
	if (ret<0) {
		perror("Get lines failed\n");
		goto close_chip;
	}

	ret = gpiod_line_request_bulk_output(bulklines,
					   CONSUMER,0);
	if (ret < 0) {
		perror("Request lines as output failed\n");
		goto release_line;
	}

	ret= gpiod_line_set_value_bulk(bulklines,
				      values);
	if (ret < 0) {
		perror("Set line output failed\n");
		goto release_line;
	}
	//printf("Output %u on line #%u\n", val, line_num);
	sleep(1);

release_line:
	gpiod_line_release_bulk(bulklines);
close_chip:
	gpiod_chip_close(chip);
end:
	return 0;
}
