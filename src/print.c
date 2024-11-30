#include "mos6502.h"

void	print_state(_6502 *mos6502) {
	unsigned program_start, program_end,
	         stack_start, i_flag = mos6502->get_flag(mos6502, 'I');

	printf("\e[1;1H\e[2J\n");
	printf("PC: %s%04X%s\nA: %02X\nX: %02X\nY: %02X\nSP: %s%02X%s\n",
	RED, mos6502->PC, RST, mos6502->A, mos6502->X, mos6502->Y, RED, mos6502->SP, RST);
	printf("SR: %s%i %i %i %i %i %i %i %i%s\n    N V - B D I Z C\n", WHT, mos6502->get_flag(mos6502, 'N'), mos6502->get_flag(mos6502, 'V'),
		0, mos6502->get_flag(mos6502, 'B'), mos6502->get_flag(mos6502, 'D'), i_flag,
		mos6502->get_flag(mos6502, 'Z'), mos6502->get_flag(mos6502, 'C'), RST);
	printf("\nopcode: %02X\n", mos6502->opcode);
	program_start = mos6502->PC - 0xFF;
	program_end = mos6502->PC + 0xFF;
	if (program_end > PRGM_START + mos6502->bus->ram_prgm_size)
		program_end = PRGM_START + mos6502->bus->ram_prgm_size;
	printf("\nROM (%04X-%04X) ROM-size %04X(bank %04X)/%04X,\n%sROM-part (%04X -> %04X)%s:\n",
		PRGM_START, PRGM_START + mos6502->bus->ram_prgm_size,
		mos6502->bus->ram_prgm_size, mos6502->bus->bank_position, mos6502->bus->rom_prgm_size,
		UND, program_start, program_end, RST);
	for (unsigned ram_addr = program_start; ram_addr < program_end; ram_addr += 16) {
		printf("%s%04X%s ", UND, ram_addr, RST);
		for (unsigned col = 0; col < 16 && ram_addr + col < program_end; col++) 
			printf("%s%02X%s ", (ram_addr + col == mos6502->PC ? RED : WHT), mos6502->bus->ram[ram_addr + col], RST);
		printf("\n");
	}
	stack_start = (STACK_START + mos6502->SP) - 0x32;
	printf("\n%sStack-part(%04X - %04X)%s:\n", UND, stack_start, STACK_END, RST);
	for (unsigned ram_addr = stack_start; ram_addr < STACK_END; ram_addr += 16) {
		printf("%s%04X%s ", UND, ram_addr, RST);
		for (unsigned col = 0; col < 16 && ram_addr + col < STACK_END; col++) 
			printf("%s%02X %s", (ram_addr + col == mos6502->SP + STACK_START ? RED : WHT), mos6502->bus->ram[ram_addr + col], RST);
		printf("\n");
	}
}


