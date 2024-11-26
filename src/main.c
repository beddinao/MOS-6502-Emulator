#include "mos6502.h"

uint8_t	read_(uint8_t *ram, uint16_t addr) {
	return ram[addr];
}

void	write_(uint8_t *ram, uint16_t addr, uint8_t val) {
	ram[addr] = val;
}

void	reset_(_bus *bus){
	memset(bus->ram, 0, sizeof(bus->ram));
	memset(bus->rom, 0, sizeof(bus->rom));
	bus->ram_program_size = 0;
	bus->rom_program_size = 0;
	bus->bank_position = 0;
	bus->read = read_;
	bus->write = write_;
}

void	init_cpu(_6502* mos6502){
	mos6502->PC = PROGRAM_START;
	mos6502->SP = STACK_SIZE;
	mos6502->ST = 0x0;
	mos6502->A = 0x0;
	mos6502->X = 0x0;
	mos6502->Y = 0x0;
	mos6502->opcode = 0x0;
	mos6502->cycles = 0x0;
}

uint8_t	load_ROM(_bus *bus, char *filename) {
	unsigned char buffer[MAX_PROGRAM_SIZE];
	unsigned chars_read;
	FILE *file = fopen(filename, "rb");
	if (!file) 
		return 0;
	/// / //	LOADING PROGRAM TO ROM
	memset(buffer, 0, sizeof(buffer));
	while ((chars_read = fread(buffer, 1, sizeof(buffer), file)) != 0) {
		if (chars_read + bus->rom_program_size > ROM_SIZE) {
			fclose(file);
			return 0;
		}
		memcpy(bus->rom + bus->rom_program_size, buffer, chars_read);
		bus->rom_program_size += chars_read;
		memset(buffer, 0, sizeof(buffer));
	}
	fclose(file);
	if (!bus->rom_program_size) 
		return 0;
	/// / //	LOADING PROGRAM TO RAM
	bus->ram_program_size = bus->rom_program_size;
	if (bus->ram_program_size > MAX_PROGRAM_SIZE)
		bus->ram_program_size = MAX_PROGRAM_SIZE;
	memcpy(bus->ram + PROGRAM_START, bus->rom, bus->ram_program_size);
	bus->bank_position = bus->ram_program_size;
	/*printf("ram_psize: %u; rom_psize: %u, bank_p: %u\n", 
		bus->ram_program_size,
		bus->rom_program_size,
		bus->bank_position);*/
	return 1;
}

int	main(int c, char **v) {
	if (c != 2) {
		printf("usage ./mos6502 [ROM]\n");
		return 1;
	}
	srand(time(0));

	_6502	*mos6502 = malloc(sizeof(_6502));
	if (!mos6502) 
		return 1;
	memset(mos6502, 0, sizeof(_6502));
	mos6502->reset = init_cpu;
	mos6502->reset(mos6502);

	_bus	*bus = malloc(sizeof(_bus));
	if (!bus) {
		free(mos6502);
		return 1;
	}
	memset(bus, 0, sizeof(_bus));
	bus->reset = reset_;
	bus->reset(bus);

	if (!load_ROM(bus, v[1])) {
		printf("failed to load program to memory\n");
		free(mos6502);
		free(bus);
		return 1;
	}

	mos6502->bus = bus;

	load_instructions(mos6502);

	/// / //		CYCLE
	instruction_cycle(mos6502);

	free(mos6502->bus);
	free(mos6502);
}
