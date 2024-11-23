#ifndef MOS_6502_H
#define MOS_6502_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>

#define	RAM_SIZE	0xFA00

typedef	struct _6502 {
	//////////////
	uint16_t		PC;	// program counter
	uint8_t		A;	// Accumulator
	uint8_t		X;	// Index register
	uint8_t		Y;	// Index register
	uint8_t		SP;	// stack pointer
	uint8_t		ST;	// status register
				// N V - B D I Z C
}	_6502;

//	cycle

void	*instruction_cycle(void*, uint8_t*);

//	instructions

#endif
