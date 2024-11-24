#include "mos6502.h"

/*
   (uint16_t) opcode
   - (0-4)   opcode & 0xF000 |
   - (4-8)   opcode & 0x0F00 >> 8 |
   - (8-12)  opcode & 0x00F0 >> 4 |
   - (12-16) opcode & 0x000F
   */

// /// /	INSTRUCTIONS	

uint8_t	BRK_IMP(_6502* mos6502) {
	printf("BRK IMP\n");
	return 7;
}

uint8_t	X(_6502 *mos6502) {
	printf("illegal opcode\n");
	return 0;
}

/// / //	LOADING

void	load_instructions(_6502* mos6502){
	for (uint8_t i = 0; i < 0xFF; i++)
		mos6502->instructions[i] = X;
}
