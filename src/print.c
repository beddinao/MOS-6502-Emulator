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

void	draw_bg(SDL_Renderer *renderer, uint32_t color) {
	SDL_SetRenderDrawColor(renderer,
			(color >> 24) & 0xFF,
			(color >> 16) & 0xFF,
			(color >> 8) & 0xFF,
			color & 0xFF);
	SDL_RenderClear(renderer);
}

void	draw_text(_worker *thread_data, char *text, int x, int y, uint32_t color) {
	int len = strlen(text);
	SDL_Color text_color = { color >> 24 & 0xFF, color >> 16 & 0xFF, color >> 8 & 0xFF, color & 0xFF };
	SDL_Surface *surface = TTF_RenderText_Solid(thread_data->mono_font, text, len, text_color);
	if (surface) {
		SDL_Texture *texture = SDL_CreateTextureFromSurface(thread_data->renderer, surface);
		if (!texture) return;
		SDL_DestroySurface(surface);
		SDL_FRect rect = { x*(PPL/2), y*PPL, len*(PPL/2),  PPL};
		SDL_RenderTexture(thread_data->renderer, texture, NULL, &rect);
		SDL_DestroyTexture(texture);
	}
}

unsigned	print_field(_worker *thread_data, unsigned x, unsigned y, unsigned num, char *str, char *res, uint32_t color) {
	unsigned str_len = strlen(str);
	//mvaddstr(y, x, str);
	draw_text(thread_data, str, x, y, color);
	convert_hex_str(num, res, 1024);
	//mvaddstr(y, x + str_len, res);
	draw_text(thread_data, res, x + str_len, y, color);
	return x + str_len + strlen(res);
}

void	poll_events() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_EVENT_QUIT: sig_handle(0);
			case SDL_EVENT_KEY_DOWN: 
				if (event.key.key == SDLK_ESCAPE)
					sig_handle(0);
				break;
		}
	}
}

void	print_state(void *p) {
	_worker	*thread_data = (_worker*)p;
	_6502	*mos6502 = thread_data->mos6502;
	unsigned	program_start, program_end, stack_start;
	char	res[1024];
	unsigned	x_index, y_index;

	unsigned	width = thread_data->win_width / (PPL*2),
		x_start = width / 2;

	uint32_t	color;

	while (1) {
		y_index = 1;
		draw_bg(thread_data->renderer, 0x000000FF);
		pthread_mutex_lock(&thread_data->data_mutex);
		/// / //	REGISTERS
		poll_events();
		color = 0xFFFFFFFF;
		draw_text(thread_data, "PC:", 0, 0, color);
		color = 0xFF0000FF;
		print_field(thread_data, 3, 0, mos6502->PC, " ", res, color);
		color = 0xFFFFFFFF;
		print_field(thread_data, 0, y_index++, mos6502->A, "A: ", res, color);
		print_field(thread_data, 0, y_index++, mos6502->X, "X: ", res, color);
		print_field(thread_data, 0, y_index++, mos6502->Y, "Y: ", res, color);
		print_field(thread_data, 0, y_index++, mos6502->SP, "SP: ", res, color);
		draw_text(thread_data, "SR: ", 0, y_index, color);
		draw_text(thread_data, mos6502->get_flag(mos6502, 'N') ? "1" : "0", 4, y_index, color);
		draw_text(thread_data, mos6502->get_flag(mos6502, 'V') ? "1 - " : "0 - ", 6, y_index, color);
		draw_text(thread_data, mos6502->get_flag(mos6502, 'B') ? "1" : "0", 10, y_index, color);
		draw_text(thread_data, mos6502->get_flag(mos6502, 'D') ? "1" : "0", 12, y_index, color);
		draw_text(thread_data, mos6502->get_flag(mos6502, 'I') ? "1" : "0", 14, y_index, color);
		draw_text(thread_data, mos6502->get_flag(mos6502, 'Z') ? "1" : "0", 16, y_index, color);
		draw_text(thread_data, mos6502->get_flag(mos6502, 'C') ? "1" : "0", 18, y_index++, color);
		draw_text(thread_data, "N V - B D I Z C", 4, y_index, color);
		y_index += 2;
		print_field(thread_data, 0, y_index, mos6502->opcode, "opcode: ", res, color);
		y_index += 2;
		// / ///	ROM DUMP HEADER
		poll_events();
		program_start = mos6502->PC - 0xFF;
		program_end = mos6502->PC + 0xFF;
		if (program_end > PRGM_START + mos6502->bus->ram_prgm_size
		|| program_start > PRGM_START + mos6502->bus->ram_prgm_size) {
			program_start = PRGM_START;
			program_end = program_start + 0xFF;
		}
		x_index = print_field(thread_data, x_start, y_index, PRGM_START, "ROM (", res, color);
		x_index = print_field(thread_data, x_index, y_index, PRGM_START + mos6502->bus->ram_prgm_size, "-", res, color);
		x_index = print_field(thread_data, x_index, y_index, mos6502->bus->ram_prgm_size, ") ROM-size ", res, color);
		x_index = print_field(thread_data, x_index, y_index, mos6502->bus->bank_position, "(bank ", res, color);
		x_index = print_field(thread_data, x_index, y_index, mos6502->bus->rom_prgm_size, ")/", res, color);
		draw_text(thread_data, ",", x_index, y_index++, color);
		x_index = print_field(thread_data, x_start, y_index, program_start, "ROM-part (", res, color);
		x_index = print_field(thread_data, x_index, y_index, program_end, " -> ", res, color);
		draw_text(thread_data, "):", x_index, y_index++, color);
		/// / //	ROM DUMP
		poll_events();
		for (unsigned ram_addr = program_start, color_mode = 1; ram_addr < program_end; ram_addr += width) {
			x_index = print_field(thread_data, x_start, y_index, ram_addr, "", res, 0xFFFFFFFF);
			draw_text(thread_data, ":", x_index++, y_index, 0xFFFFFFFF);
			for (unsigned col = 0; col < width && ram_addr + col < program_end; col++) {
				color = ram_addr+col == mos6502->PC ? 0xFF0000FF
				: mos6502->bus->ram[ram_addr+col] ? 0xFFFFFFFF : 0xB8B8B8FF;
				x_index = print_field(thread_data, x_index, y_index, mos6502->bus->ram[ram_addr + col], " ", res, color);
			}
			y_index++;
			//poll_events(); // TODO: ?
		}
		y_index++;
		// /// /	STACK HEADER
		poll_events();
		color = 0xFFFFFFFF;
		stack_start = (STACK_START + mos6502->SP) - 0x32;
		x_index = print_field(thread_data, x_start, y_index, stack_start, "Stack-part(", res, color);
		x_index = print_field(thread_data, x_index, y_index, STACK_END, " - ", res, color);
		draw_text(thread_data, "):", x_index, y_index++, color);
		/// / //	STACK DUMP
		poll_events();
		for (unsigned ram_addr = stack_start, color_mode = 1; ram_addr < STACK_END; ram_addr += width) {
			x_index = print_field(thread_data, x_start, y_index, ram_addr, "", res, 0xFFFFFFFF);
			draw_text(thread_data, ":", x_index++, y_index, 0xFFFFFFFF);
			for (unsigned col = 0; col < width && ram_addr + col < STACK_END; col++) {
				color = (ram_addr + col == (unsigned)(mos6502->SP + STACK_START)) ? 0xFF0000FF
				: mos6502->bus->ram[ram_addr+col] ? 0xFFFFFFFF : 0xB8B8B8FF;
				x_index = print_field(thread_data, x_index, y_index, mos6502->bus->ram[ram_addr + col], " ", res, color);
			}
			y_index++;
			//poll_events(); // TODO: ?
		}
		pthread_mutex_unlock(&thread_data->data_mutex);
		SDL_RenderPresent(thread_data->renderer);
		//usleep(FRAME_RATE);
	}
	sig_handle(0);
}

