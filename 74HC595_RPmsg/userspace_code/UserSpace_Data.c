

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/poll.h>

#define MAX_BUFFER_SIZE		512
char readBuf[MAX_BUFFER_SIZE];

#define NUM_MESSAGES		1
#define DEVICE_NAME		"/dev/rpmsg_pru31"

int main (void){
	struct pollfd pollfds[1];
	int i;
	int result = 0;

	/* Open the rpmsg_pru character device file */
	pollfds[0].fd = open(DEVICE_NAME, O_RDWR);

	/* 
	 * If the RPMsg channel doesn't exist yet the character device 
	 * won't either. 
	 * Make sure the PRU firmware is loaded and that the rpmsg_pru
	 * module is inserted. 
	 */
	if(pollfds[0].fd < 0) {
		printf("Failed to open %s\n", DEVICE_NAME);
		return -1;
	}

	/* The RPMsg channel exists and the character device is opened */
	printf("Opened %s, sending messages\n\n", DEVICE_NAME);


		/* Send  a no from 0 to 255  to the PRU through the RPMsg channel */
		result = write(pollfds[0].fd, "123", 13);
		if(result > 0)
			printf("Message %d: Sent to PRU\n", i);			
		
		/* Poll until we receive a message from the PRU and then print it */
		result = read(pollfds[0].fd, readBuf, MAX_BUFFER_SIZE);
		if(result > 0)
			printf("Message %d received from PRU:%s\n\n", i, readBuf);
	

	/* Received all the messages the example is complete */
	printf("Received  messages, closing %s\n", DEVICE_NAME);

	/* Close the rpmsg_pru character device file */
	close(pollfds[0].fd);

	return 0;
}

