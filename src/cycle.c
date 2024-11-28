#include "mos6502.h"

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
		mos6502->cycles = mos6502->opcodes[mos6502->opcode](mos6502);
		
		if (mos6502->PC > PRGM_START + bus->ram_prgm_size
				&& bus->bank_position < bus->rom_prgm_size) {
			memset(ram + PRGM_START, 0, bus->ram_prgm_size);
			mos6502->PC -= bus->ram_prgm_size;
			mos6502->load_ROM(bus);
		}

		if (mos6502->PC > PRGM_START + bus->ram_prgm_size
				|| mos6502->PC < PRGM_START)
			break;

		print_state(mos6502);
		usleep(70000);
	}
}
