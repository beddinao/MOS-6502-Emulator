#include "mos6502.h"

/*
   (uint16_t) opcode
   - (0-4)   opcode & 0xF000 |
   - (4-8)   opcode & 0x0F00 >> 8 |
   - (8-12)  opcode & 0x00F0 >> 4 |
   - (12-16) opcode & 0x000F
   */

// /// /	CYCLE

void	*instruction_cycle(void *p) {
	uint16_t	instruction, valid_instruction;
	CHIP8 *chip8_data = (CHIP8*)p;
	chip8_data->PC = MEMORY_START;
	struct timeval f_time, s_time;
	memset(&f_time, 0, sizeof(f_time));
	memset(&s_time, 0, sizeof(s_time));

	gettimeofday(&f_time, NULL);
	while (1) {
		pthread_mutex_lock(&chip8_data->state_mutex);
		if (chip8_data->emu_state) {
			pthread_mutex_unlock(&chip8_data->state_mutex);
			return 0;
		}
		pthread_mutex_unlock(&chip8_data->state_mutex);

		gettimeofday(&s_time, NULL);
		if (abs((int)s_time.tv_usec - (int)f_time.tv_usec) > TIME_DIFF)
			gettimeofday(&f_time, NULL);
		else	continue;

		chip8_data->DT -= (chip8_data->DT ? 1 : 0);
		chip8_data->ST -= (chip8_data->ST ? 1 : 0);

		valid_instruction = 1;
		pthread_mutex_lock(&chip8_data->keys_mutex);
		for (unsigned i = 0; i <= 0xF; i++)
			if (chip8_data->keys[i]) {
				if (chip8_data->halt) {
					chip8_data->registers[chip8_data->key_register] = i;
					chip8_data->keys[i] = 0;
					chip8_data->halt = 0;
					chip8_data->PC += 2;
					valid_instruction = 0;
					break;
				}
				else	chip8_data->keys[i]--;
			}
		pthread_mutex_unlock(&chip8_data->keys_mutex);

		if (chip8_data->halt || !valid_instruction)
			continue;

		chip8_data->opcode = (chip8_data->RAM[ chip8_data->PC ] << 8 | chip8_data->RAM[ chip8_data->PC + 1 ]);
		chip8_data->PC += 2;

		instruction = chip8_data->opcode & 0xF000;

		if (instruction == 0x0000)
			switch (chip8_data->opcode & 0x00FF) {
				case 0x00E0: _00E0(chip8_data); break;
				case 0x00EE: _00EE(chip8_data); break;
				default: valid_instruction = 0;
			}
		else if (instruction >= 0x1000 && instruction <= 0x7000)
			chip8_data->_0_7_set[(instruction >> 0xC) - 0x1](chip8_data);
		else if (instruction == 0x8000) {
			uint8_t n = chip8_data->opcode & 0x000F;
			if (n <= 0x7 || n == 0xE)
				chip8_data->_8s_set[n](chip8_data);
			else	valid_instruction = 0;
		}
		else if (instruction >= 0x9000 && instruction <= 0xD000)
			chip8_data->_9_D_set[(instruction >> 0xC) - 0x9](chip8_data);
		else if (instruction == 0xE000)
			switch (chip8_data->opcode & 0x00FF) {
				case 0x009E: chip8_data->_Es_set[0](chip8_data); break;
				case 0x00A1: chip8_data->_Es_set[1](chip8_data); break;
				default: valid_instruction = 0;
			}
		else if (instruction == 0xF000)
			switch (chip8_data->opcode & 0x00FF) {
				case 0x0007: chip8_data->_Fs_set[0](chip8_data); break;
				case 0x000A: chip8_data->_Fs_set[1](chip8_data); break;
				case 0x0015: chip8_data->_Fs_set[2](chip8_data); break;
				case 0x0018: chip8_data->_Fs_set[3](chip8_data); break;
				case 0x001E: chip8_data->_Fs_set[4](chip8_data); break;
				case 0x0029: chip8_data->_Fs_set[5](chip8_data); break;
				case 0x0033: chip8_data->_Fs_set[6](chip8_data); break;
				case 0x0055: chip8_data->_Fs_set[7](chip8_data); break;
				case 0x0065: chip8_data->_Fs_set[8](chip8_data); break;
				default: valid_instruction = 0;
			}
		else	valid_instruction = 0;

		if (!valid_instruction
				|| chip8_data->PC > MEMORY_SIZE
				|| chip8_data->PC > chip8_data->memory_occupied + MEMORY_START
				|| chip8_data->PC < MEMORY_START) {
			pthread_mutex_lock(&chip8_data->state_mutex);
			chip8_data->emu_state = 1;
			pthread_mutex_unlock(&chip8_data->state_mutex);
			break;
		}
	}
	return 0;
}
