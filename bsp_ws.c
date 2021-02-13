#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "bsp_ws.h"

#define FNAME "./sensors.txt"
#define ERRCHECK() do { \
	if(errno) { \
		fprintf(stderr, "Error at " __FILE__ " with message: %s\n", \
				strerror(errno)); \
		fprintf(stderr, __FILE__ " exiting...\n"); \
		exit(1); \
	} \
} while (0) \


// Text file definitions
// must be ordered as expected in adc_data
const char *sensors[2] = {"pt", "lc"};
FILE *fifo;

int bsp_init() {
	// write fifo making code
	int fd = open(FNAME, O_NONBLOCK);
	fifo = fdopen(fd, "r");
	ERRCHECK();
	return 0;
} 

static int _adc_read() {
	char *line = NULL;
	size_t len = 0;
	if(feof(fifo)) {
		// reset the fp so we can still try reading from it
		fseek(fifo, 0, SEEK_CUR);
		printf("holding data...\n");
		return 0;
	} else {
		if(getline(&line, &len, fifo) < 0) {
			ERRCHECK();
			return 1;
		}
		char *str = line;
		char *current = line;
		do {
			char sensorcode[4];
			unsigned int val;
			if(sscanf(current, "%s %x", sensorcode, &val) == 2) {
				for(int i = 0; i < sizeof(sensors)/sizeof(sensors[0]); i++) {
					if(!strcmp(sensorcode, sensors[i])) {
						// set sensor data if its equal
						printf("%s to %x; ", sensorcode, val);
						adc_data[i] = val;
					}
					
				}
			} else {
				printf("Did not read sensor data: %s Continuing...\n", current);
				break;
			}
		} while((current = strchr(current + 1, ',') + 1) > line);
		printf("\n");
		free(line);
	}
}

int bsp_adc_start_dma() {
	_adc_read();
	alarm(1);
}

#define TEST 0
#if TEST
int main() {
	bsp_init();
	while(1) {
		_adc_read();
		sleep(1);
	}
	fclose(fifo);
}
#endif
