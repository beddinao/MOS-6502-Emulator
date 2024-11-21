#include "mos6502.h"

// //// /		DRAWING UTILS

void	draw_background(WIN *window, unsigned color) {
	for (unsigned y = 0; y < window->height; y++)
		for (unsigned x = 0; x < window->width; x++)
			mlx_put_pixel(window->mlx_img, x, y, color);
}

unsigned	__calc_new_range(unsigned old_value,
		unsigned old_min, unsigned old_max,
		unsigned new_min, unsigned new_max) {
	if (old_value == old_min)
		return(new_min);
	return (((old_value - old_min) * (new_max - new_min)) / (old_max - old_min)) + new_min;
}

void	render_display(void *p) {
	CHIP8* chip8_data = (CHIP8*)p;
	pthread_mutex_lock(&chip8_data->state_mutex);
	if (chip8_data->emu_state) {
		pthread_mutex_unlock(&chip8_data->state_mutex);
		close_hook(chip8_data);
	}
	pthread_mutex_unlock(&chip8_data->state_mutex);
	unsigned scale_x = chip8_data->window->width / DIS_W;
	unsigned scale_y = chip8_data->window->height / DIS_H;
	uint32_t color;
	for (unsigned y = 0; y < DIS_H; y++)
		for (unsigned x = 0; x < DIS_W; x++) {
			pthread_mutex_lock(&chip8_data->display_mutex);
			color = chip8_data->display[y * DIS_W + x] ? 0xFFFFFF : 0x000000;
			pthread_mutex_unlock(&chip8_data->display_mutex);
			for (unsigned sy = 0; sy < scale_y; sy++)
				for (unsigned sx = 0; sx < scale_x; sx++)
					mlx_put_pixel(chip8_data->window->mlx_img, x*scale_x+sx, y*scale_y+sy, color<<8|0xFF);
		}
}

/// / ////	HOOKS

void	close_hook(void *p) {
	CHIP8 *chip8_data = (CHIP8*)p;
	mlx_terminate(chip8_data->window->mlx_ptr);
	pthread_mutex_lock(&chip8_data->state_mutex);
	chip8_data->emu_state = 1;
	pthread_mutex_unlock(&chip8_data->state_mutex);
	pthread_join(chip8_data->worker, NULL);
	pthread_mutex_destroy(&chip8_data->display_mutex);
	pthread_mutex_destroy(&chip8_data->state_mutex);
	pthread_mutex_destroy(&chip8_data->keys_mutex);
	free(chip8_data->window);
	free(chip8_data);
	exit(0);
}

void	key_hook(mlx_key_data_t keydata, void *p) {
	if (keydata.action != MLX_PRESS)	return;

	CHIP8* chip8_data = (CHIP8*)p;
	pthread_mutex_lock(&chip8_data->keys_mutex);
	if (keydata.key == MLX_KEY_ESCAPE) {
		pthread_mutex_unlock(&chip8_data->keys_mutex);
		close_hook(p);
	}
	else if (keydata.key >= '0' && keydata.key <= '9')
		chip8_data->keys[keydata.key - '0'] = KEY_PRESS_CYCLES;
	else if (keydata.key >= MLX_KEY_KP_0 && keydata.key <= MLX_KEY_KP_9) {
		uint8_t i;
		switch (keydata.key) {
			case MLX_KEY_KP_1: i = 7; break;
			case MLX_KEY_KP_2: i = 8; break;
			case MLX_KEY_KP_3: i = 9; break;
			case MLX_KEY_KP_7: i = 1; break;
			case MLX_KEY_KP_8: i = 2; break;
			case MLX_KEY_KP_9: i = 3; break;
			default: i = keydata.key - MLX_KEY_KP_0;
		}
		chip8_data->keys[ i ] = KEY_PRESS_CYCLES;
	}
	else if (keydata.key >= 'A' && keydata.key <= 'F')
		chip8_data->keys[(keydata.key - 'A') + 10] = KEY_PRESS_CYCLES;
	else if (keydata.key >= MLX_KEY_KP_DECIMAL && keydata.key <= MLX_KEY_KP_EQUAL) {
		uint8_t i;
		switch (keydata.key) {
			case MLX_KEY_KP_DECIMAL: i = 0xA; break;
			case MLX_KEY_KP_ENTER: i = 0xB; break;
			case MLX_KEY_KP_ADD: i = 0xC; break;
			case MLX_KEY_KP_SUBTRACT: i = 0xD; break;
			case MLX_KEY_KP_MULTIPLY: i = 0xE; break;
			case MLX_KEY_KP_DIVIDE: i = 0xF; break;
			default: i = 0xF;
		}
		chip8_data->keys[ i ] = KEY_PRESS_CYCLES;
	}
	pthread_mutex_unlock(&chip8_data->keys_mutex);
}

void	resize_hook(int width, int height, void *p) {
	CHIP8 *chip8_data = (CHIP8*)p;
	unsigned	valid = 0;

	if (height > MIN_HEIGHT) {
		chip8_data->window->height = height;
		valid = 1;
	}
	if (width > MIN_WIDTH) {
		chip8_data->window->width = width;
		valid = 1;
	}

	if (valid) {
		if (!mlx_resize_image(chip8_data->window->mlx_img,
				chip8_data->window->width,
				chip8_data->window->height))
			close_hook(p);
		render_display(chip8_data);
	}
}

//// / //	/	INIT

int	init_window(CHIP8 *chip8_data, char *ROM) {
	WIN *window = chip8_data->window;
	window->height = DEF_HEIGHT;
	window->width = DEF_WIDTH;
	window->mlx_ptr = mlx_init(window->width, window->height, ROM, true);
	if (!window->mlx_ptr)
		return 0;
	window->mlx_img = mlx_new_image(window->mlx_ptr, window->width, window->height);
	if (!window->mlx_img) {
		free(window->mlx_ptr);
		return 0;
	}
	draw_background(window, 0x000000FF);
	mlx_image_to_window(window->mlx_ptr, window->mlx_img, 0, 0);
	mlx_resize_hook(chip8_data->window->mlx_ptr, resize_hook, chip8_data);
	mlx_close_hook(window->mlx_ptr, close_hook, chip8_data);
	mlx_key_hook(window->mlx_ptr, key_hook, chip8_data);
	return 1;
}
