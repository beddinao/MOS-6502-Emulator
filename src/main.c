#include "mos6502.h"

uint8_t	read_(uint8_t *ram, uint16_t addr) {
	if (addr >= 0x000 && addr <= RAM_SIZE)
		return ram[addr];
	return 0x00;
}

void	write_(uint8_t *ram, uint16_t addr, uint8_t val) {
	if (addr >= 0x0000 && addr <= RAM_SIZE)
		ram[addr] = val;
}

void	init_cpu(_6502* mos6502) {
	load_instructions(mos6502);

	mos6502->bus->read = read_;
	mos6502->bus->write = write_;
	mos6502->PC = PROGRAM_START;
	mos6502->SP = STACK_START;
}

int	main() {
	srand(time(0));

	_6502	*mos6502 = malloc(sizeof(_6502));
	if (!mos6502) 
		return 1;
	memset(mos6502, 0, sizeof(_6502));

	_bus	*bus = malloc(sizeof(_bus));
	if (!bus) {
		free(mos6502);
		return 1;
	}
	memset(bus, 0, sizeof(bus));

	mos6502->bus = bus;
	init_cpu(mos6502);

	/// / //		CYCLE
	instruction_cycle(mos6502, ram);

	free(mos6502->bus);
	free(mos6502);
}
