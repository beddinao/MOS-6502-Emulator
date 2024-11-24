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

void	reset_(_bus *bus){
	memset(bus->ram, 0, sizeof(bus->ram));
	bus->ram_occupied = 0;
}

void	init_cpu(_6502* mos6502){
	mos6502->bus->read = read_;
	mos6502->bus->write = write_;
	mos6502->bus->reset = reset_;
	mos6502->PC = PROGRAM_START;
	mos6502->SP = 0xFF;
	mos6502->ST = 0x0;
	mos6502->A = 0x0;
	mos6502->X = 0x0;
	mos6502->Y = 0x0;
	mos6502->opcode = 0x0;
	mos6502->cycles = 0x0;
	mos6502->bus->reset(mos6502->bus);
}

uint8_t	load_ROM(_6502 *mos6502) {
	return 1;
}

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
	}
}

int	main() {
	if (c != 2) {
		printf("usage ./mos6502 [ROM]\n");
		return 1;
	}
	srand(time(0));

	_6502	*mos6502 = malloc(sizeof(_6502));
	if (!mos6502) 
		return 1;
	memset(mos6502, 0, sizeof(_6502));

	if (!load_ROM(mos6502, v[1])) {
		printf("failed to load program to memory\n");
		free(mos6502);
		return 1;
	}

	_bus	*bus = malloc(sizeof(_bus));
	if (!bus) {
		free(mos6502);
		return 1;
	}
	memset(bus, 0, sizeof(_bus));

	mos6502->bus = bus;
	mos6502->reset = init_cpu;
	mos6502->reset(mos6502);

	load_instructions(mos6502);

	load_ROM(mos6502);

	/// / //		CYCLE
	instruction_cycle(mos6502);

	free(mos6502->bus);
	free(mos6502);
}
