#ifndef MOS_6502_H
#define MOS_6502_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/time.h>
#include <stdlib.h>

#define	RAM_SIZE		0xFA00

typedef	struct _bus {
	uint8_t		ram[RAM_SIZE];
	uint16_t		ram_occupied;
	uint8_t		(*read)(uint8_t*, uint16_t);
	void		(*write)(uint8_t*, uint16_t, uint8_t);
}	_bus;

typedef	struct _6502 { 
	//////////////
	uint16_t		PC;	// program counter
	uint8_t		A;	// Accumulator
	uint8_t		X;	// Index register
	uint8_t		Y;	// Index register
	uint8_t		SP;	// stack pointer
	uint8_t		ST;	// status register
				// N V - B D I Z C
	////////////
	void		(*instructions[0xFF])(struct _6502*);
	////////////
	uint16_t		opcode;
	////////////
	_bus		*bus;
}	_6502;


void	*instruction_cycle(void*, uint8_t*);
void	load_instructions(_6502*);

#endif
