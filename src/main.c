#include "mos6502.h"

void	load_instructions(CHIP8* chip8_data) {
	chip8_data->_0_7_set[0] = _1nnn;
	chip8_data->_0_7_set[1] = _2nnn;
	chip8_data->_0_7_set[2] = _3xkk;
	chip8_data->_0_7_set[3] = _4xkk;
	chip8_data->_0_7_set[4] = _5xy0;
	chip8_data->_0_7_set[5] = _6xkk;
	chip8_data->_0_7_set[6] = _7xkk;
	chip8_data->_8s_set[0] = _8xy0;
	chip8_data->_8s_set[1] = _8xy1;
	chip8_data->_8s_set[2] = _8xy2;
	chip8_data->_8s_set[3] = _8xy3;
	chip8_data->_8s_set[4] = _8xy4;
	chip8_data->_8s_set[5] = _8xy5;
	chip8_data->_8s_set[6] = _8xy6;
	chip8_data->_8s_set[7] = _8xy7;
	chip8_data->_8s_set[8] = _8xyE;
	chip8_data->_9_D_set[0] = _9xy0;
	chip8_data->_9_D_set[1] = _Annn;
	chip8_data->_9_D_set[2] = _Bnnn;
	chip8_data->_9_D_set[3] = _Cxkk;
	chip8_data->_9_D_set[4] = _Dxyn;
	chip8_data->_Es_set[0] = _Ex9E;
	chip8_data->_Es_set[1] = _ExA1;
	chip8_data->_Fs_set[0] = _Fx07;
	chip8_data->_Fs_set[1] = _Fx0A;
	chip8_data->_Fs_set[2] = _Fx15;
	chip8_data->_Fs_set[3] = _Fx18;
	chip8_data->_Fs_set[4] = _Fx1E;
	chip8_data->_Fs_set[5] = _Fx29;
	chip8_data->_Fs_set[6] = _Fx33;
	chip8_data->_Fs_set[7] = _Fx55;
	chip8_data->_Fs_set[8] = _Fx65;
}

int	load_to_memory(CHIP8* chip8_data, char *filename) {
	unsigned char buffer[MAX_PROGRAM_SIZE];
	unsigned chars_read;
	FILE *file = fopen(filename, "rb");
	if (!file) 
		return 0;

	memset(buffer, 0, sizeof(buffer));
	while ((chars_read = fread(buffer, 1, sizeof(buffer), file)) != 0) {
		if (chars_read + chip8_data->memory_occupied > MAX_PROGRAM_SIZE) {
			fclose(file);
			return 0;
		}
		memcpy(chip8_data->RAM + (MEMORY_START + chip8_data->memory_occupied), buffer, chars_read);
		chip8_data->memory_occupied += chars_read;
		memset(buffer, 0, sizeof(buffer));
	}
	fclose(file);

	if (!chip8_data->memory_occupied) 
		return 0;

	return 1;
}

void	load_fonts(CHIP8 *chip8_data) {
	uint8_t fonts[FONTS_SIZE] = {
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80, // F
	};
	memcpy(chip8_data->RAM + FONTS_START, fonts, sizeof(fonts));
}

int	main(int c, char **v)
{
	if (c != 2) {
		printf("usage: ./chip8 [program to execute]\n");
		return 1;
	}
	srand(time(0));

	CHIP8	*chip8_data = malloc(sizeof(CHIP8));
	if (!chip8_data) 
		return 1;
	memset(chip8_data, 0, sizeof(CHIP8));

	/// / //		LOADING ROM
	if (!load_to_memory(chip8_data, v[1])) {
		printf("failed to load program to memory\n");
		free(chip8_data);
		return 1;
	}

	// /// /		LOADING FONTS
	load_fonts(chip8_data);

	/// / //		INIT DISPLAY
	chip8_data->window = malloc(sizeof(WIN));
	if (!chip8_data->window || !init_window(chip8_data, v[1])) {
		printf("failed to initialize MLX window\n");
		free(chip8_data);
		return 1;
	}

	/// /// /		LOADING INSTRUCTIONS
	load_instructions(chip8_data);

	/// / //		CYCLE
	pthread_mutex_init(&chip8_data->display_mutex, NULL);
	pthread_mutex_init(&chip8_data->state_mutex, NULL);
	pthread_mutex_init(&chip8_data->keys_mutex, NULL);
	pthread_create(&chip8_data->worker, NULL, instruction_cycle, chip8_data);
	mlx_loop_hook(chip8_data->window->mlx_ptr, render_display, chip8_data);
	mlx_loop(chip8_data->window->mlx_ptr);
}
