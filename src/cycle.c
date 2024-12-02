#include "mos6502.h"

// /// /	CYCLE

void	instruction_cycle(void *p) {
	_worker	*thread_data = (_worker*)p;
	_6502	*mos6502 = thread_data->mos6502;
	_bus	*bus = mos6502->bus;
	uint8_t	*ram = bus->ram;

	while (1) {
		if (mos6502->cycles) {
			mos6502->cycles--;
			continue;
		}
		pthread_mutex_lock(&thread_data->data_mutex);
		mos6502->opcode = bus->read(ram, mos6502->PC);
		mos6502->cycles = mos6502->opcodes[mos6502->opcode](mos6502);
		if (mos6502->PC > PRGM_START + bus->ram_prgm_size
				&& bus->bank_position < bus->rom_prgm_size) {
			memset(ram + PRGM_START, 0, bus->ram_prgm_size);
			mos6502->PC -= bus->ram_prgm_size;
			mos6502->load_ROM(bus);
		}
		pthread_mutex_unlock(&thread_data->data_mutex);
	}
}
