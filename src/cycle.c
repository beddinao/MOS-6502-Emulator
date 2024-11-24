#include "mos6502.h"

/*
   (uint16_t) opcode
   - (0-4)   opcode & 0xF000 |
   - (4-8)   opcode & 0x0F00 >> 8 |
   - (8-12)  opcode & 0x00F0 >> 4 |
   - (12-16) opcode & 0x000F
   */

// /// /	CYCLE
/*
void	*instruction_cycle(void *p, uint8_t *ram) {
	_6502	*mos6502 = (_6502*)p;
	(void)ram;
	(void)mos6502;
	return NULL;
}
*/
