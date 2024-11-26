#ifndef MOS_6502_H
#define MOS_6502_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>


#define ADDRESS_RANGE	0x10000 // 64Kb
#define RAM_SIZE		0x0800  // 2KB for RAM
			        // 0 – 800
#define STACK_START		0x0100  // 256
#define STACK_END		0x0200  // 512
#define PROGRAM_START	0x0800
#define MAX_PROGRAM_SIZE	0xF800
#define NMI		0xFFFA  // - FFFB
#define RESET		0xFFFC  // - FFFD
#define IRQ_BRK		0xFFFE  // - FFFF

#define RST "\x1B[0m"
#define RED "\x1B[31m"
#define BLU "\x1B[34m"
#define WHT "\x1B[37m"
#define UND "\033[4m"

typedef	struct _bus {
	uint8_t		ram[ADDRESS_RANGE];
	uint8_t		rom[ADDRESS_RANGE];
	void		(*write)(uint8_t*, uint16_t, uint8_t);
	uint8_t		(*read)(uint8_t*, uint16_t);
	void		(*reset)(struct _bus*);
	unsigned		ram_program_size;
	unsigned		rom_program_size;
	unsigned		bank_position;
}	_bus;

typedef	struct _6502 { 
	unsigned		PC;	// program counter (16bits)
	uint8_t		A;	// Accumulator
	uint8_t		X;	// Index register
	uint8_t		Y;	// Index register
	uint8_t		SP;	// stack pointer
	uint8_t		SR;	// status register
				// N V - B D I Z C
	////////////
	uint8_t		(*instructions[0x100])(struct _6502*);
	void		(*reset)(struct _6502*);
	uint8_t		opcode;
	uint8_t		cycles;
	_bus		*bus;
}	_6502;

void	instruction_cycle(void*);
void	load_instructions(_6502*);
void	print_state(_6502*);

#endif
