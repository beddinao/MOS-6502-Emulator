#include "mos6502.h"

int	main(int c, char **v) {
	if (c != 2) {
		printf("usage ./mos6502 [ROM]\n");
		return 1;
	}
	srand(time(0));

	_bus	*bus = malloc(sizeof(_bus));
	if (!bus) 
		return 1;
	memset(bus, 0, sizeof(_bus));
	bus->reset = bus_reset;
	bus->reset(bus);

	_6502	*mos6502 = malloc(sizeof(_6502));
	if (!mos6502) { 
		free(bus);
		return 1;
	}
	memset(mos6502, 0, sizeof(_6502));
	mos6502->reset = cpu_reset;
	mos6502->bus = bus;
	mos6502->reset(mos6502);

	if (!load_ROM(bus, v[1])) {
		printf("failed to load program to memory\n");
		free(mos6502);
		free(bus);
		return 1;
	}

	cpu_load_program(bus);
	load_instructions(mos6502);
	/// / //		CYCLE
	instruction_cycle(mos6502);

	free(mos6502);
	free(bus);
}
