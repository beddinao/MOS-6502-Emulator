#include "mos6502.h"

// /// /	INSTRUCTIONS	

/*
	BRK - op0x00
	IMPLIED
	1 Byte, 7 Cycles
*/
uint8_t	BRK_IMP(_6502* mos6502) {
	//printf("BRK IMP\n");
	mos6502->PC += 2;
	// high_byte first
	mos6502->push(mos6502, mos6502->PC >> 8);
	mos6502->push(mos6502, mos6502->PC & 0x00FF);
	mos6502->set_flag(mos6502, 'B', 1);
	mos6502->push(mos6502, mos6502->SR);
	mos6502->set_flag(mos6502, 'I', 1);
	mos6502->PC = mos6502->bus->read(mos6502->bus->ram, RSTV) << 8 |
		mos6502->bus->read(mos6502->bus->ram, RSTV+1);
	return 7;
}

/*
	ORA - op0x01
	INDEXED INDIRECT
	2 Bytes, 6 Cycles
*/
uint8_t	ORA_INDX(_6502* mos6502) {
	//printf("ORA_INDX\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1), high_byte;
	uint8_t	base = (low_byte + mos6502->X) & 0xFF;
	low_byte = mos6502->bus->read(mos6502->bus->ram, base);
	high_byte = mos6502->bus->read(mos6502->bus->ram, (base+1) & 0xFF);
	uint8_t	operand = mos6502->bus->read(mos6502->bus->ram, high_byte << 0x8 | low_byte);
	mos6502->A |= operand;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 2;
	return 6;
}


/*
	ORA - op0x05
	ZERO PAGE
	2 Bytes, 3 Cycles
*/
uint8_t	ORA_ZP(_6502* mos6502){
	//printf("ORA_ZP\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        operand = mos6502->bus->read(mos6502->bus->ram, 0x00 << 0x8 | low_byte);
	mos6502->A |= operand;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 2;
	return 3;
}

/*
	ASL - op0x06
	ZERO PAGE
	2 Bytes, 5 Cycles
*/
uint8_t	ASL_ZP(_6502* mos6502) {
	//printf("ASL_ZP\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
		operand = mos6502->bus->read(mos6502->bus->ram, 0x00 << 0x8 | low_byte);
	mos6502->set_flag(mos6502, 'C', (operand >> 0x7) & 0x1);
	operand <<= 0x1;
	mos6502->set_flag(mos6502, 'Z', operand == 0);
	mos6502->set_flag(mos6502, 'N', (operand >> 0x7) & 0x1);
	mos6502->bus->write(mos6502->bus->ram, 0x00 << 0x8 | low_byte, operand);
	mos6502->PC += 2;
	return 5;
}

/*
	PHP - op0x08
	IMPLIED
	1 Byte, 3 Cycles
*/
uint8_t	PHP_IMP(_6502 *mos6502) {
	//printf("PHP_IMP\n");
	mos6502->set_flag(mos6502, 'B', 1);
	mos6502->set_flag(mos6502, 'F', 1);
	mos6502->push(mos6502, mos6502->SR);
	mos6502->PC += 1;
	return 3;
}

/*
	ORA - op0x09
	IMMEDIATE
	2 Bytes, 2 Cycles
*/
uint8_t	ORA_IMM(_6502 *mos6502) {
	//printf("ORA_IMM\n");
	uint8_t	operand = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1);
	mos6502->A |= operand;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 2;
	return 2;
}

/*
	ASL - op0x0A
	ACCUMULATOR
	1 Byte, 2 Cycles
*/
uint8_t	ASL_ACC(_6502 *mos6502) {
	//printf("ASL_ACC\n");
	mos6502->set_flag(mos6502, 'C', (mos6502->A >> 0x7) & 0x1);
	mos6502->A <<= 0x1;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', (mos6502->A >> 0x7) & 0x1);
	mos6502->PC += 1;
	return 2;
}

/*
	ORA - op0x0D
	ABSOLUTE
	3 Byte, 4 Cycles
*/
uint8_t	ORA_ABS(_6502 *mos6502) {
	//printf("ORA_ABS\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2);
	mos6502->A |= mos6502->bus->read(mos6502->bus->ram, high_byte << 0x8 | low_byte);
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 0x3;
	return 4;
}

/*
	ASL - op0x0E
	ABSOLUTE
	3 Bytes, 6 Cycles
*/
uint8_t	ASL_ABS(_6502* mos6502) {
	//printf("ASL_ABS\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
		high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2);
	uint8_t operand = mos6502->bus->read(mos6502->bus->ram, high_byte << 0x8 | low_byte);
	mos6502->set_flag(mos6502, 'C', (operand >> 0x7) & 0x1);
	operand <<= 0x1;
	mos6502->set_flag(mos6502, 'Z', operand == 0);
	mos6502->set_flag(mos6502, 'N', (operand >> 0x7) & 0x1);
	mos6502->bus->write(mos6502->bus->ram, high_byte << 0x8 | low_byte, operand);
	mos6502->PC += 3;
	return 6;
}

/*
	BPL - op0x10
	RELATIVE
	2 Bytes, 2** Cycles
*/
uint8_t	BPL_REL(_6502 *mos6502) {
	//printf("BPL_REL\n");
	mos6502->PC += 2;
	if (mos6502->get_flag(mos6502, 'N') == 0) {
		uint8_t operand = mos6502->bus->read(mos6502->bus->ram, mos6502->PC-1);
		mos6502->PC += operand;
		return 3;
	}
	return 2;
}

/*
	ORA - op0x11
	INDIRECT INDEXING
	2 Bytes, 5* Cycles
*/
uint8_t	ORA_INDY(_6502* mos6502) {
	//printf("ORA_INDY\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1), high_byte;
	uint8_t	base = 0x00 << 0x8 | low_byte, cycles = 5;
	low_byte = mos6502->bus->read(mos6502->bus->ram, base);
	high_byte = mos6502->bus->read(mos6502->bus->ram, (base+1) & 0xFF);
	uint16_t	addr = (high_byte << 0x8 | low_byte) + mos6502->Y;
	cycles += ((addr & 0xFF00) != ((high_byte << 0x8 | low_byte) & 0xFF00));
	uint8_t	operand = mos6502->bus->read(mos6502->bus->ram, addr);
	mos6502->A |= operand;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 2;
	return cycles;
}

/*
	ORA - op0x15
	ZERO PAGE X
	2 Bytes, 4 Cycles
*/
uint8_t	ORA_ZPX(_6502* mos6502) {
	//printf("ORA_ZPX\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        operand = mos6502->bus->read(mos6502->bus->ram, (0x00 << 0x8 | low_byte) + mos6502->X);
	mos6502->A |= operand;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 2;
	return 4;
}

/*
	ASL - op0x16
	ZERO PAGE X
	2 Bytes, 6 Cycles
*/
uint8_t	ASL_ZPX(_6502* mos6502) {
	//printf("ASL_ZPX\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1);
	uint16_t	addr = (0x00 << 0x8 | low_byte) + mos6502->X;
	uint8_t	operand = mos6502->bus->read(mos6502->bus->ram, addr);
	mos6502->set_flag(mos6502, 'C', (operand >> 0x7) & 0x1);
	operand <<= 0x1;
	mos6502->set_flag(mos6502, 'Z', operand == 0);
	mos6502->set_flag(mos6502, 'N', (operand >> 0x7) & 0x1);
	mos6502->bus->write(mos6502->bus->ram, addr, operand);
	mos6502->PC += 2;
	return 6;
}

/*
	CLC - op0x18
	IMPLIED
	1 Byte, 2 Cycles
*/
uint8_t	CLC_IMP(_6502 *mos6502) {
	//printf("CLC_IMP\n");
	mos6502->set_flag(mos6502, 'C', 0);
	mos6502->PC += 1;
	return 2;
}

/*
	ORA - op0x19
	ABSOLUTE Y
	3 Bytes, 4* Cycles
*/
uint8_t	ORA_ABSY(_6502 *mos6502) {
	//printf("ORA_ABSY\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2), cycles = 4;
	uint16_t absy_addr = (high_byte << 0x8 | low_byte) + mos6502->Y;
	cycles += ((absy_addr & 0xFF00) != ((high_byte << 0x8 | low_byte) & 0xFF00));
	mos6502->A |= mos6502->bus->read(mos6502->bus->ram, absy_addr);
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 3;
	return cycles;
}

/*
	ORA - op0x1D
	ABSOLUTE X
	3 Bytes, 4* Cycles
*/
uint8_t	ORA_ABSX(_6502 *mos6502) {
	//printf("ORA_ABSX\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2), cycles = 4;
	uint16_t absx_addr = (high_byte << 0x8 | low_byte) + mos6502->X;
	cycles += ((absx_addr & 0xFF00) != ((high_byte << 0x8 | low_byte) & 0xFF00));
	mos6502->A |= mos6502->bus->read(mos6502->bus->ram, absx_addr);
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 3;
	return 4;
}

/*
	ASL - op0x1E
	ABSOLUTE X
	3 Bytes, 7 Cycles
*/
uint8_t	ASL_ABSX(_6502 *mos6502) {
	//printf("ASL_ABSX\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
		high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2);
	uint16_t	absx_addr = (high_byte << 0x8 | low_byte) + mos6502->X;
	uint8_t operand = mos6502->bus->read(mos6502->bus->ram, absx_addr);
	mos6502->set_flag(mos6502, 'C', (operand >> 0x7) & 0x1);
	operand <<= 0x1;
	mos6502->set_flag(mos6502, 'Z', operand == 0);
	mos6502->set_flag(mos6502, 'N', (operand >> 0x7) & 0x1);
	mos6502->bus->write(mos6502->bus->ram, absx_addr, operand);
	mos6502->PC += 3;
	return 7;
}

/*
	JSR - op0x20
	ABSOLUTE
	3 Bytes, 6 Cycles
*/
uint8_t	JSR_ABS(_6502* mos6502) {
	//printf("JSR_ABS\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2);
	// high_byte first
	mos6502->push(mos6502, mos6502->PC >> 0x8);
	mos6502->push(mos6502, mos6502->PC & 0x00FF);
	mos6502->PC = low_byte << 0x8 | high_byte;
	return 6;
}

/*
	AND - op0x21
	INDEXED INDIRECT
	2 Bytes, 6 Cycles
*/
uint8_t	AND_INDX(_6502 *mos6502) {
	//printf("AND_INDX\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1), high_byte;
	uint8_t	base = (low_byte + mos6502->X) & 0xFF;
	low_byte = mos6502->bus->read(mos6502->bus->ram, base);
	high_byte = mos6502->bus->read(mos6502->bus->ram, (base+1) & 0xFF);
	uint8_t	operand = mos6502->bus->read(mos6502->bus->ram, high_byte << 0x8 | low_byte);
	mos6502->A &= operand;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 2;
	return 6;
}

/*
	BIT - op0x24
	ZERO PAGE
	2 Bytes, 3 Cycles
*/
uint8_t	BIT_ZP(_6502 *mos6502) {
	//printf("BIT_ZP\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
		operand = mos6502->bus->read(mos6502->bus->ram, 0x00 << 0x8 | low_byte);
	mos6502->set_flag(mos6502, 'Z', (operand & mos6502->A) == 0);
	mos6502->set_flag(mos6502, 'N', (operand >> 0x7) & 0x1);
	mos6502->set_flag(mos6502, 'V', (operand >> 0x6) & 0x1);
	mos6502->PC += 2;
	return 3;
}

/*
	AND - op0x25
	ZERO PAGE
	2 Bytes, 3 Cycles
*/
uint8_t	AND_ZP(_6502 *mos6502) {
	//printf("AND_ZP\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        operand = mos6502->bus->read(mos6502->bus->ram, 0x00 << 0x8 | low_byte);
	mos6502->A &= operand;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 2;
	return 3;
}

/*
	ROL - op0x27
	ZERO PAGE
	2 Bytes, 5 Cycles
*/
uint8_t	ROL_ZP(_6502 *mos6502) {
	//printf("ROL_ZP\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
		operand = mos6502->bus->read(mos6502->bus->ram, 0x00 << 8 | low_byte),
		carry_in = mos6502->get_flag(mos6502, 'C');
	mos6502->set_flag(mos6502, 'C', (operand >> 0x7) & 0x1);
	operand  = operand << 0x1 | carry_in;
	mos6502->set_flag(mos6502, 'Z', operand == 0);
	mos6502->set_flag(mos6502, 'N', (operand >> 0x7) & 0x1);
	mos6502->bus->write(mos6502->bus->ram, 0x00 << 0x8 | low_byte, operand);
	mos6502->PC += 2;
	return 5;
}

/*
	PLP - op0x28
	IMPLIED
	1 Byte, 4 Cycles
*/
uint8_t	PLP_IMP(_6502 *mos6502) {
	//printf("PLP_IMP\n");
	mos6502->SR = mos6502->pull(mos6502);
	mos6502->PC += 1;
	return 4;
}

/*
	AND - op0x29
	IMMEDIATE
	2 Bytes, 2 Cycles
*/
uint8_t	AND_IMM(_6502 *mos6502) {
	//printf("AND_IMM\n");
	uint8_t	operand = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1);
	mos6502->A &= operand;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 2;
	return 2;
}

/*
	ROL - op0x2A
	ACCUMULATOR
	1 Byte, 2 Cycles
*/
uint8_t	ROL_ACC(_6502 *mos6502) {
	//printf("ROL_ACC\n");
	uint8_t	carry_in = mos6502->get_flag(mos6502, 'C');
	mos6502->set_flag(mos6502, 'C', (mos6502->A >> 0x7) & 0x1);
	mos6502->A  = mos6502->A << 0x1 | carry_in;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', (mos6502->A >> 0x7) & 0x1);
	mos6502->PC += 1;
	return 2;
}

/*
	BIT - op0x2C
	ABSOLUTE
	3 Bytes, 4 Cycles
*/
uint8_t	BIT_ABS(_6502 *mos6502) {
	//printf("BIT_ABS\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	       	high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2);
	uint8_t	operand = mos6502->bus->read(mos6502->bus->ram, high_byte << 0x8 | low_byte);
	mos6502->set_flag(mos6502, 'Z', (operand & mos6502->A) == 0);
	mos6502->set_flag(mos6502, 'N', (operand >> 0x7) & 0x1);
	mos6502->set_flag(mos6502, 'V', (operand >> 0x6) & 0x1);
	mos6502->PC += 3;
	return 4;
}

/*
	AND - op0x2D
	ABSOLUTE
	3 Bytes, 4 Cycles
*/
uint8_t	AND_ABS(_6502 *mos6502) {
	//printf("AND_ABS\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2);
	mos6502->A &= mos6502->bus->read(mos6502->bus->ram, high_byte << 8 | low_byte);
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 3;
	return 4;
}

/*
	ROL - op0x2E
	ABSOLUTE
	3 Bytes, 6 Cycles
*/
uint8_t	ROL_ABS(_6502 *mos6502) {
	//printf("ROL_ABS\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
		high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2);
	uint8_t	operand = mos6502->bus->read(mos6502->bus->ram, high_byte << 8 | low_byte),
		carry_in = mos6502->get_flag(mos6502, 'C');
	mos6502->set_flag(mos6502, 'C', (operand >> 0x7) & 0x1);
	operand  = operand << 0x1 | carry_in;
	mos6502->set_flag(mos6502, 'Z', operand == 0);
	mos6502->set_flag(mos6502, 'N', (operand >> 0x7) & 0x1);
	mos6502->bus->write(mos6502->bus->ram, high_byte << 0x8 | low_byte, operand);
	mos6502->PC += 3;
	return 6;
}

/*
	BMI - op0x30
	RELATIVE
	2 Bytes, 2** Cycles
*/
uint8_t	BMI_REL(_6502 *mos6502) {
	//printf("BMI_REL\b");
	mos6502->PC += 2;
	if (mos6502->get_flag(mos6502, 'N')) {
		uint8_t operand = mos6502->bus->read(mos6502->bus->ram, mos6502->PC-1);
		mos6502->PC += operand;
		return 3;
	}
	return 2;
}

/*
	AND - op0x31
	INDIRECT INDEXED
	2 Bytes, 5* Cycles
*/
uint8_t	AND_INDY(_6502 *mos6502) {
	//printf("AND_INDY\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1), high_byte;
	uint8_t	base = 0x00 << 0x8 | low_byte, cycles = 5;
	low_byte = mos6502->bus->read(mos6502->bus->ram, base);
	high_byte = mos6502->bus->read(mos6502->bus->ram, (base+1) & 0xFF);
	uint16_t	addr = (high_byte << 0x8 | low_byte) + mos6502->Y;
	cycles += ((addr & 0xFF00) != ((high_byte << 0x8 | low_byte) & 0xFF00));
	uint8_t	operand = mos6502->bus->read(mos6502->bus->ram, addr);
	mos6502->A &= operand;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 2;
	return cycles;
}

/*
	AND - op0x35
	ZERO PAGE X
	2 Bytes, 4 Cycles
*/
uint8_t	AND_ZPX(_6502 *mos6502) {
	//printf("AND_ZPX\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        operand = mos6502->bus->read(mos6502->bus->ram, (0x00 << 0x8 | low_byte) + mos6502->X);
	mos6502->A &= operand;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 2;
	return 4;
}

/*
	ROL - op0x36
	ZERO PAGE X
	2 Bytes, 6 Cycles
*/
uint8_t	ROL_ZPX(_6502 *mos6502) {
	//printf("ROL_ZPX\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1);
	uint16_t	addr = (0x00 << 0x8 | low_byte) + mos6502->X;
	uint8_t	operand = mos6502->bus->read(mos6502->bus->ram, addr),
		carry_in = mos6502->get_flag(mos6502, 'C');
	mos6502->set_flag(mos6502, 'C', (operand >> 0x7) & 0x1);
	operand  = operand << 0x1 | carry_in;
	mos6502->set_flag(mos6502, 'Z', operand == 0);
	mos6502->set_flag(mos6502, 'N', (operand >> 0x7) & 0x1);
	mos6502->bus->write(mos6502->bus->ram, addr, operand);
	mos6502->PC += 2;
	return 6;
}

/*
	SEC - op0x38
	IMPLIED
	1 Byte, 2 Cycles
*/
uint8_t	SEC_IMP(_6502 *mos6502) {
	//printf("SEC_IMP\n");
	mos6502->set_flag(mos6502, 'C', 1);
	mos6502->PC += 1;
	return 2;
}

/*
	AND - op0x39
	ABSOLUTE Y
	3 Bytes, 4* Cycles
*/
uint8_t	AND_ABSY(_6502 *mos6502) {
	//printf("AND_ABSY\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2), cycles = 4;
	uint16_t absy_addr = (high_byte << 0x8 | low_byte) + mos6502->Y;
	cycles += ((absy_addr & 0xFF00) != ((high_byte << 0x8 | low_byte) & 0xFF00));
	mos6502->A &= mos6502->bus->read(mos6502->bus->ram, absy_addr);
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 3;
	return cycles;
}

/*
	AND - op0x3D
	ABSOLUTE X
	3 Bytes, 4* Cycles
*/
uint8_t	AND_ABSX(_6502 *mos6502) {
	//printf("AND_ABSX\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2), cycles = 4;
	uint16_t absx_addr = (high_byte << 0x8 | low_byte) + mos6502->X;
	cycles += ((absx_addr & 0xFF00) != ((high_byte << 0x8 | low_byte) & 0xFF00));
	mos6502->A &= mos6502->bus->read(mos6502->bus->ram, absx_addr);
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 3;
	return cycles;
}

/*
	ROL - op0x3E
	ABSOLUTE X
	3 Bytes, 7 Cycles
*/
uint8_t	ROL_ABSX(_6502 *mos6502) {
	//printf("ROL_ABSX\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
		high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2);
	uint16_t	absx_addr = (high_byte << 0x8 | low_byte) + mos6502->X;
	uint8_t	operand = mos6502->bus->read(mos6502->bus->ram, absx_addr),
		carry_in = mos6502->get_flag(mos6502, 'C');
	mos6502->set_flag(mos6502, 'C', (operand >> 0x7) & 0x1);
	operand  = operand << 0x1 | carry_in;
	mos6502->set_flag(mos6502, 'Z', operand == 0);
	mos6502->set_flag(mos6502, 'N', (operand >> 0x7) & 0x1);
	mos6502->bus->write(mos6502->bus->ram, absx_addr, operand);
	mos6502->PC += 3;
	return 7;
}

/*
	RTI - op0x40
	IMPLIED
	1 Byte, 6 Cycles
*/
uint8_t	RTI_IMP(_6502 *mos6502) {
	//printf("RTI_IMP\n");
	mos6502->SR = mos6502->pull(mos6502);
	uint8_t	high_byte = mos6502->pull(mos6502);
	mos6502->PC = mos6502->pull(mos6502) << 8 | high_byte;
	return 6;
}

/*
	EOR - op0x41
	INDEXED INDIRECT
	2 Bytes, 6 Cycles
*/
uint8_t	EOR_INDX(_6502 *mos6502) {
	//printf("EOR_INDX\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1), high_byte;
	uint8_t	base = (low_byte + mos6502->X) & 0xFF;
	low_byte = mos6502->bus->read(mos6502->bus->ram, base);
	high_byte = mos6502->bus->read(mos6502->bus->ram, (base+1) & 0xFF);
	uint8_t	operand = mos6502->bus->read(mos6502->bus->ram, high_byte << 0x8 | low_byte);
	mos6502->A ^= operand;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 2;
	return 6;
}

/*
	EOR - op0x45
	ZERO PAGE
	2 Bytes, 3 Cycles
*/
uint8_t	EOR_ZP(_6502 *mos6502) {
	//printf("EOR_ZP\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        operand = mos6502->bus->read(mos6502->bus->ram, 0x00 << 8 | low_byte);
	mos6502->A ^= operand;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 2;
	return 3;
}

/*
	LSR - op0x46
	ZERO PAGE
	2 Bytes, 5 Cycles
*/
uint8_t	LSR_ZP(_6502 *mos6502) {
	//printf("LSR_ZP\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
		operand = mos6502->bus->read(mos6502->bus->ram, 0x00 << 8 | low_byte);
	mos6502->set_flag(mos6502, 'C', operand & 0x1);
	operand >>= 0x1;
	mos6502->set_flag(mos6502, 'Z', operand == 0);
	mos6502->bus->write(mos6502->bus->ram, 0x00 << 0x8 | low_byte, operand);
	mos6502->PC += 2;
	return 5;
}

/*
	PHA - op0x48
	IMPLIED
	1 Byte, 3 Cycles
*/
uint8_t	PHA_IMP(_6502 *mos6502) {
	//printf("PHA_IMP\n");
	mos6502->push(mos6502, mos6502->A);
	mos6502->PC += 1;
	return 3;
}

/*
	EOR - op0x49
	IMMEDIATE
	2 Bytes, 2 Cycles
*/
uint8_t	EOR_IMM(_6502 *mos6502) {
	//printf("EOR_IMM\n");
	uint8_t operand = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1);
	mos6502->A ^= operand;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 2;
	return 2;
}

/*
	LSR - op0x4A
	ACCUMULATOR
	1 Byte, 2 Cycles
*/
uint8_t	LSR_ACC(_6502 *mos6502) {
	//printf("LSR_ACC\n");
	mos6502->set_flag(mos6502, 'C', mos6502->A & 0x1);
	mos6502->A >>= 0x1;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->PC += 1;
	return 2;
}

/*
	JMP - op0x4C
	ABSOLUTE
	3 Bytes, 3 Cycles
*/
uint8_t	JMP_ABS(_6502 *mos6502) {
	//printf("JMP_ABS\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
		high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2);
	mos6502->PC = high_byte << 0x8 | low_byte;
	return 3;
}

/*
	EOR - op0x4D
	ABSOLUTE
	3 Bytes, 4 Cycles
*/
uint8_t	EOR_ABS(_6502 *mos6502) {
	//printf("EOR_ABS\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2);
	mos6502->A ^= mos6502->bus->read(mos6502->bus->ram, high_byte << 8 | low_byte);
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 3;
	return 4;
}

/*
	LSR - op0x4E
	ABSOLUTE
	3 Bytes, 6 Cycles
*/
uint8_t	LSR_ABS(_6502 *mos6502) {
	//printf("LSR_ABS\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
		high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2);
	uint8_t	operand = mos6502->bus->read(mos6502->bus->ram, high_byte << 8 | low_byte);
	mos6502->set_flag(mos6502, 'C', operand & 0x1);
	operand >>= 0x1;
	mos6502->set_flag(mos6502, 'Z', operand == 0);
	mos6502->bus->write(mos6502->bus->ram, high_byte << 0x8 | low_byte, operand);
	mos6502->PC += 3;
	return 6;
}

/*
	BVC - op0x50
	RELATIVE
	2 Bytes, 2** Cycles
*/
uint8_t	BVC_REL(_6502 *mos6502) {
	//printf("BVC_REL\n");
	mos6502->PC += 2;
	if (mos6502->get_flag(mos6502, 'V') == 0) {
		uint8_t operand = mos6502->bus->read(mos6502->bus->ram, mos6502->PC-1);
		mos6502->PC += operand;
		return 3;
	}
	return 2;
}

/*
	EOR - op0x51
	INDIRECT INDEXED
	2 Bytes, 5* Cycles
*/
uint8_t	EOR_INDY(_6502 *mos6502) {
	//printf("EOR_INDY\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1), high_byte;
	uint8_t	base = 0x00 << 0x8 | low_byte, cycles = 5;
	low_byte = mos6502->bus->read(mos6502->bus->ram, base);
	high_byte = mos6502->bus->read(mos6502->bus->ram, (base+1) & 0xFF);
	uint16_t	addr = (high_byte << 0x8 | low_byte) + mos6502->Y;
	cycles += ((addr & 0xFF00) != ((high_byte << 0x8 | low_byte) & 0xFF00));
	uint8_t	operand = mos6502->bus->read(mos6502->bus->ram, addr);
	mos6502->A ^= operand;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 2;
	return cycles;
}

/*
	EOR - op0x55
	ZERO PAGE X
	2 Bytes, 4 Cycles
*/
uint8_t	EOR_ZPX(_6502 *mos6502) {
	//printf("EOR_ZPX\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        operand = mos6502->bus->read(mos6502->bus->ram, (0x00 << 8 | low_byte) + mos6502->X);
	mos6502->A ^= operand;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 2;
	return 4;
}

/*
	LSR - op0x56
	ZERO PAGE X
	2 Bytes, 6 Cycles
*/
uint8_t	LSR_ZPX(_6502 *mos6502) {
	//printf("LSR_ZPX\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1);
	uint16_t	addr = (0x00 << 0x8 | low_byte) + mos6502->X;
	uint8_t	operand = mos6502->bus->read(mos6502->bus->ram, addr);
	mos6502->set_flag(mos6502, 'C', operand & 0x1);
	operand >>= 0x1;
	mos6502->set_flag(mos6502, 'Z', operand == 0);
	mos6502->bus->write(mos6502->bus->ram, addr, operand);
	mos6502->PC += 2;
	return 6;
}

/*
	CLI - op0x58
	IMPLIED
	1 Byte, 2 Cycles
*/
uint8_t	CLI_IMP(_6502 *mos6502) {
	//printf("CLI_IMP\n");
	mos6502->set_flag(mos6502, 'I', 0);
	mos6502->PC += 1;
	return 2;
}

/*
	EOR - op0x59
	ABSOLUTE Y
	3 Bytes, 4* Cycles
*/
uint8_t	EOR_ABSY(_6502 *mos6502) {
	//printf("EOR_ABSY\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2), cycles = 4;
	uint16_t absy_addr = (high_byte << 0x8 | low_byte) + mos6502->Y;
	cycles += ((absy_addr & 0xFF00) != ((high_byte << 0x8 | low_byte) & 0xFF00));
	mos6502->A ^= mos6502->bus->read(mos6502->bus->ram, absy_addr);
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 3;
	return cycles;
}

/*
	EOR - op0x5D
	ABSOUTE X
	3 Bytes, 4* Cycles
*/
uint8_t	EOR_ABSX(_6502 *mos6502) {
	//printf("EOR_ABSX\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2), cycles = 4;
	uint16_t absx_addr = (high_byte << 0x8 | low_byte) + mos6502->X;
	cycles += ((absx_addr & 0xFF00) != ((high_byte << 0x8 | low_byte) & 0xFF00));
	mos6502->A ^= mos6502->bus->read(mos6502->bus->ram, absx_addr);
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 3;
	return cycles;
}

/*
	LSR - op0x5E
	ABSOLUTE X
	3 Bytes, 7 Cycles
*/
uint8_t	LSR_ABSX(_6502 *mos6502) {
	//printf("LSR_ABSX\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
		high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2);
	uint16_t	absx_addr = (high_byte << 0x8 | low_byte) + mos6502->X;
	uint8_t	operand = mos6502->bus->read(mos6502->bus->ram, absx_addr);
	mos6502->set_flag(mos6502, 'C', operand & 0x1);
	operand >>= 0x1;
	mos6502->set_flag(mos6502, 'Z', operand == 0);
	mos6502->bus->write(mos6502->bus->ram, absx_addr, operand);
	mos6502->PC += 3;
	return 7;
}

/*
	RTS - op0x60
	IMPLIED
	1 Byte, 6 Cycles
*/
uint8_t	RTS_IMP(_6502 *mos6502) {
	//printf("RTS_IMP\n");
	// low byte first
	uint8_t low_byte = mos6502->pull(mos6502);
	mos6502->PC = mos6502->pull(mos6502) << 8 | low_byte;
	mos6502->PC += 1;
	return 6;
}

/*
	ADC - op0x61
	INDEXED INDIRECT
	2 Bytes, 6 Cycles
*/
uint8_t	ADC_INDX(_6502 *mos6502) {
	//printf("ADC_INDX\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1), high_byte;
	uint8_t	base = (low_byte + mos6502->X) & 0xFF;
	low_byte = mos6502->bus->read(mos6502->bus->ram, base);
	high_byte = mos6502->bus->read(mos6502->bus->ram, (base+1) & 0xFF);
	uint8_t	operand = mos6502->bus->read(mos6502->bus->ram, high_byte << 0x8 | low_byte);
	uint16_t	res = mos6502->A + operand + mos6502->get_flag(mos6502, 'C');
	mos6502->set_flag(mos6502, 'C', res > 255);
	mos6502->set_flag(mos6502, 'V', (((operand ^ (res & 0xFF)) & 0x80) && !((operand ^ mos6502->A) & 0x80)));
	mos6502->A = res & 0xFF;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	
	mos6502->PC += 2;
	return 6;
}

/*
	ADC - op0x65
	ZERO PAGE
	2 Bytes, 3 Cycles
*/
uint8_t	ADC_ZP(_6502 *mos6502) {
	//printf("ADC_ZP\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
		operand = mos6502->bus->read(mos6502->bus->ram, 0x00 << 0x8 | low_byte);
	uint16_t res = mos6502->A + operand + mos6502->get_flag(mos6502, 'C');
	mos6502->set_flag(mos6502, 'C', res > 255);
	mos6502->set_flag(mos6502, 'V', (((operand ^ (res & 0xFF)) & 0x80) && !((operand ^ mos6502->A) & 0x80)));
	mos6502->A = res & 0xFF;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 2;
	return 3;
}

/*
	ROR - op0x66
	ZERO PAGE
	2 Bytes, 5 Cycles
*/
uint8_t	ROR_ZP(_6502 *mos6502) {
	//printf("ROR_ZP\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
		operand = mos6502->bus->read(mos6502->bus->ram, 0x00 << 0x8 | low_byte),
		carry_in = mos6502->get_flag(mos6502, 'C');

	mos6502->set_flag(mos6502, 'C', operand & 0x1);
	operand  = operand >> 0x1 | carry_in;


	mos6502->set_flag(mos6502, 'Z', operand == 0);
	mos6502->set_flag(mos6502, 'N', (operand >> 0x7) & 0x1);
	
	mos6502->bus->write(mos6502->bus->ram, 0x00 << 0x8 | low_byte, operand);
	mos6502->PC += 2;
	return 5;
}

/*
	PLA - op0x68
	IMPLIED
	1 Byte, 4 Cycles
*/
uint8_t	PLA_IMP(_6502 *mos6502) {
	//printf("PLA_IMP\n");
	mos6502->A = mos6502->pull(mos6502);
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 1;
	return 4;
}

/*
	ADC - op0x69
	IMMEDIATE
	2 Bytes, 2 Cycles
*/
uint8_t	ADC_IMM(_6502 *mos6502) {
	//printf("ADC_IMM\n");
	uint8_t operand = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1);
	uint16_t res = mos6502->A + operand + mos6502->get_flag(mos6502, 'C');
	mos6502->set_flag(mos6502, 'C', res > 255);
	// signOverflow: sameSign (+*..) sameSign = diffSign
	mos6502->set_flag(mos6502, 'V', (((operand ^ (res & 0xFF)) & 0x80) && !((operand ^ mos6502->A) & 0x80)));
	mos6502->A = res & 0xFF;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 2;
	return 2;
}

/*
	ROR - op0x6A
	ACCUMULATOR
	1 Byte, 2 Cycles
*/
uint8_t	ROR_ACC(_6502* mos6502) {
	//printf("ROR_ACC\n");
	uint8_t	carry_in = mos6502->get_flag(mos6502, 'C');

	mos6502->set_flag(mos6502, 'C', mos6502->A & 0x1);
	mos6502->A = mos6502->A >> 0x1 | carry_in;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', (mos6502->A >> 0x7) & 0x1);
	
	mos6502->PC += 1;
	return 2;
}

/*
	JMP - op0x6C
	INDIRECT
	3 Bytes, 5 Cycles
*/
uint8_t	JMP_IND(_6502 *mos6502) {
	//printf("JMP_IND\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
		high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2);
	uint16_t	addr = mos6502->bus->read(mos6502->bus->ram, high_byte << 0x8 | low_byte);
	mos6502->PC = addr & 0x00FF | addr >> 0x8;
	return 5;
}

/*
	ADC - op0x6D
	ABSOLUTE
	3 Bytes, 4 Cycles
*/
uint8_t	ADC_ABS(_6502 *mos6502) {
	//printf("ADC_ABS\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2);
	uint8_t operand = mos6502->bus->read(mos6502->bus->ram, high_byte << 0x8 | low_byte);
	uint16_t res = mos6502->A + operand + mos6502->get_flag(mos6502, 'C');
	mos6502->set_flag(mos6502, 'C', res > 255);
	mos6502->set_flag(mos6502, 'V', (((operand ^ (res & 0xFF)) & 0x80) && !((operand ^ mos6502->A) & 0x80)));
	mos6502->A = res & 0xFF;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 3;
	return 4;
}

/*
	ROR - op0x6E
	ABSOLUTE
	3 Bytes, 6 Cycles
*/
uint8_t	ROR_ABS(_6502 *mos6502) {
	//printf("ROR_ABS\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
		high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2);
	uint8_t	operand = mos6502->bus->read(mos6502->bus->ram, high_byte << 0x8 | low_byte),
		carry_in = mos6502->get_flag(mos6502, 'C');

	mos6502->set_flag(mos6502, 'C', operand & 0x1);
	operand  = operand >> 0x1 | carry_in;

	mos6502->set_flag(mos6502, 'Z', operand == 0);
	mos6502->set_flag(mos6502, 'N', (operand >> 0x7) & 0x1);
	
	mos6502->bus->write(mos6502->bus->ram, high_byte << 0x8 | low_byte, operand);
	mos6502->PC += 3;
	return 6;
}

/*
	BVS - op0x70
	RELATIVE
	2 Bytes, 2** Cycles
*/
uint8_t	BVS_REL(_6502 *mos6502) {
	//printf("BVS_REL\n");
	mos6502->PC += 2;
	if (mos6502->get_flag(mos6502, 'V')) {
		uint8_t operand = mos6502->bus->read(mos6502->bus->ram, mos6502->PC-1);
		mos6502->PC += operand;
		return 3;
	}
	return 2;
}

/*
	ADC - op0x71
	INDIRECT INDEXED
	2 Bytes, 5* Cycles
*/
uint8_t	ADC_INDY(_6502 *mos6502) {
	//printf("ADC_INDY\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1), high_byte;
	uint8_t	base = 0x00 << 0x8 | low_byte, cycles = 5;
	low_byte = mos6502->bus->read(mos6502->bus->ram, base);
	high_byte = mos6502->bus->read(mos6502->bus->ram, (base+1) & 0xFF);
	uint16_t	addr = (high_byte << 0x8 | low_byte) + mos6502->Y;
	cycles += ((addr & 0xFF00) != ((high_byte << 0x8 | low_byte) & 0xFF00));
	uint8_t	operand = mos6502->bus->read(mos6502->bus->ram, addr);
	uint16_t	res = mos6502->A + operand + mos6502->get_flag(mos6502, 'C');
	mos6502->set_flag(mos6502, 'C', res > 255);
	mos6502->set_flag(mos6502, 'V', (((operand ^ (res & 0xFF)) & 0x80) && !((operand ^ mos6502->A) & 0x80)));
	mos6502->A = res & 0xFF;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 2;
	return cycles;
}

/*
	ADC - op0x75
	ZERO PAGE X
	2 Bytes, 4 Cycles
*/
uint8_t	ADC_ZPX(_6502 *mos6502) {
	//printf("ASC_ZPX\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
		operand = mos6502->bus->read(mos6502->bus->ram, (0x00 << 0x8 | low_byte) + mos6502->X);
	uint16_t res = mos6502->A + operand + mos6502->get_flag(mos6502, 'C');
	mos6502->set_flag(mos6502, 'C', res > 255);
	mos6502->set_flag(mos6502, 'V', (((operand ^ (res & 0xFF)) & 0x80) && !((operand ^ mos6502->A) & 0x80)));
	mos6502->A = res & 0xFF;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 2;
	return 4;
}

/*
	ROR - op0x76
	ZERO PAGE X
	2 Bytes, 6 Cycles
*/
uint8_t	ROR_ZPX(_6502 *mos6502) {
	//printf("ROR_ZPX\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1);
	uint16_t	addr = (0x00 << 0x8 | low_byte) + mos6502->X;
	uint8_t	operand = mos6502->bus->read(mos6502->bus->ram, addr),
		carry_in = mos6502->get_flag(mos6502, 'C');

	mos6502->set_flag(mos6502, 'C', operand & 0x1);
	operand  = operand >> 0x1 | carry_in;


	mos6502->set_flag(mos6502, 'Z', operand == 0);
	mos6502->set_flag(mos6502, 'N', (operand >> 0x7) & 0x1);
	
	mos6502->bus->write(mos6502->bus->ram, addr, operand);
	mos6502->PC += 2;
	return 6;
}

/*
	SEI - op0x78
	IMPLIED
	1 Byte, 2 Cycles
*/
uint8_t	SEI_IMP(_6502 *mos6502) {
	//printf("SEI_IMP\n");
	mos6502->set_flag(mos6502, 'I', 1);
	mos6502->PC += 1;
	return 2;
}

/*
	ADC - op0x79 HoliShit!
	ABSOLUTE Y
	3 Bytes, 4* Cycles
*/
uint8_t	ADC_ABSY(_6502* mos6502) {
	//printf("ADC_ABSY\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
		high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2),
		cycles = 4, operand;
	uint16_t absy_addr = high_byte << 0x8 | low_byte, res;
	absy_addr += mos6502->Y;
	cycles += ((absy_addr & 0xFF00) != ((high_byte << 0x8 | low_byte) & 0xFF00)); // page crossing
	operand = mos6502->bus->read(mos6502->bus->ram, absy_addr);
	res = mos6502->A + operand + mos6502->get_flag(mos6502, 'C');
	mos6502->set_flag(mos6502, 'C', res > 255);
	mos6502->set_flag(mos6502, 'V', (((operand ^ (res & 0xFF)) & 0x80) && !((operand ^ mos6502->A) & 0x80)));
	mos6502->A = res & 0xFF;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 3;
	return cycles;
}

/*
	ADC - op0x7D
	ABSOLUTE X
	3 Bytes, 4* Cycles
*/
uint8_t	ADC_ABSX(_6502 *mos6502) {
	//printf("ADC_ABSX\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
		high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2),
		cycles = 4, operand;
	uint16_t absx_addr = high_byte << 0x8 | low_byte, res;
	absx_addr += mos6502->X;
	cycles += ((absx_addr & 0xFF00) != ((high_byte << 0x8 | low_byte) & 0xFF00));
	operand = mos6502->bus->read(mos6502->bus->ram, absx_addr);
	res = mos6502->A + operand + mos6502->get_flag(mos6502, 'C');
	mos6502->set_flag(mos6502, 'C', res > 255);
	mos6502->set_flag(mos6502, 'V', (((operand ^ (res & 0xFF)) & 0x80) && !((operand ^ mos6502->A) & 0x80)));
	mos6502->A = res & 0xFF;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 3;
	return cycles;
}

/*
	ROR - op0x7E
	ABSOLUTE X
	3 Bytes, 7 Cycles
*/
uint8_t	ROR_ABSX(_6502 *mos6502) {
	//printf("ROR_ABSX\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
		high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2);
	uint16_t	absx_addr = (high_byte << 0x8 | low_byte) + mos6502->X;
	uint8_t	operand = mos6502->bus->read(mos6502->bus->ram, absx_addr),
		carry_in = mos6502->get_flag(mos6502, 'C');

	mos6502->set_flag(mos6502, 'C', operand & 0x1);
	operand  = operand >> 0x1 | carry_in;

	mos6502->set_flag(mos6502, 'Z', operand == 0);
	mos6502->set_flag(mos6502, 'N', (operand >> 0x7) & 0x1);
	
	mos6502->bus->write(mos6502->bus->ram, absx_addr, operand);
	mos6502->PC += 3;
	return 7;
}

/*
	STA - op0x81
	INDEXED INDIRECT
	2 Bytes, 6 Cycles
*/
uint8_t	STA_INDX(_6502 *mos6502) {
	//printf("STA_INDX\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1), high_byte;
	uint8_t	base = (low_byte + mos6502->X) & 0xFF;
	low_byte = mos6502->bus->read(mos6502->bus->ram, base);
	high_byte = mos6502->bus->read(mos6502->bus->ram, (base+1) & 0xFF);
	uint8_t	operand = mos6502->bus->read(mos6502->bus->ram, high_byte << 0x8 | low_byte);
	mos6502->bus->write(mos6502->bus->ram, operand, mos6502->A);
	mos6502->PC += 2;
	return 6;
}

/*
	STY - op0x84
	ZERO PAGE
	2 Bytes, 3 Cycles
*/
uint8_t	STY_ZP(_6502 *mos6502) {
	//printf("STY_ZP\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1);
	mos6502->bus->write(mos6502->bus->ram, 0x00 << 0x8 | low_byte, mos6502->Y);
	mos6502->PC += 2;
	return 3;
}

/*
	STA - op0x85
	ZERO PAGE
	2 Bytes, 3 Cycles
*/
uint8_t	STA_ZP(_6502 *mos6502) {
	//printf("STA_ZP\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1);
	mos6502->bus->write(mos6502->bus->ram, 0x00 << 0x8 | low_byte, mos6502->A);
	mos6502->PC += 2;
	return 3;
}

/*
	STX - op0x86
	ZERO PAGE
	2 Bytes, 3 Cycles
*/
uint8_t	STX_ZP(_6502 *mos6502) {
	//printf("STX_ZP\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1);
	mos6502->bus->write(mos6502->bus->ram, 0x00 << 0x8 | low_byte, mos6502->X);
	mos6502->PC += 2;
	return 3;
}

/*
	DEY - op0x88
	IMPLIED
	1 Byte, 2 Cycles
*/
uint8_t	DEY_IMP(_6502* mos6502) {
	//printf("DEY_IMP\n");
	mos6502->Y -= 1;
	mos6502->set_flag(mos6502, 'Z', mos6502->Y == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->Y & 0x80);
	mos6502->PC += 1;
	return 2;
}

/*
	TXA - op0x8A
	IMPLIED
	1 Byte, 2 Cycles
*/
uint8_t	TXA_IMP(_6502 *mos6502) {
	//printf("TXA_IMP\n");
	mos6502->A = mos6502->X;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 1;
	return 2;
}

/*
	STY - op0x8C
	ABSOLUTE
	3 Bytes, 4 Cycles
*/
uint8_t	STY_ABS(_6502 *mos6502) {
	//printf("STY_ABS\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2);
	mos6502->bus->write(mos6502->bus->ram, high_byte << 0x8 | low_byte, mos6502->Y);
	mos6502->PC += 3;
	return 4;
}

/*
	STA - op0x8D
	ABSOLUTE
	3 Bytes, 4 Cycles
*/
uint8_t	STA_ABS(_6502 *mos6502) {
	//printf("STA_ABS\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2);
	mos6502->bus->write(mos6502->bus->ram, high_byte << 0x8 | low_byte, mos6502->A);
	mos6502->PC += 3;
	return 4;
}

/*
	STX - op0x8E
	ABSOLUTE
	3 Bytes, 4 Cycles
*/
uint8_t	STX_ABS(_6502 *mos6502) {
	//printf("STX_ABS\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2);
	mos6502->bus->write(mos6502->bus->ram, high_byte << 0x8 | low_byte, mos6502->X);
	mos6502->PC += 3;
	return 4;
}

/*
	BCC - op0x90
	RELATIVE
	2 Bytes, 2** Cycles
*/
uint8_t	BCC_REL(_6502 *mos6502) {
	//printf("BCC_REL\n");
	mos6502->PC += 2;
	if (mos6502->get_flag(mos6502, 'C') == 0) {
		uint8_t operand = mos6502->bus->read(mos6502->bus->ram, mos6502->PC-1);
		mos6502->PC += operand;
		return 3;
	}
	return 2;
}

/*
	STA - op0x91
	INDIRECT INDEXED
	2 Bytes, 6 Cycles
*/
uint8_t	STA_INDY(_6502 *mos6502) {
	//printf("STA_INDY\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1), high_byte;
	uint8_t	base = 0x00 << 0x8 | low_byte, cycles = 5;
	low_byte = mos6502->bus->read(mos6502->bus->ram, base);
	high_byte = mos6502->bus->read(mos6502->bus->ram, (base+1) & 0xFF);
	uint16_t	addr = (high_byte << 0x8 | low_byte) + mos6502->Y;
	cycles += ((addr & 0xFF00) != ((high_byte << 0x8 | low_byte) & 0xFF00));
	mos6502->bus->write(mos6502->bus->ram, addr, mos6502->A);
	mos6502->PC += 2;
	return 6;
}

/*
	STY - op0x94
	ZERO PAGE X
	2 Bytes, 4 Cycles
*/
uint8_t	STY_ZPX(_6502 *mos6502) {
	//printf("STY_ZPX\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1);
	mos6502->bus->write(mos6502->bus->ram, (0x00 << 0x8 | low_byte) + mos6502->X, mos6502->Y);
	mos6502->PC += 2;
	return 4;
}

/*
	STA - op0x95
	ZERO PAGE X
	2 Bytes, 4 Cycles
*/
uint8_t	STA_ZPX(_6502 *mos6502) {
	//printf("STA_ZPX\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1);
	mos6502->bus->write(mos6502->bus->ram, (0x00 << 0x8 | low_byte) + mos6502->X, mos6502->A);
	mos6502->PC += 2;
	return 4;
}

/*
	STX - op0x96
	ZERO PAGE Y
	2 Bytes, 4 Cycles
*/
uint8_t	STX_ZPY(_6502 *mos6502) {
	//printf("STX_ZPY\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1);
	mos6502->bus->write(mos6502->bus->ram, (0x00 << 0x8 | low_byte) + mos6502->Y, mos6502->X);
	mos6502->PC += 2;
	return 4;
}

/*
	TYA - op0x98
	IMPLIED
	1 Byte, 2 Cycles
*/
uint8_t	TYA_IMP(_6502 *mos6502) {
	//printf("TYA_IMP\n");
	mos6502->A = mos6502->Y;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 1;
	return 2;
}

/*
	STA - op0x99
	ABSOLUTE Y
	3 Bytes, 5 Cycles
*/
uint8_t	STA_ABSY(_6502 *mos6502) {
	//printf("STA_ABSY\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2);
	mos6502->bus->write(mos6502->bus->ram, (high_byte << 0x8 | low_byte) + mos6502->Y, mos6502->A);
	mos6502->PC += 3;
	return 5;
}

/*
	TXS - op0x9A
	IMPLIED
	1 Byte, 2 Cycles
*/
uint8_t	TXS_IMP(_6502 *mos6502) {
	//printf("TXS_IMP\n");
	mos6502->SP = mos6502->X;
	mos6502->set_flag(mos6502, 'Z', mos6502->SP == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->SP & 0x80);
	mos6502->PC += 1;
	return 2;
}

/*
	STA - op0x9D
	ABSOLUTE X
	3 Bytes, 5 Cycles
*/
uint8_t	STA_ABSX(_6502 *mos6502) {
	//printf("STA_ABSX\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2);
	mos6502->bus->write(mos6502->bus->ram, (high_byte << 0x8 | low_byte) + mos6502->X, mos6502->A);
	mos6502->PC += 3;
	return 5;
}

/*
	LDY - op0xA0
	IMMEDIATE
	2 Bytes, 2 Cycles
*/
uint8_t	LDY_IMM(_6502 *mos6502) {
	//printf("LDY_IMM\n");
	uint8_t operand = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1);
	mos6502->Y = operand;
	mos6502->set_flag(mos6502, 'Z', mos6502->Y == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->Y & 0x80);
	mos6502->PC += 2;
	return 2;
}

/*
	LDA - op0xA1
	INDEXED INDIRECT
	2 Bytes, 6 Cycles
*/
uint8_t	LDA_INDX(_6502 *mos6502) {
	//printf("LDA_INDX\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1), high_byte;
	uint8_t	base = (low_byte + mos6502->X) & 0xFF;
	low_byte = mos6502->bus->read(mos6502->bus->ram, base);
	high_byte = mos6502->bus->read(mos6502->bus->ram, (base+1) & 0xFF);
	uint8_t	operand = mos6502->bus->read(mos6502->bus->ram, high_byte << 0x8 | low_byte);
	mos6502->A = operand;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC  += 2;
	return 6;
}

/*
	LDX - op0xA2
	IMMEDIATE
	2 Bytes, 2 Cycles
*/
uint8_t	LDX_IMM(_6502 *mos6502) {
	//printf("LDX_IMM\n");
	uint8_t operand = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1);
	mos6502->X = operand;
	mos6502->set_flag(mos6502, 'Z', mos6502->X == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->X & 0x80);
	mos6502->PC += 2;
	return 2;
}

/*
	LDY - op0xA4
	ZERO PAGE
	2 Bytes, 3 Cycles
*/
uint8_t	LDY_ZP(_6502 *mos6502) {
	//printf("LDY_ZP\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        operand = mos6502->bus->read(mos6502->bus->ram, 0x00 << 0x8 | low_byte);
	mos6502->Y = operand;
	mos6502->set_flag(mos6502, 'Z', mos6502->Y == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->Y & 0x80);
	mos6502->PC += 2;
	return 3;
}

/*
	LDA - op0xA5
	ZERO PAGE
	2 Bytes, 3 Cycles
*/
uint8_t	LDA_ZP(_6502 *mos6502) {
	//printf("LDA_ZP\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
		operand = mos6502->bus->read(mos6502->bus->ram, 0x00 << 0x8 | low_byte);
	mos6502->A = operand;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 2;
	return 3;
}

/*
	LDX - op0xA6
	ZERO PAGE
	2 Bytes, 3 Cycles
*/
uint8_t	LDX_ZP(_6502 *mos6502) {
	//printf("LDX_ZP\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
		operand = mos6502->bus->read(mos6502->bus->ram, 0x00 << 0x8 | low_byte);
	mos6502->X = operand;
	mos6502->set_flag(mos6502, 'Z', mos6502->X == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->X & 0x80);
	mos6502->PC += 2;
	return 3;
}

/*
	TAY - op0xA8
	IMPLIED
	1 Byte, 2 Cycles
*/
uint8_t	TAY_IMP(_6502 *mos6502) {
	//printf("TAY_IMP\n");
	mos6502->Y = mos6502->A;
	mos6502->set_flag(mos6502, 'Z', mos6502->Y == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->Y & 0x80);
	mos6502->PC += 1;
	return 2;
}

/*
	LDA - op0xA9
	IMMEDIATE
	2 Bytes, 2 Cycles
*/
uint8_t	LDA_IMM(_6502 *mos6502) {
	//printf("LDA_IMM\n");
	uint8_t operand = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1);
	mos6502->A = operand;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 2;
	return 2;
}

/*
	TAX - op0xAA
	IMPLIED
	1 Byte, 2 Cycles
*/
uint8_t	TAX_IMP(_6502 *mos6502) {
	//printf("TAX_IMP\n");
	mos6502->X = mos6502->A;
	mos6502->set_flag(mos6502, 'Z', mos6502->X == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->X & 0x80);
	mos6502->PC += 1;
	return 2;
}

/*
	LDY - op0xAC
	ABSOLUTE
	3 Bytes, 4 Cycles
*/
uint8_t	LDY_ABS(_6502 *mos6502) {
	//printf("LDY_ABS\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
		high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2),
		operand = mos6502->bus->read(mos6502->bus->ram, high_byte << 0x8 | low_byte);
	mos6502->Y = operand;
	mos6502->set_flag(mos6502, 'Z', mos6502->Y == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->Y & 0x80);
	mos6502->PC += 3;
	return 4;
}

/*
	LDA - op0xAD
	ABSOLUTE
	3 Bytes, 4 Cycles
*/
uint8_t	LDA_ABS(_6502 *mos6502) {
	//printf("LDA_ABS\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
		high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2),
		operand = mos6502->bus->read(mos6502->bus->ram, high_byte << 0x8 | low_byte);
	mos6502->A = operand;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 3;
	return 4;
}

/*
	LDX - op0xAE
	ABSOLUTE
	3 Bytes, 4 Cycles
*/
uint8_t	LDX_ABS(_6502 *mos6502) {
	//printf("LDX_ABS\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
		high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2),
		operand = mos6502->bus->read(mos6502->bus->ram, high_byte << 0x8 | low_byte);
	mos6502->X = operand;
	mos6502->set_flag(mos6502, 'Z', mos6502->X == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->X & 0x80);
	mos6502->PC += 3;
	return 4;
}

/*
	BCS - op0xB0
	RELATIVE
	2 Bytes, 2** Cycles
*/
uint8_t	BCS_REL(_6502 *mos6502) {
	//printf("BCS_REL\n");
	mos6502->PC += 2;
	if (mos6502->get_flag(mos6502, 'C')) {
		uint8_t operand = mos6502->bus->read(mos6502->bus->ram, mos6502->PC-1);
		mos6502->PC += operand;
		return 3;
	}
	return 2;
}

/*
	LDA - op0xB1
	INDIRECT INDEXED
	2 Bytes, 5* Cycles
*/
uint8_t	LDA_INDY(_6502 *mos6502) {
	//printf("LDA_INDY\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1), high_byte;
	uint8_t	base = 0x00 << 0x8 | low_byte, cycles = 5;
	low_byte = mos6502->bus->read(mos6502->bus->ram, base);
	high_byte = mos6502->bus->read(mos6502->bus->ram, (base+1) & 0xFF);
	uint16_t	addr = (high_byte << 0x8 | low_byte) + mos6502->Y;
	cycles += ((addr & 0xFF00) != ((high_byte << 0x8 | low_byte) & 0xFF00));
	uint8_t	operand = mos6502->bus->read(mos6502->bus->ram, addr);
	mos6502->A = operand;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 2;
	return cycles;
}

/*
	LDY - op0xB4
	ZERO PAGE X
	2 Bytes, 4 Cycles
*/
uint8_t	LDY_ZPX(_6502 *mos6502) {
	//printf("LDY_ZPX\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        operand = mos6502->bus->read(mos6502->bus->ram, (0x00 << 0x8 | low_byte) + mos6502->X);
	mos6502->Y = operand;
	mos6502->set_flag(mos6502, 'Z', mos6502->Y == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->Y & 0x80);
	mos6502->PC += 2;
	return 4;
}

/*
	LDA - op0xB5
	ZERO PAGE X
	2 Bytes, 4 Cycles
*/
uint8_t	LDA_ZPX(_6502 *mos6502) {
	//printf("LDA_ZPX\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
		operand = mos6502->bus->read(mos6502->bus->ram, (0x00 << 0x8 | low_byte) + mos6502->X);
	mos6502->A = operand;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 2;
	return 4;
}

/*
	LDX - op0xB6
	ZERO PAGE Y
	2 Bytes, 4 Cycles
*/
uint8_t	LDX_ZPY(_6502 *mos6502) {
	//printf("LDX_ZPY\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
		operand = mos6502->bus->read(mos6502->bus->ram, (0x00 << 0x8 | low_byte) + mos6502->Y);
	mos6502->X = operand;
	mos6502->set_flag(mos6502, 'Z', mos6502->X == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->X & 0x80);
	mos6502->PC += 2;
	return 4;
}

/*
	CLV - op0xB8
	IMPLIED
	1 Byte, 2 Cycles
*/
uint8_t	CLV_IMP(_6502 *mos6502) {
	//printf("CLV_IMP\n");
	mos6502->set_flag(mos6502, 'V', 0);
	mos6502->PC += 1;
	return 2;
}

/*
	LDA - op0xB9 
	ABSOLUTE Y
	3 Bytes, 4* Cycles
*/
uint8_t	LDA_ABSY(_6502 *mos6502) {
	//printf("LDA_ABSY\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
		high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2), cycles = 4;
	uint16_t	absy_addr = (high_byte << 0x8 | low_byte) + mos6502->Y;
	cycles += ((absy_addr & 0xFF00) != ((high_byte << 0x8 | low_byte) & 0xFF00));
	uint8_t	operand = mos6502->bus->read(mos6502->bus->ram, absy_addr);
	mos6502->A = operand;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 3;
	return cycles;
}

/*
	TSX - op0xBA
	IMPLIED
	1 Byte, 2 Cycles
*/
uint8_t	TSX_IMP(_6502 *mos6502) {
	//printf("TSX_IMP\n");
	mos6502->X = mos6502->SP;
	mos6502->set_flag(mos6502, 'Z', mos6502->X == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->X & 0x80);
	mos6502->PC += 1;
	return 2;
}

/*
	LDY - op0xBC
	ABSOLUTE X
	3 Bytes, 4* Cycles
*/
uint8_t	LDY_ABSX(_6502 *mos6502) {
	//printf("LDY_ABSX\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
		high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2), cycles = 4;
	uint16_t	absx_addr = (high_byte << 0x8 | low_byte) + mos6502->X;
	cycles += ((absx_addr & 0xFF00) != ((high_byte << 0x8 | low_byte) & 0xFF00));
	uint8_t	operand = mos6502->bus->read(mos6502->bus->ram, absx_addr);
	mos6502->Y = operand;
	mos6502->set_flag(mos6502, 'Z', mos6502->Y == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->Y & 0x80);
	mos6502->PC += 3;
	return cycles;
}

/*
	LDA - op0xBD
	ABSOLUTE X
	3 Bytes, 4* Cycles
*/
uint8_t	LDA_ABSX(_6502 *mos6502) {
	//printf("LDA_ABSX\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
		high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2), cycles = 4;
	uint16_t	absx_addr = (high_byte << 0x8 | low_byte) + mos6502->X;
	cycles += ((absx_addr & 0xFF00) != ((high_byte << 0x8 | low_byte) & 0xFF00));
	uint8_t	operand = mos6502->bus->read(mos6502->bus->ram, absx_addr);
	mos6502->A = operand;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 3;
	return cycles;
}

/*
	LDX - op0xBE
	ABSOLUTE Y
	3 Bytes, 4* Cycles
*/
uint8_t	LDX_ABSY(_6502 *mos6502) {
	//printf("LDX_ABSY\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
		high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2), cycles = 4;
	uint16_t	absy_addr = (high_byte << 0x8 | low_byte) + mos6502->Y;
	cycles += ((absy_addr & 0xFF00) != ((high_byte << 0x8 | low_byte) & 0xF00));
	uint8_t operand = mos6502->bus->read(mos6502->bus->ram, absy_addr);
	mos6502->X = operand;
	mos6502->set_flag(mos6502, 'Z', mos6502->X == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->X & 0x80);
	mos6502->PC += 3;
	return 4;
}

/*
	CPY - op0xC0
	IMMEDIATE
	2 Bytes, 2 Cycles
*/
uint8_t	CPY_IMM(_6502 *mos6502) {
	//printf("CPY_IMM\n");
	uint8_t operand = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1);
	uint8_t res = mos6502->Y - operand;
	mos6502->set_flag(mos6502, 'C', mos6502->Y >= operand);
	mos6502->set_flag(mos6502, 'Z', mos6502->Y == operand);
	mos6502->set_flag(mos6502, 'N', res & 0x80);
	mos6502->PC += 2;
	return 2;
}

/*
	CMP - op0xC1
	INDEXED INDIRECT
	2 Bytes, 6 Cycles
*/
uint8_t	CMP_INDX(_6502 *mos6502) {
	//printf("CMP_INDX\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1), high_byte;
	uint8_t	base = (low_byte + mos6502->X) & 0xFF;
	low_byte = mos6502->bus->read(mos6502->bus->ram, base);
	high_byte = mos6502->bus->read(mos6502->bus->ram, (base+1) & 0xFF);
	uint8_t	operand = mos6502->bus->read(mos6502->bus->ram, high_byte << 0x8 | low_byte);
	uint8_t	res = mos6502->A - operand;
	mos6502->set_flag(mos6502, 'C', mos6502->A >= operand);
	mos6502->set_flag(mos6502, 'Z', mos6502->A == operand);
	mos6502->set_flag(mos6502, 'N', res & 0x80);
	mos6502->PC += 2;
	return 6;
}

/*
	CPY - op0xC4
	ZERO PAGE
	2 Bytes, 3 Cycles
*/
uint8_t	CPY_ZP(_6502 *mos6502) {
	//printf("CPY_ZP\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        operand = mos6502->bus->read(mos6502->bus->ram, 0x00 << 0x8 | low_byte);
	uint8_t res = mos6502->Y - operand;
	mos6502->set_flag(mos6502, 'C', mos6502->Y >= operand);
	mos6502->set_flag(mos6502, 'Z', mos6502->Y == operand);
	mos6502->set_flag(mos6502, 'N', res & 0x80);
	mos6502->PC += 2;
	return 3;
}

/*
	CMP - op0xC5
	ZERO PAGE
	2 Bytes, 3 Cycles
*/
uint8_t	CMP_ZP(_6502 *mos6502) {
	//printf("CMP_ZP\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        operand = mos6502->bus->read(mos6502->bus->ram, 0x00 << 0x8 | low_byte);
	uint8_t res = mos6502->A - operand;
	mos6502->set_flag(mos6502, 'C', mos6502->A >= operand);
	mos6502->set_flag(mos6502, 'Z', mos6502->A == operand);
	mos6502->set_flag(mos6502, 'N', res & 0x80);
	mos6502->PC += 2;
	return 3;
}

/*
	DEC - op0xC6
	ZERO PAGE
	2 Bytes, 5 Cycles
*/
uint8_t	DEC_ZP(_6502 *mos6502) {
	//printf("DEC_ZP\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1);
	uint8_t operand = mos6502->bus->read(mos6502->bus->ram, 0x00 << 0x8 | low_byte);
	operand -= 1;
	mos6502->set_flag(mos6502, 'Z', operand == 0);
	mos6502->set_flag(mos6502, 'N', operand & 0x80);
	mos6502->bus->write(mos6502->bus->ram, 0x00 << 0x8 | low_byte, operand);
	mos6502->PC += 2;
	return 5;
}

/*
	INY - op0xC8
	IMPLIED
	1 Byte, 2 Cycles
*/
uint8_t	INY_IMP(_6502 *mos6502) {
	//printf("INY_IMP\n");
	mos6502->Y += 1;
	mos6502->set_flag(mos6502, 'Z', mos6502->Y == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->Y & 0x80);
	mos6502->PC += 1;
	return 2;
}

/*
	CMP - op0xC9
	IMMEDIATE
	2 Bytes, 2 Cycles
*/
uint8_t	CMP_IMM(_6502 *mos6502) {
	//printf("CMP_IMM\n");
	uint8_t operand = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1);
	mos6502->set_flag(mos6502, 'C', mos6502->A >= operand);
	uint8_t res = mos6502->A - operand;
	mos6502->set_flag(mos6502, 'Z', mos6502->A == operand);
	mos6502->set_flag(mos6502, 'N', res & 0x80);
	mos6502->PC += 2;
	return 2;
}

/*
	DEX - op0xCA
	IMPLIED
	1 Byte, 2 Cycles
*/
uint8_t	DEX_IMP(_6502 *mos6502) {
	//printf("DEX_IMP\n");
	mos6502->X -= 1;
	mos6502->set_flag(mos6502, 'Z', mos6502->X == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->X & 0x80);
	mos6502->PC += 1;
	return 2;
}

/*
	CPY - op0xCC
	ABSOLUTE
	3 Bytes, 4 Cycles
*/
uint8_t	CPY_ABS(_6502 *mos6502) {
	//printf("CPY_ABS\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2);
	uint8_t operand = mos6502->bus->read(mos6502->bus->ram, high_byte << 8 | low_byte);
	uint8_t res = mos6502->Y - operand;
	mos6502->set_flag(mos6502, 'C', mos6502->Y >= operand);
	mos6502->set_flag(mos6502, 'Z', mos6502->Y == operand);
	mos6502->set_flag(mos6502, 'N', res & 0x80);
	mos6502->PC += 3;
	return 4;
}

/*
	CMP - op0xCD
	ABSOLUTE
	3 Bytes, 4 Cycles
*/
uint8_t	CMP_ABS(_6502 *mos6502) {
	//printf("CMP_ABS\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2);
	uint8_t operand = mos6502->bus->read(mos6502->bus->ram, high_byte << 0x8 | low_byte);
	uint8_t res = mos6502->A - operand;
	mos6502->set_flag(mos6502, 'C', mos6502->A >= operand);
	mos6502->set_flag(mos6502, 'Z', mos6502->A == operand);
	mos6502->set_flag(mos6502, 'N', res & 0x80);
	mos6502->PC += 3;
	return 4;
}

/*
	DEC - op0xCE
	ABSOLUTE
	3 Bytes, 6 Cycles
*/
uint8_t	DEC_ABS(_6502 *mos6502) {
	//printf("DEC_ABS\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2);
	uint8_t operand = mos6502->bus->read(mos6502->bus->ram, high_byte << 0x8 | low_byte);
	operand -= 1;
	mos6502->set_flag(mos6502, 'Z', operand == 0);
	mos6502->set_flag(mos6502, 'N', operand & 0x80);
	mos6502->bus->write(mos6502->bus->ram, high_byte << 0x8 | low_byte, operand);
	mos6502->PC += 3;
	return 6;
}

/*
	BNE - op0xD0
	RELATIVE
	2 Bytes, 2** Cycles
*/
uint8_t	BNE_REL(_6502 *mos6502) {
	//printf("BNE_REL\n");
	mos6502->PC += 2;
	if (mos6502->get_flag(mos6502, 'Z') == 0) {
		uint8_t operand = mos6502->bus->read(mos6502->bus->ram, mos6502->PC-1);
		mos6502->PC += operand;
		return 3;
	}
	return 2;
}

/*
	CMP - op0xD1
	INDIRECT INDEXED
	2 Bytes, 5* Cycles
*/
uint8_t	CMP_INDY(_6502 *mos6502) {
	//printf("CMP_INDY\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1), high_byte;
	uint8_t	base = 0x00 << 0x8 | low_byte, cycles = 5;
	low_byte = mos6502->bus->read(mos6502->bus->ram, base);
	high_byte = mos6502->bus->read(mos6502->bus->ram, (base+1) & 0xFF);
	uint16_t	addr = (high_byte << 0x8 | low_byte) + mos6502->Y;
	cycles += ((addr & 0xFF00) != ((high_byte << 0x8 | low_byte) & 0xFF00));
	uint8_t	operand = mos6502->bus->read(mos6502->bus->ram, addr);
	uint8_t res = mos6502->A - operand;
	mos6502->set_flag(mos6502, 'C', mos6502->A >= operand);
	mos6502->set_flag(mos6502, 'Z', mos6502->A == operand);
	mos6502->set_flag(mos6502, 'N', res & 0x80);
	mos6502->PC += 2;
	return cycles;
}

/*
	CMP - op0xD5
	ZERO PAGE X
	2 Bytes, 4 Cycles	
*/
uint8_t	CMP_ZPX(_6502 *mos6502) {
	//printf("CMP_ZPX\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        operand = mos6502->bus->read(mos6502->bus->ram, (0x00 << 0x8 | low_byte) + mos6502->X);
	uint8_t res = mos6502->A - operand;
	mos6502->set_flag(mos6502, 'C', mos6502->A >= operand);
	mos6502->set_flag(mos6502, 'Z', mos6502->A == operand);
	mos6502->set_flag(mos6502, 'N', res & 0x80);
	mos6502->PC += 2;
	return 4;
}

/*
	DEC - op0xD6
	ZERO PAGE X
	2 Bytes, 6 Cycles
*/
uint8_t	DEC_ZPX(_6502 *mos6502) {
	//printf("DEC_ZPX\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1);
	uint16_t	addr = (0x00 << 0x8 | low_byte) + mos6502->X;
	uint8_t	operand = mos6502->bus->read(mos6502->bus->ram, addr);
	operand -= 1;
	mos6502->set_flag(mos6502, 'Z', operand == 0);
	mos6502->set_flag(mos6502, 'N', operand & 0x80);
	mos6502->bus->write(mos6502->bus->ram, addr, operand);
	mos6502->PC += 2;
	return 6;
}

/*
	CLD - op0xD8
	IMPLIED
	1 Bytes, 2 Cycles
*/
uint8_t	CLD_IMP(_6502 *mos6502) {
	//printf("CLD_IMP\n");
	mos6502->set_flag(mos6502, 'D', 0);
	mos6502->PC += 1;
	return 2;
}

/*
	CMP - op0xD9
	ABSOLUTE Y
	2 Bytes, 4* Cycles
*/
uint8_t	CMP_ABSY(_6502 *mos6502) {
	//printf("CMP_ABSY\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2), cycles = 4;
	uint16_t	absy_addr = (high_byte << 0x8 | low_byte) + mos6502->Y;
	cycles += ((absy_addr & 0xFF00) != ((high_byte << 0x8 | low_byte) & 0xFF00));
	uint8_t operand = mos6502->bus->read(mos6502->bus->ram, absy_addr);
	uint8_t res = mos6502->A - operand;
	mos6502->set_flag(mos6502, 'C', mos6502->A >= operand);
	mos6502->set_flag(mos6502, 'Z', mos6502->A == operand);
	mos6502->set_flag(mos6502, 'N', res & 0x80);
	mos6502->PC += 2;
	return cycles;
}

/*
	CMP - op0xDD
	ABSOLUTE X
	2 Bytes, 4* Cycles
*/
uint8_t	CMP_ABSX(_6502 *mos6502) {
	//printf("CMP_ABSX\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2), cycles = 4;
	uint16_t	absx_addr = (high_byte << 0x8 | low_byte) + mos6502->X;
	cycles += ((absx_addr & 0xFF00) != ((high_byte << 0x8 | low_byte) & 0xFF00));
	uint8_t operand = mos6502->bus->read(mos6502->bus->ram, absx_addr);
	uint8_t res = mos6502->A - operand;
	mos6502->set_flag(mos6502, 'C', mos6502->A >= operand);
	mos6502->set_flag(mos6502, 'Z', mos6502->A == operand);
	mos6502->set_flag(mos6502, 'N', res & 0x80);
	mos6502->PC += 2;
	return cycles;
}

/*
	DEC - op0xDE
	ABSOLUTE X
	3 Bytes, 7 Cycles
*/
uint8_t	DEC_ABSX(_6502 *mos6502) {
	//printf("DEC_ABSX\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
		high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2);
	uint16_t	absx_addr = (high_byte << 0x8 | low_byte) + mos6502->X;
	uint8_t	operand = mos6502->bus->read(mos6502->bus->ram, absx_addr);
	operand -= 1;
	mos6502->set_flag(mos6502, 'Z', operand == 0);
	mos6502->set_flag(mos6502, 'N', operand & 0x80);
	mos6502->bus->write(mos6502->bus->ram, absx_addr, operand);
	mos6502->PC += 3;
	return 7;
}

/*
	CPX - op0xE0
	IMMEDIATE
	2 Bytes, 2 Cycles
*/
uint8_t	CPX_IMM(_6502 *mos6502) {
	//printf("CPX_IMM\n");
	uint8_t operand = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1);
	uint8_t res = mos6502->X - operand;
	mos6502->set_flag(mos6502, 'C', mos6502->X >= operand);
	mos6502->set_flag(mos6502, 'Z', mos6502->X == operand);
	mos6502->set_flag(mos6502, 'N', res & 0x80);
	mos6502->PC += 2;
	return 2;
}

/*
	SBC - op0xE1
	INDEXED INDIRECT
	2 Bytes, 6 Cycles
*/
uint8_t	SBC_INDX(_6502 *mos6502) {
	//printf("SBC_INDX\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1), high_byte;
	uint8_t	base = (low_byte + mos6502->X) & 0xFF;
	low_byte = mos6502->bus->read(mos6502->bus->ram, base);
	high_byte = mos6502->bus->read(mos6502->bus->ram, (base+1) & 0xFF);
	uint8_t	operand = mos6502->bus->read(mos6502->bus->ram, high_byte << 0x8 | low_byte);
	mos6502->A -= operand - (1 - mos6502->get_flag(mos6502, 'C'));
	mos6502->set_flag(mos6502, 'C', mos6502->A >= operand);
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 2;
	return 6;
}

/*
	CPX - op0xE4
	ZERO PAGE
	2 Bytes, 3 Cycles
*/
uint8_t	CPX_ZP(_6502 *mos6502) {
	//printf("CPX_ZP\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        operand = mos6502->bus->read(mos6502->bus->ram, 0x00 << 0x8 | low_byte);
	uint8_t res = mos6502->X - operand;
	mos6502->set_flag(mos6502, 'C', mos6502->X >= operand);
	mos6502->set_flag(mos6502, 'Z', mos6502->X == operand);
	mos6502->set_flag(mos6502, 'N', res & 0x80);
	mos6502->PC += 2;
	return 3;
}

/*
	SBC - op0xE5
	ZERO PAGE
	2 Bytes, 3 Cycles
*/
uint8_t	SBC_ZP(_6502 *mos6502) {
	//printf("SBC_ZP\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
		operand = mos6502->bus->read(mos6502->bus->ram, 0x00 << 8 | low_byte);
	mos6502->A -= operand - (1 - mos6502->get_flag(mos6502, 'C'));
	mos6502->set_flag(mos6502, 'C', mos6502->A >= operand);
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 2;
	return 3;
}

/*
	INC - op0xE6
	ZERO PAGE
	2 Bytes, 5 Cycles
*/
uint8_t	INC_ZP(_6502 *mos6502) {
	//printf("INC_ZP\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
		operand = mos6502->bus->read(mos6502->bus->ram, 0x00 << 8 | low_byte);
	operand += 1;
	mos6502->set_flag(mos6502, 'Z', operand == 0);
	mos6502->set_flag(mos6502, 'N', operand & 0x80);
	mos6502->bus->write(mos6502->bus->ram, 0x00 << 8 | low_byte, operand);
	mos6502->PC += 2;
	return 5;
}

/*
	INX - op0xE8
	IMPLIED
	1 Byte, 2 Cycles
*/
uint8_t	INX_IMP(_6502 *mos6502) {
	//printf("INX_IMP\n");
	mos6502->X += 1;
	mos6502->set_flag(mos6502, 'Z', mos6502->X == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->X & 0x80);
	mos6502->PC += 1;
	return 2;
}

/*
	SBC - op0xE9
	IMMEDIATE
	2 Bytes, 2 Cycles
*/
uint8_t	SBC_IMM(_6502 *mos6502) {
	//printf("SBC_IMM\n");
	uint8_t operand = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1);
	mos6502->set_flag(mos6502, 'C', mos6502->A >= operand);
	mos6502->A -= operand - (1 - mos6502->get_flag(mos6502, 'C'));
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 2;
	return 2;
}

/*
	NOP - op0xEA
	IMPLIED
	1 Byte, 2 Cycles
*/
uint8_t	NOP_IMP(_6502 *mos6502) {
	//printf("NOP_IMP\n");
	mos6502->PC += 1;
	return 2;
}

/*
	CPX - op0xEC
	ABSOLUTE
	3 Bytes, 4 Cycles
*/
uint8_t	CPX_ABS(_6502 *mos6502) {
	//printf("CPX_ABS\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2);
	uint8_t operand = mos6502->bus->read(mos6502->bus->ram, high_byte << 0x8 | low_byte);
	uint8_t res = mos6502->X - operand;
	mos6502->set_flag(mos6502, 'C', mos6502->X >= operand);
	mos6502->set_flag(mos6502, 'Z', mos6502->X == operand);
	mos6502->set_flag(mos6502, 'N', res & 0x80);
	mos6502->PC += 3;
	return 4;
}

/*
	SBC - op0xED
	ABSOLUTE
	3 Bytes, 4 Cycles
*/
uint8_t	SBC_ABS(_6502 *mos6502) {
	//printf("SBC_ABS\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2);
	uint8_t operand = mos6502->bus->read(mos6502->bus->ram, high_byte << 0x8 | low_byte);
	mos6502->set_flag(mos6502, 'C', mos6502->A >= operand);
	mos6502->A -= operand - (1 - mos6502->get_flag(mos6502, 'C'));
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 3;
	return 4;
}

/*
	INC - op0xEE
	ABSOLUTE
	3 Bytes, 6 Cycles
*/
uint8_t	INC_ABS(_6502 *mos6502) {
	//printf("INC_ABS\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2);
	uint8_t operand = mos6502->bus->read(mos6502->bus->ram, high_byte << 0x8 | low_byte);
	operand += 1;
	mos6502->set_flag(mos6502, 'Z', operand == 0);
	mos6502->set_flag(mos6502, 'N', operand & 0x80);
	mos6502->bus->write(mos6502->bus->ram, high_byte << 0x8 | low_byte, operand);
	mos6502->PC += 3;
	return 6;
}

/*
	BEQ - op0xF0
	RELATIVE
	2 Bytes, 2** Cycles
*/
uint8_t	BEQ_REL(_6502 *mos6502) {
	//printf("BEQ_REL\n");
	mos6502->PC += 2;
	if (mos6502->get_flag(mos6502, 'Z')) {
		uint8_t operand = mos6502->bus->read(mos6502->bus->ram, mos6502->PC-1);
		mos6502->PC += operand;
		return 3;
	}
	return 2;
}

/*
	SBC - op0xF1
	INDIRECT INDEXED
	2 Bytes, 5* Cycles
*/
uint8_t	SBC_INDY(_6502 *mos6502) {
	//printf("SBC_INDY\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1), high_byte;
	uint8_t	base = 0x00 << 0x8 | low_byte, cycles = 5;
	low_byte = mos6502->bus->read(mos6502->bus->ram, base);
	high_byte = mos6502->bus->read(mos6502->bus->ram, (base+1) & 0xFF);
	uint16_t	addr = (high_byte << 0x8 | low_byte) + mos6502->Y;
	cycles += ((addr & 0xFF00) != ((high_byte << 0x8 | low_byte) & 0xFF00));
	uint8_t	operand = mos6502->bus->read(mos6502->bus->ram, addr);
	mos6502->A -= operand - (1 - mos6502->get_flag(mos6502, 'C'));
	mos6502->set_flag(mos6502, 'C', mos6502->A >= operand);
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 2;
	return cycles;
}

/*
	SBC - op0xF5
	ZERO PAGE X
	2 Bytes, 4 Cycles
*/
uint8_t	SBC_ZPX(_6502 *mos6502) {
	//printf("SBC_ZPX\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
		operand = mos6502->bus->read(mos6502->bus->ram, (0x00 << 8 | low_byte) + mos6502->X);
	mos6502->A -= operand - (1 - mos6502->get_flag(mos6502, 'C'));
	mos6502->set_flag(mos6502, 'C', mos6502->A >= operand);
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 2;
	return 4;
}

/*
	INC - op0xF6
	ZERO PAGE X
	2 Bytes, 6 Cycles
*/
uint8_t	INC_ZPX(_6502 *mos6502) {
	//printf("INC_ZPX\n");
	uint8_t	low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1);
	uint16_t	addr = (0x00 << 0x8 | low_byte) + mos6502->X;
	uint8_t	operand = mos6502->bus->read(mos6502->bus->ram, addr);
	operand += 1;
	mos6502->set_flag(mos6502, 'Z', operand == 0);
	mos6502->set_flag(mos6502, 'N', operand & 0x80);
	mos6502->bus->write(mos6502->bus->ram, addr, operand);
	mos6502->PC += 2;
	return 6;
}

/*
	SED - op0xF8
	IMPLIED
	1 Byte, 2 Cycles
*/
uint8_t	SED_IMP(_6502 *mos6502) {
	//printf("SED_IMP\n");
	mos6502->set_flag(mos6502, 'D', 1);
	mos6502->PC += 1;
	return 2;
}

/*
	SBC - op0xF9
	ABSOLUTE Y
	2 Bytes, 4* Cycles
*/
uint8_t	SBC_ABSY(_6502 *mos6502) {
	//printf("SBC_ABSY\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2),
	        cycles = 4;
	uint16_t absy_addr = high_byte << 0x8 | low_byte;
	absy_addr += mos6502->Y;
	cycles += ((absy_addr & 0xFF00) != ((high_byte << 0x8 | low_byte) & 0xFF00));
	uint8_t operand = mos6502->bus->read(mos6502->bus->ram, absy_addr);
	mos6502->set_flag(mos6502, 'C', mos6502->A >= operand);
	mos6502->A -= operand - (1 - mos6502->get_flag(mos6502, 'C'));
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 2;
	return cycles;
}

/*
	SBC - op0xFD
	ABSOLUTE X
	3 Bytes, 4* Cycles
*/
uint8_t	SBC_ABSX(_6502 *mos6502) {
	//printf("SBC_ABSX\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2),
	        cycles = 4;
	uint16_t absx_addr = high_byte << 0x8 | low_byte;
	absx_addr += mos6502->X;
	cycles += ((absx_addr & 0xFF00) != ((high_byte << 0x8 | low_byte) & 0xFF00));
	uint8_t operand = mos6502->bus->read(mos6502->bus->ram, absx_addr);
	mos6502->set_flag(mos6502, 'C', mos6502->A >= operand);
	mos6502->A -= operand - (1 - mos6502->get_flag(mos6502, 'C'));
	mos6502->set_flag(mos6502, 'Z', mos6502->A == 0);
	mos6502->set_flag(mos6502, 'N', mos6502->A & 0x80);
	mos6502->PC += 2;
	return cycles;
}

/*
	INC - op0xFE
	ABSOLUTE X
	3 Bytes, 7 Cycles
*/
uint8_t	INC_ABSX(_6502 *mos6502) {
	//printf("INC_ABSX\n");
	uint8_t low_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+1),
	        high_byte = mos6502->bus->read(mos6502->bus->ram, mos6502->PC+2);
	uint16_t	absx_addr = (high_byte << 0x8 | low_byte) + mos6502->X;
	uint8_t operand = mos6502->bus->read(mos6502->bus->ram, absx_addr);
	operand += 1;
	mos6502->set_flag(mos6502, 'Z', operand == 0);
	mos6502->set_flag(mos6502, 'N', operand & 0x80);
	mos6502->bus->write(mos6502->bus->ram, absx_addr, operand);
	
	mos6502->PC += 3;
	return 7;
}

/*
	X
	FAILBACK 
	FOR ILLEGAL OPCODES
*/
uint8_t	X(_6502 *mos6502) {
	//printf("illegal opcode\n");
	mos6502->PC += 1;
	return 0;
}

/// / //	LOADING

void	load_instructions(_6502* mos6502){
	for (unsigned i = 0; i < 0x100; i++) 
		mos6502->opcodes[i] = X;
	//	MSD 0
	mos6502->opcodes[0x00] = BRK_IMP;
	mos6502->opcodes[0x01] = ORA_INDX;
	mos6502->opcodes[0x05] = ORA_ZP;
	mos6502->opcodes[0x06] = ASL_ZP;
	mos6502->opcodes[0x08] = PHP_IMP;
	mos6502->opcodes[0x09] = ORA_IMM;
	mos6502->opcodes[0x0A] = ASL_ACC;
	mos6502->opcodes[0x0D] = ORA_ABS;
	mos6502->opcodes[0x0E] = ASL_ABS;
	//	MSD 1 
	mos6502->opcodes[0x10] = BPL_REL;
	mos6502->opcodes[0x11] = ORA_INDY;
	mos6502->opcodes[0x15] = ORA_ZPX;
	mos6502->opcodes[0x16] = ASL_ZPX;
	mos6502->opcodes[0x18] = CLC_IMP;
	mos6502->opcodes[0x19] = ORA_ABSY;
	mos6502->opcodes[0x1D] = ORA_ABSX;
	mos6502->opcodes[0x1E] = ASL_ABSX;
	//	MSD 2 
	mos6502->opcodes[0x20] = JSR_ABS;
	mos6502->opcodes[0x21] = AND_INDX;
	mos6502->opcodes[0x24] = BIT_ZP;
	mos6502->opcodes[0x25] = AND_ZP;
	mos6502->opcodes[0x26] = ROL_ZP;
	mos6502->opcodes[0x28] = PLP_IMP;
	mos6502->opcodes[0x29] = AND_IMM;
	mos6502->opcodes[0x2A] = ROL_ACC;
	mos6502->opcodes[0x2C] = BIT_ABS;
	mos6502->opcodes[0x2D] = AND_ABS;
	mos6502->opcodes[0x2E] = ROL_ABS;
	//	MSD 3 
	mos6502->opcodes[0x30] = BMI_REL;
	mos6502->opcodes[0x31] = AND_INDY;
	mos6502->opcodes[0x35] = AND_ZPX;
	mos6502->opcodes[0x36] = ROL_ZPX;
	mos6502->opcodes[0x38] = SEC_IMP;
	mos6502->opcodes[0x39] = AND_ABSY;
	mos6502->opcodes[0x3D] = AND_ABSX;
	mos6502->opcodes[0x3E] = ROL_ABSX;
	//	MSD 4 
	mos6502->opcodes[0x40] = RTI_IMP;
	mos6502->opcodes[0x41] = EOR_INDX;
	mos6502->opcodes[0x45] = EOR_ZP;
	mos6502->opcodes[0x46] = LSR_ZP;
	mos6502->opcodes[0x48] = PHA_IMP;
	mos6502->opcodes[0x49] = EOR_IMM;
	mos6502->opcodes[0x4A] = LSR_ACC;
	mos6502->opcodes[0x4C] = JMP_ABS;
	mos6502->opcodes[0x4D] = EOR_ABS;
	mos6502->opcodes[0x4E] = LSR_ABS;
	//	MSD 5 
	mos6502->opcodes[0x50] = BVS_REL;
	mos6502->opcodes[0x51] = EOR_INDY;
	mos6502->opcodes[0x55] = EOR_ZPX;
	mos6502->opcodes[0x56] = LSR_ZPX;
	mos6502->opcodes[0x58] = CLI_IMP;
	mos6502->opcodes[0x59] = EOR_ABSY;
	mos6502->opcodes[0x5D] = EOR_ABSX;
	mos6502->opcodes[0x5E] = LSR_ABSX;
	//	MSD 6 
	mos6502->opcodes[0x60] = RTS_IMP;
	mos6502->opcodes[0x61] = ADC_INDX;
	mos6502->opcodes[0x65] = ADC_ZP;
	mos6502->opcodes[0x66] = ROR_ZP;
	mos6502->opcodes[0x68] = PLA_IMP;
	mos6502->opcodes[0x69] = ADC_IMM;
	mos6502->opcodes[0x6A] = ROR_ACC;
	mos6502->opcodes[0x6C] = JMP_IND;
	mos6502->opcodes[0x6D] = ADC_ABS;
	mos6502->opcodes[0x6E] = ROR_ABS;
	//	MSD 7 
	mos6502->opcodes[0x70] = BVS_REL;
	mos6502->opcodes[0x71] = ADC_INDY;
	mos6502->opcodes[0x75] = ADC_ZPX;
	mos6502->opcodes[0x76] = ROR_ZPX;
	mos6502->opcodes[0x78] = SEI_IMP;
	mos6502->opcodes[0x79] = ADC_ABSY;
	mos6502->opcodes[0x7D] = ADC_ABSX;
	mos6502->opcodes[0x7E] = ROR_ABSX;
	//	MSD 8 
	mos6502->opcodes[0x81] = STA_INDX;
	mos6502->opcodes[0x84] = STY_ZP;
	mos6502->opcodes[0x85] = STA_ZP;
	mos6502->opcodes[0x86] = STX_ZP;
	mos6502->opcodes[0x88] = DEY_IMP;
	mos6502->opcodes[0x8A] = TXA_IMP;
	mos6502->opcodes[0x8C] = STY_ABS;
	mos6502->opcodes[0x8D] = STA_ABS;
	mos6502->opcodes[0x8E] = STX_ABS;
	//	MSD 9 
	mos6502->opcodes[0x90] = BCC_REL;
	mos6502->opcodes[0x91] = STA_INDY;
	mos6502->opcodes[0x94] = STY_ZPX;
	mos6502->opcodes[0x95] = STA_ZPX;
	mos6502->opcodes[0x96] = STX_ZPY;
	mos6502->opcodes[0x98] = TYA_IMP;
	mos6502->opcodes[0x99] = STA_ABSY;
	mos6502->opcodes[0x9A] = TXS_IMP;
	mos6502->opcodes[0x9D] = STA_ABSX;
	//	MSD A 
	mos6502->opcodes[0xA0] = LDY_IMM;
	mos6502->opcodes[0xA1] = LDA_INDX;
	mos6502->opcodes[0xA2] = LDX_IMM;
	mos6502->opcodes[0xA4] = LDY_ZP;
	mos6502->opcodes[0xA5] = LDA_ZP;
	mos6502->opcodes[0xA6] = LDX_ZP;
	mos6502->opcodes[0xA8] = TAY_IMP;
	mos6502->opcodes[0xA9] = LDA_IMM;
	mos6502->opcodes[0xAA] = TAX_IMP;
	mos6502->opcodes[0xAC] = LDY_ABS;
	mos6502->opcodes[0xAD] = LDA_ABS;
	mos6502->opcodes[0xAE] = LDX_ABS;
	//	MSD B 
	mos6502->opcodes[0xB0] = BCS_REL;
	mos6502->opcodes[0xB1] = LDA_INDY;
	mos6502->opcodes[0xB4] = LDY_ZPX;
	mos6502->opcodes[0xB5] = LDA_ZPX;
	mos6502->opcodes[0xB6] = LDX_ZPY;
	mos6502->opcodes[0xB8] = CLV_IMP;
	mos6502->opcodes[0xB9] = LDA_ABSY;
	mos6502->opcodes[0xBA] = TSX_IMP;
	mos6502->opcodes[0xBC] = LDY_ABSX;
	mos6502->opcodes[0xBD] = LDA_ABSX;
	mos6502->opcodes[0xBE] = LDX_ABSY;
	//	MSD C 
	mos6502->opcodes[0xC0] = CPY_IMM;
	mos6502->opcodes[0xC1] = CMP_INDX;
	mos6502->opcodes[0xC4] = CPY_ZP;
	mos6502->opcodes[0xC5] = CMP_ZP;
	mos6502->opcodes[0xC6] = DEC_ZP;
	mos6502->opcodes[0xC8] = INY_IMP;
	mos6502->opcodes[0xC9] = CMP_IMM;
	mos6502->opcodes[0xCA] = DEX_IMP;
	mos6502->opcodes[0xCC] = CPY_ABS;
	mos6502->opcodes[0xCD] = CMP_ABS;
	mos6502->opcodes[0xCE] = DEC_ABS;
	//	MSD D
	mos6502->opcodes[0xD0] = BNE_REL;
	mos6502->opcodes[0xD1] = CMP_INDY;
	mos6502->opcodes[0xD5] = CMP_ZPX;
	mos6502->opcodes[0xD6] = DEC_ZPX;
	mos6502->opcodes[0xD8] = CLD_IMP;
	mos6502->opcodes[0xD9] = CMP_ABSY;
	mos6502->opcodes[0xDD] = CMP_ABSX;
	mos6502->opcodes[0xDE] = DEC_ABSX;
	//	MSD E 
	mos6502->opcodes[0xE0] = CPX_IMM;
	mos6502->opcodes[0xE1] = SBC_INDX;
	mos6502->opcodes[0xE4] = CPX_ZP;
	mos6502->opcodes[0xE5] = SBC_ZP;
	mos6502->opcodes[0xE6] = INC_ZP;
	mos6502->opcodes[0xE8] = INX_IMP;
	mos6502->opcodes[0xE9] = SBC_IMM;
	mos6502->opcodes[0xEA] = NOP_IMP;
	mos6502->opcodes[0xEC] = CPX_ABS;
	mos6502->opcodes[0xED] = SBC_ABS;
	mos6502->opcodes[0xEE] = INC_ABS;
	//	MSD F 
	mos6502->opcodes[0xF0] = BEQ_REL;
	mos6502->opcodes[0xF1] = SBC_INDY;
	mos6502->opcodes[0xF5] = SBC_ZPX;
	mos6502->opcodes[0xF6] = INC_ZPX;
	mos6502->opcodes[0xF8] = SED_IMP;
	mos6502->opcodes[0xF9] = SBC_ABSY;
	mos6502->opcodes[0xFD] = SBC_ABSX;
	mos6502->opcodes[0xFE] = INC_ABSX;
}
