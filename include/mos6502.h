#ifndef MOS_6502_H
#define MOS_6502_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>

#define ADDR_RANGE		0x10000 // 64Kb - 6502 addressable range
#define RAM_SIZE		0x0800  // 2KB for RAM
			        // 0 – 800
#define STACK_START		0x0100  // 256
#define STACK_END		0x0200  // 512
#define PRGM_START		0x0800  // program start
#define PRGM_MSIZE		0xF800  // max program size
#define NMI		0xFFFA  // FFFA - FFFB
			        // non-maskable interrupt
#define RSTV		0xFFFC  // FFFC - FFFD
			        // reset vector
#define IRQ_BRK		0xFFFE  // FFFE - FFFF
			        // interrupt request/break
// ANSI codes
#define RST "\x1B[0m"
#define RED "\x1B[31m"
#define BLU "\x1B[34m"
#define WHT "\x1B[37m"
#define UND "\033[4m"

typedef	struct _bus {
	uint8_t		ram[ADDR_RANGE];
	uint8_t		rom[ADDR_RANGE];
	unsigned		ram_prgm_size;
	unsigned		rom_prgm_size;
	unsigned		bank_position;
	void		(*write)(uint8_t*, uint16_t, uint8_t);
	uint8_t		(*read)(uint8_t*, uint16_t);
	uint8_t		(*load_ROM)(struct _bus*, char*);
	void		(*reset)(struct _bus*);
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
	uint8_t		(*opcodes[0x100])(struct _6502*);
				// opcodes matrix
	void		(*load_ROM)(_bus*);
	uint8_t		(*pull)(struct _6502*);
	void		(*push)(struct _6502*, uint8_t);
	void		(*set_flag)(struct _6502*, uint8_t, uint8_t);
	uint8_t		(*get_flag)(struct _6502*, uint8_t);
	void		(*reset)(struct _6502*);
	void		(*instruction_cycle)(void*);
				// fetch-decode-execute

	uint8_t		opcode;	// last fetched opcode
	uint8_t		cycles;   // last instr. cycles count
	_bus		*bus;	// BUS Address
}	_6502;

/* cycle.c */
void	instruction_cycle(void*);

/* instructions.c */
void	load_instructions(_6502*);

/* cpu_methods.c */
void	cpu_init(_6502*);

/* bus.c */
void	bus_init(_bus*);

/* print.c */
void	print_state(_6502*);

#endif
