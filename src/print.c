#include <mos6502.h> 

void	convert_hex_str(unsigned num, char *res, unsigned res_size) {
	unsigned num_size = 0;

	if (!num)
		num_size = 2;
	else
		for (unsigned temp_num = num; temp_num;) {
			num_size++;
			temp_num /= 16;
		}
	if (num_size < 2)
		num_size = 2;
	memset(res, 0, res_size);
	if (!num) 
		res[--num_size] = '0';
	else
		for (; num; ) {
			res[--num_size] = "0123456789ABCDEF"[(num % 16)];
			num /= 16;
		}
	if (num_size)
		res[0] = '0';
}

unsigned	print_field(unsigned x, unsigned y, unsigned num, char *str, char *res) {
	unsigned str_len = strlen(str);
	mvaddstr(y, x, str);
	convert_hex_str(num, res, 1024);
	mvaddstr(y, x + str_len, res);
	return x + str_len + strlen(res);
}

void	*print_state(void *p) {
	_worker	*thread_data = (_worker*)p;
	_6502	*mos6502 = thread_data->mos6502;
	unsigned	program_start, program_end, stack_start;
	char	res[1024];
	unsigned	x_index, y_index;

	initscr();
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	unsigned	width = COLS / 4,
		x_start = width / 2;
	while (1) {
		y_index = 1;
		pthread_mutex_lock(&thread_data->halt_mutex);
		if (thread_data->halt) {
			pthread_mutex_unlock(&thread_data->halt_mutex);
			break;
		}
		pthread_mutex_unlock(&thread_data->halt_mutex);
		pthread_mutex_lock(&thread_data->data_mutex);
		/// / //	REGISTERS
		mvaddstr(0, 0, "PC:");
		attron(COLOR_PAIR(2));
		print_field(3, 0, mos6502->PC, " ", res);
		attroff(COLOR_PAIR(2));
		attron(COLOR_PAIR(1));
		print_field(0, y_index++, mos6502->A, "A: ", res);
		print_field(0, y_index++, mos6502->X, "X: ", res);
		print_field(0, y_index++, mos6502->Y, "Y: ", res);
		print_field(0, y_index++, mos6502->SP, "SP: ", res);
		mvaddstr(y_index, 0, "SR: ");
		mvaddstr(y_index, 4, mos6502->get_flag(mos6502, 'N') ? "1" : "0");
		mvaddstr(y_index, 6, mos6502->get_flag(mos6502, 'V') ? "1 - " : "0 - ");
		mvaddstr(y_index, 10, mos6502->get_flag(mos6502, 'B') ? "1" : "0");
		mvaddstr(y_index, 12, mos6502->get_flag(mos6502, 'D') ? "1" : "0");
		mvaddstr(y_index, 14, mos6502->get_flag(mos6502, 'I') ? "1" : "0");
		mvaddstr(y_index, 16, mos6502->get_flag(mos6502, 'Z') ? "1" : "0");
		mvaddstr(y_index++, 18, mos6502->get_flag(mos6502, 'C') ? "1" : "0");
		mvaddstr(y_index, 4, "N V - B D I Z C");
		y_index += 2;
		print_field(0, y_index, mos6502->opcode, "opcode: ", res);
		y_index += 2;
		// / ///	ROM DUMP HEADER
		program_start = mos6502->PC - 0xFF;
		program_end = mos6502->PC + 0xFF;
		if (program_end > PRGM_START + mos6502->bus->ram_prgm_size
		|| program_start > PRGM_START + mos6502->bus->ram_prgm_size) {
			program_start = PRGM_START;
			program_end = program_start + 0xFF;
		}
		x_index = print_field(x_start, y_index, PRGM_START, "ROM (", res);
		x_index = print_field(x_index, y_index, PRGM_START + mos6502->bus->ram_prgm_size, "-", res);
		x_index = print_field(x_index, y_index, mos6502->bus->ram_prgm_size, ") ROM-size ", res);
		x_index = print_field(x_index, y_index, mos6502->bus->bank_position, "(bank ", res);
		x_index = print_field(x_index, y_index, mos6502->bus->rom_prgm_size, ")/", res);
		mvaddstr(y_index++, x_index, ",");
		x_index = print_field(x_start, y_index, program_start, "ROM-part (", res);
		x_index = print_field(x_index, y_index, program_end, " -> ", res);
		mvaddstr(y_index++, x_index, "):");
		/// / //	ROM DUMP
		for (unsigned ram_addr = program_start, color_mode = 1; ram_addr < program_end; ram_addr += width) {
			x_index = print_field(x_start, y_index, ram_addr, "", res);
			mvaddstr(y_index, x_index++, ":");
			for (unsigned col = 0; col < width && ram_addr + col < program_end; col++) {
				color_mode = (ram_addr + col == mos6502->PC) ? 2 : 1;
				attron(COLOR_PAIR(color_mode));
				x_index = print_field(x_index, y_index, mos6502->bus->ram[ram_addr + col], " ", res);
				attroff(COLOR_PAIR(color_mode));
			}
			y_index++;
		}
		y_index++;
		// /// /	STACK HEADER
		stack_start = (STACK_START + mos6502->SP) - 0x32;
		x_index = print_field(x_start, y_index, stack_start, "Stack-part(", res);
		x_index = print_field(x_index, y_index, STACK_END, " - ", res);
		mvaddstr(y_index++, x_index, "):");
		/// / //	STACK DUMP
		for (unsigned ram_addr = stack_start, color_mode = 1; ram_addr < STACK_END; ram_addr += width) {
			x_index = print_field(x_start, y_index, ram_addr, "", res);
			mvaddstr(y_index, x_index++, ":");
			for (unsigned col = 0; col < width && ram_addr + col < STACK_END; col++) {
				color_mode = (ram_addr + col == (unsigned)(mos6502->SP + STACK_START)) ? 2 : 1;
				attron(COLOR_PAIR(color_mode));
				x_index = print_field(x_index, y_index, mos6502->bus->ram[ram_addr + col], " ", res);
				attroff(COLOR_PAIR(color_mode));
			}
			y_index++;
		}
		pthread_mutex_unlock(&thread_data->data_mutex);
		attroff(COLOR_PAIR(1));
		refresh();
		erase();
		usleep(FRAME_RATE);
	}
	endwin();
	return 0;
}

