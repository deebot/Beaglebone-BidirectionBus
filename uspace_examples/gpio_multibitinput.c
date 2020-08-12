#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

#ifndef	CONSUMER
#define	CONSUMER	"Consumer"
#endif

int main(int argc, char **argv)
{
	char *chipname = "gpiochip4";
	//unsigned int line_num = 0;	// GPIO Pin #24
	int val;
	struct gpiod_chip *chip;
	struct gpiod_line *line;
	int i, ret;
	unsigned int line_num[] = {0,1,2,3,4,5,6,7};
	int inputval[8]={0,0,0,0,0,0,0,0};
	unsigned int total_lines=8;
	struct gpiod_line_bulk *bulklines;

	chip = gpiod_chip_open_by_name(chipname);
	if (!chip) {
		perror("Open chip failed\n");
		goto end;
	}

	/*line = gpiod_chip_get_line(chip, line_num);

	if (!line) {
		perror("Get line failed\n");
		goto close_chip;
	}*/
	ret=gpiod_chip_get_lines(chip,
				 line_num, total_lines,
				 bulklines) ;
		if (ret<0) {
			perror("Get lines failed\n");
			goto close_chip;
		}


		ret = gpiod_line_request_bulk_input(bulklines,
							   CONSUMER);
		if (ret < 0) {
				perror("Request lines as input failed\n");
				goto release_line;
			}
		ret= gpiod_line_get_value_bulk(bulklines,
					      inputval);

		if (ret < 0) {
				perror("get line input failed\n");
				goto release_line;
			}
		for(int i=0;i<8;i++)
		{
			printf("Input %d \n", inputval[i]);

		}
		sleep(1);
	/* Read input 20 times */
/*	for (i = 20; i > 0; i--) {
		val = gpiod_line_get_value(line);
		if (val < 0) {
			perror("Read line input failed\n");
			goto release_line;
		}
		printf("Intput %d on line #%u\n", val, line_num);

	}*/

release_line:
	gpiod_line_release_bulk(bulklines);
close_chip:
	gpiod_chip_close(chip);
end:
	return 0;
}
