#include "mos6502.h"
#include <unistd.h>
/*
   (uint16_t) opcode
   - (0-4)   opcode & 0xF000 |
   - (4-8)   opcode & 0x0F00 >> 8 |
   - (8-12)  opcode & 0x00F0 >> 4 |
   - (12-16) opcode & 0x000F
*/

void print_state(_6502 *mos6502) {
	printf("\e[1;1H\e[2J\n");

	printf("PC: %s%04X%s\nAR: %02X\nXR: %02X\nYR: %02X\nSP: %s%02X%s\nSR: %02X\n",
	KRED, mos6502->PC, KNRM, mos6502->A, mos6502->X, mos6502->Y, KRED, mos6502->SP, KNRM, mos6502->ST);

	printf("\nopcode: %02X\n", mos6502->opcode);

	unsigned program_start = mos6502->PC - 0xFF;
	unsigned program_end = mos6502->PC + 0xFF;

	if (program_end > PROGRAM_START + mos6502->bus->ram_program_size)
		program_end = PROGRAM_START + mos6502->bus->ram_program_size;

	printf("\nROM (%04X-%04X) ROM-size %04X(bank %04X)/%04X,\n%sROM-part (%04X -> %04X)%s:\n",
		PROGRAM_START, PROGRAM_START + mos6502->bus->ram_program_size,
		mos6502->bus->ram_program_size, mos6502->bus->bank_position, mos6502->bus->rom_program_size,
		KUND, program_start, program_end, KNRM);

	for (unsigned ram_addr = program_start; ram_addr < program_end; ram_addr += 16) {
		printf("%s%04X%s ", KUND, ram_addr, KNRM);
		for (unsigned col = 0; col < 16 && ram_addr + col < program_end; col++) 
			printf("%s%02X%s ", (ram_addr + col == mos6502->PC ? KRED : KWHT), mos6502->bus->ram[ram_addr + col], KNRM);
		printf("\n");
	}

	unsigned stack_start = (STACK_START + mos6502->SP) - 0x32;
	printf("\n%sStack-part(%04X - %04X)%s:\n", KUND, stack_start, STACK_END, KNRM);
	for (unsigned ram_addr = stack_start; ram_addr < STACK_END; ram_addr += 16) {
		printf("%s%04X%s ",KUND, ram_addr, KNRM);
		for (unsigned col = 0; col < 16 && ram_addr + col < STACK_END; col++) 
			printf("%s%02X %s", (ram_addr + col == mos6502->SP + STACK_START ? KRED : KWHT), mos6502->bus->ram[ram_addr + col], KNRM);
		printf("\n");
	}
}


// /// /	CYCLE

void	instruction_cycle(void *p) {
	_6502	*mos6502 = (_6502*)p;
	_bus	*bus = mos6502->bus;
	uint8_t	*ram = bus->ram;
	unsigned	print_counter = 0;

	while (1) {
		if (mos6502->cycles) {
			mos6502->cycles--;
			continue;
		}
		mos6502->opcode = bus->read(ram, mos6502->PC);
		mos6502->cycles = mos6502->instructions[mos6502->opcode](mos6502);
		
		if (mos6502->PC > PROGRAM_START + bus->ram_program_size
				&& bus->bank_position < bus->rom_program_size) {
			memset(ram + PROGRAM_START, 0, bus->ram_program_size);
			mos6502->PC -= bus->ram_program_size;
			bus->ram_program_size = bus->rom_program_size - bus->ram_program_size;
			if (bus->ram_program_size > MAX_PROGRAM_SIZE)
				bus->ram_program_size = MAX_PROGRAM_SIZE;
			memcpy(ram + PROGRAM_START, bus->rom + bus->bank_position, bus->ram_program_size);
			bus->bank_position += bus->ram_program_size;
		}

		if (mos6502->PC > PROGRAM_START + bus->ram_program_size
				|| mos6502->PC < PROGRAM_START) {
			break;
		}

		print_state(mos6502);
		usleep(10);
	}
}
