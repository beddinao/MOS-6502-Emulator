#include "mos6502.h"

//// / /	LOADING

void	cpu_load_program(_bus *bus) {
	bus->ram_prgm_size = bus->rom_prgm_size - bus->ram_prgm_size;
	if (bus->ram_prgm_size > PRGM_MSIZE)
		bus->ram_prgm_size = PRGM_MSIZE;
	memcpy(bus->ram + PRGM_START, bus->rom + bus->bank_position, bus->ram_prgm_size);
	bus->bank_position += bus->ram_prgm_size;
}

/// / //	STACK

void	stack_push(_6502 *mos6502, uint8_t val) {
	if (!mos6502->SP)
		mos6502->SP = 0xFF;
	mos6502->SP--;
	mos6502->bus->write(mos6502->bus->ram, STACK_START + mos6502->SP, val);
}

uint8_t	stack_pull(_6502 *mos6502) {
	uint8_t	val = mos6502->bus->read(mos6502->bus->ram, STACK_START + mos6502->SP);
	// TODO: remove
	mos6502->bus->write(mos6502->bus->ram, STACK_START + mos6502->SP, 0x0);

	mos6502->SP++;
	if (mos6502->SP > 0xFF)
		mos6502->SP = 0x0;
	return val;
}

// /////	STATUS FLAGS

void	set_flag(_6502 *mos6502, uint8_t pos, uint8_t bit) {
	uint8_t	sr = mos6502->SR;
	switch (pos) {
		case 'N':	bit ? (sr|=0x80) : (sr^=0x80); break;
		case 'V': bit ? (sr|=0x40) : (sr^=0x40); break;
		case 'B':	bit ? (sr|=0x10) : (sr^=0x10); break;
		case 'D':	bit ? (sr|=0x8) : (sr^=0x8); break;
		case 'I':	bit ? (sr|=0x4) : (sr^=0x4); break;
		case 'Z':	bit ? (sr|=0x2) : (sr^=0x2); break;
		case 'C':	bit ? (sr|=0x1) : (sr^=0x1); break;
		default: bit ? (sr|0x20) : (sr^=0x20); break;
		         // who knows what that 5th bit
		         // can do
	}
	mos6502->SR = sr;
}

uint8_t	get_flag(_6502* mos6502, uint8_t pos) {
	uint8_t	sr = mos6502->SR;
	switch (pos) {
		case 'N': return (sr >> 0x7) & 0x1;
		case 'V': return (sr >> 0x6) & 0x1;
		case 'B': return (sr >> 0x4) & 0x1;
		case 'D': return (sr >> 0x3) & 0x1;
		case 'I': return (sr >> 0x2) & 0x1;
		case 'Z': return (sr >> 0x1) & 0x1;
		case 'C': return sr & 0x1;
		default:	return (sr >> 0x5) & 0x1;
	}
}

/// // /	RESET

void	cpu_init(_6502* mos6502) {
	_bus *bus = mos6502->bus;
	uint8_t *ram = bus->ram;
	mos6502->load_ROM = cpu_load_program;
	mos6502->pull = stack_pull;
	mos6502->push = stack_push;
	mos6502->set_flag = set_flag;
	mos6502->get_flag = get_flag;
	load_instructions(mos6502);
	mos6502->instruction_cycle = instruction_cycle;
	mos6502->PC = bus->read(ram, RSTV) << 8 |
		bus->read(ram, RSTV+1);
	mos6502->opcode = 0x0;
	mos6502->cycles = 0x0;
	mos6502->SP = 0xFF;
	mos6502->SR = 0x0;
	mos6502->A = 0x0;
	mos6502->X = 0x0;
	mos6502->Y = 0x0;
	mos6502->set_flag(mos6502, 'I', 1);
	mos6502->set_flag(mos6502, 'B', 1);
}
