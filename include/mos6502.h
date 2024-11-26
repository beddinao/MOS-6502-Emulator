#ifndef MOS_6502_H
#define MOS_6502_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/time.h>
#include <stdlib.h>


#define ADDRESS_RANGE	0x10000

#define RAM_SIZE		0x0800  // 2KB for RAM
			        // 0 – 800

#define STACK_SIZE		0x00FF  // 255
#define STACK_START		0x0100  // 256
#define STACK_END		0x0200  // 512

#define PROGRAM_START	0x0800
#define MAX_PROGRAM_SIZE	0xF800

#define ROM_SIZE		0x10000  // 64kb

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define KUND  "\033[4m"
# define KBGR  "\033[7m"

typedef	struct _bus {
	uint8_t		ram[ADDRESS_RANGE];
	uint8_t		rom[ROM_SIZE];
	unsigned		ram_program_size;
	unsigned		rom_program_size;
	unsigned		bank_position;
	void		(*write)(uint8_t*, uint16_t, uint8_t);
	uint8_t		(*read)(uint8_t*, uint16_t);
	void		(*reset)(struct _bus*);
}	_bus;

typedef	struct _6502 { 
	uint16_t		PC;	// program counter
	uint8_t		A;	// Accumulator
	uint8_t		X;	// Index register
	uint8_t		Y;	// Index register
	uint8_t		SP;	// stack pointer
	uint8_t		ST;	// status register
				// N V - B D I Z C
	////////////
	uint8_t		(*instructions[0xFF])(struct _6502*);
	uint8_t		opcode;
	uint8_t		cycles;
	void		(*reset)(struct _6502*);
	_bus		*bus;
}	_6502;


void	instruction_cycle(void*);
void	load_instructions(_6502*);

#endif
