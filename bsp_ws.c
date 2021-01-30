#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "bsp_ws.h"

#define FNAME "./sensors.txt"
const char* sensors[2] = {"pt", "lc"}; // pressure transducer, load cell
FILE* fifo;

int bsp_init() {
	// write fifo making code
	fifo = fopen(FNAME, "r");
	if(errno) {
		fprintf(stderr, "Error opening sensorfile with message: %s\n",
				strerror(errno));
		fprintf(stderr, __FILE__ " exiting...\n");
		exit(1);
	}
	return 0;
} 

static int _adc_read() {
	
}

int bsp_adc_start_dma() {

}

#define TEST 1
#if TEST
int main() {
	bsp_init();
	fclose(fifo);
}
#endif
