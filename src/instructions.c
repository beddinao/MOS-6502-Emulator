#include "mos6502.h"

//// // /		INSTRUCTIONS

//	00E0:CLS
void	_00E0 (CHIP8* chip8_data) {
	memset(chip8_data->display, 0, sizeof(chip8_data->display));
}

//	00EE:RET
void	_00EE (CHIP8* chip8_data) {
	if (chip8_data->SP) {
		chip8_data->PC = chip8_data->stack[0];
		for (unsigned i = 0; i <= chip8_data->SP && i < STACK_DEPTH; i++) {
			if (i == chip8_data->SP||i == STACK_DEPTH - 1)
				chip8_data->stack[i] = 0;
			else	chip8_data->stack[i] = chip8_data->stack[i + 1];
		}
		chip8_data->SP -= 1;
	}
}

//	1NNN:jump addr
void	_1nnn (CHIP8* chip8_data) {
	chip8_data->PC = chip8_data->opcode & 0x0FFF;
}

//	2NNN:call addr
void	_2nnn (CHIP8* chip8_data) {
	if (chip8_data->SP < STACK_DEPTH) {
		for (unsigned i = STACK_DEPTH - 1; i; i--)
			chip8_data->stack[i] = chip8_data->stack[i - 1];
		chip8_data->SP += 1;
		chip8_data->stack[0] = chip8_data->PC;
		chip8_data->PC = chip8_data->opcode & 0x0FFF;
	}
}

//	3XKK:SE Vx, byte
void	_3xkk (CHIP8* chip8_data) {
	uint8_t	reg = (chip8_data->opcode & 0x0F00) >> 8;
	if (reg < 16 && chip8_data->registers[reg] == (chip8_data->opcode & 0x00FF))
		chip8_data->PC += 2;
}

//	4XKK:SNE Vx, byte
void	_4xkk (CHIP8* chip8_data) {
	uint8_t	reg = (chip8_data->opcode & 0x0F00) >> 8;
	if (reg < 16 && chip8_data->registers[reg] != (chip8_data->opcode & 0x00FF))
		chip8_data->PC += 2;
}

//	5XY0:SE Vx, Vy
void	_5xy0 (CHIP8* chip8_data) {
	uint8_t	reg_x = (chip8_data->opcode & 0x0F00) >> 8,
		reg_y = (chip8_data->opcode & 0x00F0) >> 4;
	if (reg_x <= 0xF && reg_y <= 0xF
			&& chip8_data->registers[reg_x] == chip8_data->registers[reg_y])
		chip8_data->PC += 2;
}

//	6XKK:LD Vx, byte
void	_6xkk (CHIP8* chip8_data) {
	uint8_t	reg_x = (chip8_data->opcode & 0x0F00) >> 8;
	if (reg_x <= 0xF)
		chip8_data->registers[reg_x] = chip8_data->opcode & 0x00FF;
}

//	7XKK:ADD Vx, byte
void	_7xkk (CHIP8* chip8_data) {
	uint8_t	reg_x = (chip8_data->opcode & 0x0F00) >> 8;
	if (reg_x <= 0xF)
		chip8_data->registers[reg_x] += chip8_data->opcode & 0x00FF;
}

//	8XY0:LD Vx, Vy
void	_8xy0 (CHIP8* chip8_data) {
	uint8_t	reg_x = (chip8_data->opcode & 0x0F00) >> 8,
		reg_y = (chip8_data->opcode & 0x00F0) >> 4;
	if (reg_y <= 0xF && reg_x <= 0xF)
		chip8_data->registers[reg_x] = chip8_data->registers[reg_y];
}

//	8XY1:OR Vx, Vy
void	_8xy1 (CHIP8* chip8_data) {
	uint8_t	reg_x = (chip8_data->opcode & 0x0F00) >> 8,
		reg_y = (chip8_data->opcode & 0x00F0) >> 4;
	if (reg_x <= 0xF && reg_y <= 0xF)
		chip8_data->registers[reg_x] |= chip8_data->registers[reg_y];
}

//	8XY2: AND Vx, Vy
void	_8xy2 (CHIP8* chip8_data) {
	uint8_t	reg_x = (chip8_data->opcode & 0x0F00) >> 8,
		reg_y = (chip8_data->opcode & 0x00F0) >> 4;
	if (reg_x <= 0xF && reg_y <= 0xF)
		chip8_data->registers[reg_x] &= chip8_data->registers[reg_y];
}

//	8XY3: XOR Vx, Vy
void	_8xy3 (CHIP8* chip8_data) {
	uint8_t	reg_x = (chip8_data->opcode & 0x0F00) >> 8,
		reg_y = (chip8_data->opcode & 0x00F0) >> 4;
	if (reg_x <= 0xF && reg_y <= 0xF)
		chip8_data->registers[reg_x] ^= chip8_data->registers[reg_y];
}

//	8XY4: ADD Vx, Vy
void	_8xy4 (CHIP8* chip8_data) {
	uint8_t	reg_x = (chip8_data->opcode & 0x0F00) >> 8,
		reg_y = (chip8_data->opcode & 0x00F0) >> 4;
	if (reg_x <= 0xF && reg_y <= 0xF) {
		uint16_t	sum = chip8_data->registers[reg_x] + chip8_data->registers[reg_y];
		chip8_data->registers[0xF] = sum > 0xFF ? 1 : 0;
		chip8_data->registers[reg_x] = sum & 0xFF;
	}
}

//	8XY5: SUB Vx, Vy
void	_8xy5 (CHIP8* chip8_data) {
	uint8_t	reg_x = (chip8_data->opcode & 0x0F00) >> 8,
		reg_y = (chip8_data->opcode & 0x00F0) >> 4;
	if (reg_x <= 0xF && reg_y <= 0xF) {
		chip8_data->registers[0xF] = chip8_data->registers[reg_x] >= chip8_data->registers[reg_y] ? 1 : 0;
		chip8_data->registers[reg_x] = (chip8_data->registers[reg_x] - chip8_data->registers[reg_y]) & 0xFF;
	}
}

//	8XY6: SHR Vx {, Vy}
void	_8xy6 (CHIP8* chip8_data) {
	uint8_t	reg_x = (chip8_data->opcode & 0x0F00) >> 8,
		reg_y = (chip8_data->opcode & 0x00F0) >> 4;
	if (reg_x <= 0xF && reg_y <= 0xF) {
		chip8_data->registers[reg_x] = chip8_data->registers[reg_y];
		chip8_data->registers[0xF] = chip8_data->registers[reg_x] & 0x1;
		chip8_data->registers[reg_x] >>= 1;
	}
}

//	8XY7:SUBN Vx, Vy
void	_8xy7 (CHIP8* chip8_data) {
	uint8_t	reg_x = (chip8_data->opcode & 0x0F00) >> 8,
		reg_y = (chip8_data->opcode & 0x00F0) >> 4;
	if (reg_x <= 0xF && reg_y <= 0xF) {
		chip8_data->registers[0xF] = chip8_data->registers[reg_y] >= chip8_data->registers[reg_x] ? 1 : 0;
		chip8_data->registers[reg_x] = (chip8_data->registers[reg_y] - chip8_data->registers[reg_x]) & 0xFF;
	}

}

//	8XYE:SHL Vx {, Vy}
void	_8xyE (CHIP8* chip8_data) {
	uint8_t	reg_x = (chip8_data->opcode & 0x0F00) >> 8,
		reg_y = (chip8_data->opcode & 0x00F0) >> 4;
	if (reg_x <= 0xF && reg_y <= 0xF) {
		chip8_data->registers[reg_x] = chip8_data->registers[reg_y];
		chip8_data->registers[0xF] = (chip8_data->registers[reg_x] & 0x80) >> 7;
		chip8_data->registers[reg_x] <<= 1;
		chip8_data->registers[reg_x] &= 0xFF;
	}
}

//	9XY0: SNE Vx, Vy
void	_9xy0 (CHIP8* chip8_data) {
	uint8_t	reg_x = (chip8_data->opcode & 0x0F00) >> 8,
		reg_y = (chip8_data->opcode & 0x00F0) >> 4;
	if (reg_x <= 0xF && reg_y <= 0xF
			&& chip8_data->registers[reg_x] != chip8_data->registers[reg_y])
		chip8_data->PC += 2;
}

//	ANNN: LD I, addr
void	_Annn (CHIP8* chip8_data) {
	chip8_data->IR = chip8_data->opcode & 0x0FFF;
}

//	BNNN: JP V0, addr
void	_Bnnn (CHIP8* chip8_data) {
	chip8_data->PC = (chip8_data->opcode & 0x0FFF) + chip8_data->registers[0];
}

//	CXKK:RND Vx, byte
void	_Cxkk (CHIP8* chip8_data) {
	uint8_t	reg_x = (chip8_data->opcode & 0x0F00) >> 8;
	if (reg_x <= 0xF) {
		uint16_t	kk = chip8_data->opcode & 0x00FF;
		chip8_data->registers[reg_x] = kk & (rand() % 255);
	}
}

//	DXYN - DRW Vx, Vy, nibble
void	_Dxyn (CHIP8* chip8_data) {
	uint8_t	reg_x = (chip8_data->opcode & 0x0F00) >> 8,
		reg_y = (chip8_data->opcode & 0x00F0) >> 4,
		n_height = chip8_data->opcode & 0x000F;
	if (reg_x <= 0xF && reg_y <= 0xF) {
		unsigned x = chip8_data->registers[reg_x] % DIS_W;
		unsigned y = chip8_data->registers[reg_y] % DIS_H;
		uint8_t sprite_byte, sprite_pixel;
		uint32_t *display_pixel;
		chip8_data->registers[0xF] = 0;
		for (unsigned row = 0; row < n_height; row++) {
			sprite_byte = chip8_data->RAM[chip8_data->IR + row];
			for (unsigned col = 0; col < 8; col++) {
				if (x + col >= DIS_W || y + row >= DIS_H)
					continue;
				sprite_pixel = (sprite_byte >> (7 - col)) & 0x1;
				pthread_mutex_lock(&chip8_data->display_mutex);
				display_pixel = &chip8_data->display[(y + row) * DIS_W + (x + col)];
				if (*display_pixel == 1 && sprite_pixel == 1)
					chip8_data->registers[0xF] = 1;
				*display_pixel ^= sprite_pixel;
				pthread_mutex_unlock(&chip8_data->display_mutex);
			}
		}
		//render_display(chip8_data);
	}
}

//	EX9E:SKP Vx
void	_Ex9E (CHIP8* chip8_data) {
	uint8_t	reg_x = (chip8_data->opcode & 0x0F00) >> 8;
	uint8_t	key = chip8_data->registers[ reg_x ];
	pthread_mutex_lock(&chip8_data->keys_mutex);
	if (chip8_data->keys[ key ]) {
		chip8_data->keys[ key ] = 0;
		chip8_data->PC += 2;
	}
	pthread_mutex_unlock(&chip8_data->keys_mutex);
}

//	EXA1:SKNP Vx
void	_ExA1 (CHIP8* chip8_data) {
	uint8_t	reg_x = (chip8_data->opcode & 0x0F00) >> 8;
	uint8_t	key = chip8_data->registers[reg_x];
	pthread_mutex_lock(&chip8_data->keys_mutex);
	if (chip8_data->keys[ key ])
		chip8_data->PC += 2;
	pthread_mutex_unlock(&chip8_data->keys_mutex);
}

//	FX07: LD Vx, DT
void	_Fx07 (CHIP8* chip8_data) {
	uint8_t	reg_x = (chip8_data->opcode & 0x0F00) >> 8;
	if (reg_x <= 0xF)
		chip8_data->registers[reg_x] = chip8_data->DT;
}

//	FX0A:LD Vx, K
void	_Fx0A (CHIP8* chip8_data) {
	uint8_t	reg_x = (chip8_data->opcode & 0x0F00) >> 8;
	printf("ordered halt with key: %i\n", reg_x);
	if (reg_x <= 0xF) {
		chip8_data->halt = 1;
		chip8_data->key_register = reg_x;
		chip8_data->PC -= 2;
	}
}

//	FX15: LD DT, Vx
void	_Fx15 (CHIP8* chip8_data) {
	uint8_t	reg_x = (chip8_data->opcode & 0x0F00) >> 8;
	if (reg_x <= 0xF)
		chip8_data->DT = chip8_data->registers[reg_x];
}

//	FX18: LD ST, Vx - set sound timer = Vx.
void	_Fx18 (CHIP8* chip8_data) {
	uint8_t	reg_x = (chip8_data->opcode & 0x0F00) >> 8;
	if (reg_x <= 0xF)
		chip8_data->ST = chip8_data->registers[reg_x];
}

//	FX1E: ADD I, Vx - set I = I + Vx.
void	_Fx1E (CHIP8* chip8_data) {
	uint8_t	reg_x = (chip8_data->opcode & 0x0F00) >> 8;
	if (reg_x < 16) {
		chip8_data->IR += chip8_data->registers[reg_x];
		chip8_data->IR = chip8_data->IR & 0xFFF;
	}
}

//	FX29:LD F, Vx - set I = location of sprite for digit Vx.
void	_Fx29 (CHIP8* chip8_data) {
	uint8_t	vx = (chip8_data->opcode & 0x0F00) >> 8;
	if (vx <= 0xF)
		chip8_data->IR = vx * 5 + FONTS_START;
}

//	FX33:LD B, Vx - store BCD representation of Vx in memory location I, I+1, and I+2
void	_Fx33 (CHIP8* chip8_data) {
	uint8_t	reg_x = (chip8_data->opcode & 0x0F00) >> 8;
	if (reg_x <= 0xF && chip8_data->IR >= MEMORY_START
			&& chip8_data->IR + 2 < chip8_data->memory_occupied + MEMORY_START) {
		chip8_data->RAM[ chip8_data->IR ] = chip8_data->registers[reg_x] / 100;
		chip8_data->RAM[ chip8_data->IR + 1 ] = (chip8_data->registers[reg_x] / 10) % 10;
		chip8_data->RAM[ chip8_data->IR + 2 ] = chip8_data->registers[reg_x] % 10;
	}
}

//	FX55: LD [I], Vx
void	_Fx55 (CHIP8* chip8_data) {
	uint8_t	reg_x = (chip8_data->opcode & 0x0F00) >> 8;
	uint16_t	IR = chip8_data->IR;
	if (IR >= MEMORY_START && IR + reg_x < chip8_data->memory_occupied + MEMORY_START && reg_x <= 0xF) {
		for (uint8_t reg = 0; reg <= reg_x; reg++)
			chip8_data->RAM[IR + reg] = chip8_data->registers[reg];
	}
}

//	FX65: Lx Vx, [I]
void	_Fx65 (CHIP8* chip8_data) {
	uint8_t	reg_x = (chip8_data->opcode & 0x0F00) >> 8;
	uint16_t	IR = chip8_data->IR;
	for (uint8_t reg = 0; reg <= reg_x && reg <= 0xF && IR < chip8_data->memory_occupied + MEMORY_START
			&& IR > MEMORY_START; reg++, IR++)
		chip8_data->registers[reg] = chip8_data->RAM[IR];
}

/*
   (uint16_t) opcode
   - (0-4)   opcode & 0xF000 |
   - (4-8)   opcode & 0x0F00 >> 8 |
   - (8-12)  opcode & 0x00F0 >> 4 |
   - (12-16) opcode & 0x000F
   */

