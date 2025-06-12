#include <mos6502.h> 

// /// /	CYCLE

void	*instruction_cycle(void *p) {
	_worker	*thread_data = (_worker*)p;
	_6502	*mos6502 = thread_data->mos6502;
	_bus	*bus = mos6502->bus;
	uint8_t	*ram = bus->ram;

	/*unsigned pc, chars_read;
	FILE *file = fopen("./ROMS/tests/bin_files/6502_functional_test.bin", "rb");
	if (!file) exit(1);
	char buffer[0x10000];
	memset(buffer, 0, sizeof(buffer));
	chars_read = fread(buffer, 1, 0x10000, file);
	if (!chars_read) exit(1);
	memcpy(bus->ram + 0x400, buffer, chars_read);
	mos6502->PC = 0x400;*/

	while (1) {

		pthread_mutex_lock(&thread_data->halt_mutex);
		if (thread_data->halt) {
			pthread_mutex_unlock(&thread_data->halt_mutex);
			break;	
		}
		pthread_mutex_unlock(&thread_data->halt_mutex);

		if (mos6502->cycles) {
			mos6502->cycles--;
			continue;
		}
		pthread_mutex_lock(&thread_data->data_mutex);
		//pc = mos6502->PC;
		mos6502->opcode = bus->read(ram, mos6502->PC);
		//printf("PC %04X: OP %02X -> ", mos6502->PC, mos6502->opcode);
		/*printf("%04X  %02X %02X %02X  ",
		   mos6502->PC, mos6502->opcode, bus->read(bus->ram, mos6502->PC+1), bus->read(bus->ram, mos6502->PC+2));
		   printf("\tA:%02X X:%02X Y:%02X P:%02X SP:%02X\t",
		   mos6502->A, mos6502->X, mos6502->Y, mos6502->SR, mos6502->SP);*/
		mos6502->cycles = mos6502->opcodes[mos6502->opcode](mos6502);
		//printf("\n");
		/*printf("A: %02X, X: %02X, Y: %02X, SP: %02X, SR: %02X\n",
			mos6502->A, mos6502->X, mos6502->Y, mos6502->SP, mos6502->SR);*/
		/*if (pc == mos6502->PC) {
			printf("infinite loop: %04X\n", pc);
			exit(1);
		}
		if (mos6502->PC == 0x3463) {
			printf("test passed\n");
			exit(0);
		}*/
		if (mos6502->PC > PRGM_START + bus->ram_prgm_size
				&& bus->bank_position < bus->rom_prgm_size) {
			memset(ram + PRGM_START, 0, bus->ram_prgm_size);
			mos6502->PC -= bus->ram_prgm_size;
			mos6502->load_ROM(bus);
		}
		pthread_mutex_unlock(&thread_data->data_mutex);
		// slowing down the cpu for debugging
		//usleep(1000000);
	}
	return 0;
}
