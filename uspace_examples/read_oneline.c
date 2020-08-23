#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

#ifndef	CONSUMER
#define	CONSUMER	"Consumer"
#endif

int main(int argc, char **argv)
{
	char *chipname = "gpiochip4";
	unsigned int read_linenum = 1;	//
	int val;
	struct gpiod_chip *chip;
	struct gpiod_line *line;
	int i, ret;
   /*Openning the chardev interface for gpiochip driver*/
	chip = gpiod_chip_open_by_name(chipname);
	if (!chip) {
		perror("Open chip failed\n");
		goto end;
	}
    /*grab the line, on success it returns 1 */
	line = gpiod_chip_get_line(chip, read_linenum);
	if (!line) {
		perror("Get line failed\n");
		goto close_chip;
	}
    /*Set the grabed line as input*/
	ret = gpiod_line_request_input(line, CONSUMER);
	if (ret < 0) {
		perror("Request line as input failed\n");
		goto release_line;
	}

	/* Read input 2 times */
	for (i = 2; i > 0; i--) {
		val = gpiod_line_get_value(line);
		if (val < 0) {
			perror("Read line input failed\n");
			goto release_line;
		}
		printf("Intput %d on line #%u\n", val, read_linenum);
		sleep(1);
	}

release_line:
	gpiod_line_release(line);
close_chip:
	gpiod_chip_close(chip);
end:
	return 0;
}
