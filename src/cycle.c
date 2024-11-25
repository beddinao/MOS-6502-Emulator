#include "mos6502.h"

/*
   (uint16_t) opcode
   - (0-4)   opcode & 0xF000 |
   - (4-8)   opcode & 0x0F00 >> 8 |
   - (8-12)  opcode & 0x00F0 >> 4 |
   - (12-16) opcode & 0x000F
   */

// /// /	CYCLE

void	instruction_cycle(void *p) {
	_6502	*mos6502 = (_6502*)p;
	_bus	*bus = mos6502->bus;
	uint8_t	*ram = bus->ram;

	while (1) {
		if (mos6502->cycles) {
			mos6502->cycles--;
			continue;
		}
		mos6502->opcode = bus->read(ram, mos6502->PC);
		mos6502->cycles = mos6502->instructions[mos6502->opcode](mos6502);
		mos6502->PC += 1;

		if (mos6502->PC > PROGRAM_START + bus->ram_program_size
		&& bus->ram_program_size < bus->rom_program_size) {
			memset(ram + PROGRAM_START, 0, bus->ram_program_size);
			bus->ram_program_size = bus->rom_program_size - bus->ram_program_size;
			if (bus->ram_program_size > MAX_PROGRAM_SIZE)
				bus->ram_program_size = MAX_PROGRAM_SIZE;
			memcpy(ram + PROGRAM_START, bus->rom + bus->bank_position, bus->ram_program_size);
			bus->bank_position += bus->ram_program_size;
			mos6502->PC = PROGRAM_START;
		}

		if (mos6502->PC > PROGRAM_START + bus->ram_program_size
			|| mos6502->PC < PROGRAM_START) {
			break;
		}
	}
}
