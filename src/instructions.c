#include "mos6502.h"

/*
   (uint16_t) opcode
   - (0-4)   opcode & 0xF000 |
   - (4-8)   opcode & 0x0F00 >> 8 |
   - (8-12)  opcode & 0x00F0 >> 4 |
   - (12-16) opcode & 0x000F
   */

// /// /	INSTRUCTIONS	

/*
	BRK
	IMPLIED
	1 Byte, 7 Cycles
*/
uint8_t	BRK_IMP(_6502* mos6502) {
	//printf("BRK IMP\n");
	mos6502->PC += 1;
	return 7;
}

/*
	ORA
	INDEXED INDIRECT
	2 Bytes, 6 Cycles
*/
uint8_t	ORA_INDX(_6502* mos6502) {
	//printf("ORA_INDX\n");
	mos6502->PC += 2;
	return 6;
}


/*
	ORA
	ZERO PAGE
	2 Bytes, 3 Cycles
*/
uint8_t	ORA_ZP(_6502* mos6502){
	//printf("ORA_ZP\n");
	mos6502->PC += 2;
	return 3;
}

/*
	ASL
	ZERO PAGE
	2 Bytes, 5 Cycles
*/
uint8_t	ASL_ZP(_6502* mos6502) {
	//printf("ASL_ZP\n");
	mos6502->PC += 2;
	return 5;
}

/*
	PHP
	IMPLIED
	1 Byte, 3 Cycles
*/
uint8_t	PHP_IMP(_6502 *mos6502) {
	//printf("PHP_IMP\n");
	mos6502->PC += 1;
	return 3;
}

/*
	ORA
	IMMEDIATE
	2 Bytes, 2 Cycles
*/
uint8_t	ORA_IMM(_6502 *mos6502) {
	//printf("ORA_IMM\n");
	mos6502->PC += 2;
	return 2;
}

/*
	ASL
	ACCUMULATOR
	1 Byte, 2 Cycles
*/
uint8_t	ASL_ACC(_6502 *mos6502) {
	//printf("ASL_ACC\n");
	mos6502->PC += 1;
	return 2;
}

/*
	ORA
	ABSOLUTE
	3 Byte, 4 Cycles
*/
uint8_t	ORA_ABS(_6502 *mos6502) {
	//printf("ORA_ABS\n");
	mos6502->PC += 3;
	return 4;
}

/*
	ASL
	ABSOLUTE
	3 Bytes, 6 Cycles
*/
uint8_t	ASL_ABS(_6502* mos6502) {
	//printf("ASL_ABS\n");
	mos6502->PC += 3;
	return 6;
}

/*
	BPL
	RELATIVE
	2 Bytes, 2** Cycles
*/
uint8_t	BPL_REL(_6502 *mos6502) {
	//printf("BPL_REL\n");
	mos6502->PC += 2;
	return 2;
}

/*
	ORA
	INDIRECT INDEXING
	2 Bytes, 5* Cycles
*/
uint8_t	ORA_INDY(_6502* mos6502) {
	//printf("ORA_INDY\n");
	mos6502->PC += 2;
	return 5;
}

/*
	ORA
	ZERO PAGE X
	2 Bytes, 4 Cycles
*/
uint8_t	ORA_ZPX(_6502* mos6502) {
	//printf("ORA_ZPX\n");
	mos6502->PC += 2;
	return 4;
}

/*
	ASL
	ZERO PAGE X
	2 Bytes, 6 Cycles
*/
uint8_t	ASL_ZPX(_6502* mos6502) {
	//printf("ASL_ZPX\n");
	mos6502->PC += 2;
	return 6;
}

/*
	CLC
	IMPLIED
	1 Byte, 2 Cycles
*/
uint8_t	CLC_IMP(_6502 *mos6502) {
	//printf("CLC_IMP\n");
	mos6502->PC += 2;
	return 2;
}

/*
	ORA
	ABSOLUTE Y
	3 Bytes, 4* Cycles
*/
uint8_t	ORA_ABSY(_6502 *mos6502) {
	//printf("ORA_ABSY\n");
	mos6502->PC += 3;
	return 4;
}

/*
	ORA
	ABSOLUTE X
	3 Bytes, 4* Cycles
*/
uint8_t	ORA_ABSX(_6502 *mos6502) {
	//printf("ORA_ABSX\n");
	mos6502->PC += 3;
	return 4;
}

/*
	ASL
	ABSOLUTE X
	3 Bytes, 7 Cycles
*/
uint8_t	ASL_ABSX(_6502 *mos6502) {
	//printf("ASL_ABSX\n");
	mos6502->PC += 3;
	return 7;
}

/*
	JSR
	ABSOLUTE
	3 Bytes, 6 Cycles
*/
uint8_t	JSR_ABS(_6502* mos6502) {
	//printf("JSR_ABS\n");
	mos6502->PC += 3;
	return 6;
}

/*
	AND
	INDEXED INDIRECT
	2 Bytes, 6 Cycles
*/
uint8_t	AND_INDX(_6502 *mos6502) {
	//printf("AND_INDX\n");
	mos6502->PC += 2;
	return 6;
}

/*
	BIT
	ZERO PAGE
	2 Bytes, 3 Cycles
*/
uint8_t	BIT_ZP(_6502 *mos6502) {
	//printf("BIT_ZP\n");
	mos6502->PC += 2;
	return 3;
}

/*
	AND
	ZERO PAGE
	2 Bytes, 3 Cycles
*/
uint8_t	AND_ZP(_6502 *mos6502) {
	//printf("AND_ZP\n");
	mos6502->PC += 2;
	return 3;
}

/*
	ROL
	ZERO PAGE
	2 Bytes, 5 Cycles
*/
uint8_t	ROL_ZP(_6502 *mos6502) {
	//printf("ROL_ZP\n");
	mos6502->PC += 2;
	return 5;
}

/*
	PLP
	IMPLIED
	1 Byte, 4 Cycles
*/
uint8_t	PLP_IMP(_6502 *mos6502) {
	//printf("PLP_IMP\n");
	mos6502->PC += 1;
	return 4;
}

/*
	AND
	IMMEDIATE
	2 Bytes, 2 Cycles
*/
uint8_t	AND_IMM(_6502 *mos6502) {
	//printf("AND_IMM\n");
	mos6502->PC += 2;
	return 2;
}

/*
	ROL
	ACCUMULATOR
	1 Byte, 2 Cycles
*/
uint8_t	ROL_ACC(_6502 *mos6502) {
	//printf("ROL_ACC\n");
	mos6502->PC += 1;
	return 2;
}

/*
	BIT
	ABSOLUTE
	3 Bytes, 4 Cycles
*/
uint8_t	BIT_ABS(_6502 *mos6502) {
	//printf("BIT_ABS\n");
	mos6502->PC += 3;
	return 4;
}

/*
	AND
	ABSOLUTE
	3 Bytes, 4 Cycles
*/
uint8_t	AND_ABS(_6502 *mos6502) {
	//printf("AND_ABS\n");
	mos6502->PC += 3;
	return 4;
}

/*
	ROL
	ABSOLUTE
	3 Bytes, 6 Cycles
*/
uint8_t	ROL_ABS(_6502 *mos6502) {
	//printf("ROL_ABS\n");
	mos6502->PC += 3;
	return 6;
}

/*
	BMI
	RELATIVE
	2 Bytes, 2** Cycles
*/
uint8_t	BMI_REL(_6502 *mos6502) {
	//printf("BMI_REL\b");
	mos6502->PC += 2;
	return 2;
}

/*
	AND
	INDIRECT INDEXED
	2 Bytes, 5* Cycles
*/
uint8_t	AND_INDY(_6502 *mos6502) {
	//printf("AND_INDY\n");
	mos6502->PC += 2;
	return 5;
}

/*
	AND
	ZERO PAGE X
	2 Bytes, 4 Cycles
*/
uint8_t	AND_ZPX(_6502 *mos6502) {
	//printf("AND_ZPX\n");
	mos6502->PC += 2;
	return 4;
}

/*
	ROL
	ZERO PAGE X
	2 Bytes, 6 Cycles
*/
uint8_t	ROL_ZPX(_6502 *mos6502) {
	//printf("ROL_ZPX\n");
	mos6502->PC += 2;
	return 6;
}

/*
	SEC
	IMPLIED
	1 Byte, 2 Cycles
*/
uint8_t	SEC_IMP(_6502 *mos6502) {
	//printf("SEC_IMP\n");
	mos6502->PC += 1;
	return 2;
}

/*
	AND
	ABSOLUTE Y
	3 Bytes, 4* Cycles
*/
uint8_t	AND_ABSY(_6502 *mos6502) {
	//printf("AND_ABSY\n");
	mos6502->PC += 3;
	return 4;
}

/*
	AND
	ABSOLUTE X
	3 Bytes, 4* Cycles
*/
uint8_t	AND_ABSX(_6502 *mos6502) {
	//printf("AND_ABSX\n");
	mos6502->PC += 3;
	return 4;
}

/*
	ROL
	ABSOLUTE X
	3 Bytes, 7 Cycles
*/
uint8_t	ROL_ABSX(_6502 *mos6502) {
	//printf("ROL_ABSX\n");
	mos6502->PC += 3;
	return 7;
}

/*
	RTI
	IMPLIED
	1 Byte, 6 Cycles
*/
uint8_t	RTI_IMP(_6502 *mos6502) {
	//printf("RTI_IMP\n");
	mos6502->PC += 1;
	return 6;
}

/*
	EOR
	INDEXED INDIRECT
	2 Bytes, 6 Cycles
*/
uint8_t	EOR_INDX(_6502 *mos6502) {
	//printf("EOR_INDX\n");
	mos6502->PC += 2;
	return 6;
}

/*
	EOR
	ZERO PAGE
	2 Bytes, 3 Cycles
*/
uint8_t	EOR_ZP(_6502 *mos6502) {
	//printf("EOR_ZP\n");
	mos6502->PC += 2;
	return 3;
}

/*
	LSR
	ZERO PAGE
	2 Bytes, 5 Cycles
*/
uint8_t	LSR_ZP(_6502 *mos6502) {
	//printf("LSR_ZP\n");
	mos6502->PC += 2;
	return 5;
}

/*
	PHA
	IMPLIED
	1 Byte, 3 Cycles
*/
uint8_t	PHA_IMP(_6502 *mos6502) {
	//printf("PHA_IMP\n");
	mos6502->PC += 1;
	return 3;
}

/*
	EOR
	IMMEDIATE
	2 Bytes, 2 Cycles
*/
uint8_t	EOR_IMM(_6502 *mos6502) {
	//printf("EOR_IMM\n");
	mos6502->PC += 2;
	return 2;
}

/*
	LSR
	ACCUMULATOR
	1 Byte, 2 Cycles
*/
uint8_t	LSR_ACC(_6502 *mos6502) {
	//printf("LSR_ACC\n");
	mos6502->PC += 1;
	return 2;
}

/*
	JMP
	ABSOLUTE
	3 Bytes, 3 Cycles
*/
uint8_t	JMP_ABS(_6502 *mos6502) {
	//printf("JMP_ABS\n");
	mos6502->PC += 3;
	return 3;
}

/*
	EOR
	ABSOLUTE
	3 Bytes, 4 Cycles
*/
uint8_t	EOR_ABS(_6502 *mos6502) {
	//printf("EOR_ABS\n");
	mos6502->PC += 3;
	return 4;
}

/*
	LSR
	ABSOLUTE
	3 Bytes, 6 Cycles
*/
uint8_t	LSR_ABS(_6502 *mos6502) {
	//printf("LSR_ABS\n");
	mos6502->PC += 3;
	return 6;
}

/*
	BVC
	RELATIVE
	2 Bytes, 2** Cycles
*/
uint8_t	BVC_REL(_6502 *mos6502) {
	//printf("BVC_REL\n");
	mos6502->PC += 2;
	return 2;
}

/*
	EOR
	INDIRECT INDEXED
	2 Bytes, 5* Cycles
*/
uint8_t	EOR_INDY(_6502 *mos6502) {
	//printf("EOR_INDY\n");
	mos6502->PC += 2;
	return 5;
}

/*
	EOR
	ZERO PAGE X
	2 Bytes, 4 Cycles
*/
uint8_t	EOR_ZPX(_6502 *mos6502) {
	//printf("EOR_ZPX\n");
	mos6502->PC += 2;
	return 4;
}

/*
	LSR
	ZERO PAGE X
	2 Bytes, 6 Cycles
*/
uint8_t	LSR_ZPX(_6502 *mos6502) {
	//printf("LSR_ZPX\n");
	mos6502->PC += 2;
	return 6;
}

/*
	CLI
	IMPLIED
	1 Byte, 2 Cycles
*/
uint8_t	CLI_IMP(_6502 *mos6502) {
	//printf("CLI_IMP\n");
	mos6502->PC += 1;
	return 2;
}

/*
	EOR
	ABSOLUTE Y
	3 Bytes, 4* Cycles
*/
uint8_t	EOR_ABSY(_6502 *mos6502) {
	//printf("EOR_ABSY\n");
	mos6502->PC += 3;
	return 4;
}

/*
	EOR
	ABSOUTE X
	3 Bytes, 4* Cycles
*/
uint8_t	EOR_ABSX(_6502 *mos6502) {
	//printf("EOR_ABSX\n");
	mos6502->PC += 3;
	return 4;
}

/*
	LSR
	ABSOLUTE X
	3 Bytes, 7 Cycles
*/
uint8_t	LSR_ABSX(_6502 *mos6502) {
	//printf("LSR_ABSX\n");
	mos6502->PC += 3;
	return 7;
}

/*
	RTS
	IMPLIED
	1 Byte, 6 Cycles
*/
uint8_t	RTS_IMP(_6502 *mos6502) {
	//printf("RTS_IMP\n");
	mos6502->PC += 1;
	return 6;
}

/*
	ADC
	INDEXED INDIRECT
	2 Bytes, 6 Cycles
*/
uint8_t	ADC_INDX(_6502 *mos6502) {
	//printf("ADC_INDX\n");
	mos6502->PC += 2;
	return 6;
}

/*
	ADC
	ZERO PAGE
	2 Bytes, 3 Cycles
*/
uint8_t	ADC_ZP(_6502 *mos6502) {
	//printf("ADC_ZP\n");
	mos6502->PC += 2;
	return 3;
}

/*
	ROR
	ZERO PAGE
	2 Bytes, 5 Cycles
*/
uint8_t	ROR_ZP(_6502 *mos6502) {
	//printf("ROR_ZP\n");
	mos6502->PC += 2;
	return 5;
}

/*
	PLA
	IMPLIED
	1 Byte, 4 Cycles
*/
uint8_t	PLA_IMP(_6502 *mos6502) {
	//printf("PLA_IMP\n");
	mos6502->PC += 1;
	return 4;
}

/*
	ADC
	IMMEDIATE
	2 Bytes, 2 Cycles
*/
uint8_t	ADC_IMM(_6502 *mos6502) {
	//printf("ADC_IMM\n");
	mos6502->PC += 2;
	return 2;
}

/*
	ROR
	ACCUMULATOR
	1 Byte, 2 Cycles
*/
uint8_t	ROR_ACC(_6502* mos6502) {
	//printf("ROR_ACC\n");
	mos6502->PC += 1;
	return 2;
}

/*
	JMP
	INDIRECT
	3 Bytes, 5 Cycles
*/
uint8_t	JMP_IND(_6502 *mos6502) {
	//printf("JMP_IND\n");
	mos6502->PC += 3;
	return 5;
}

/*
	ADC
	ABSOLUTE
	3 Bytes, 4 Cycles
*/
uint8_t	ADC_ABS(_6502 *mos6502) {
	//printf("ADC_ABS\n");
	mos6502->PC += 3;
	return 4;
}

/*
	ROR
	ABSOLUTE
	3 Bytes, 6 Cycles
*/
uint8_t	ROR_ABS(_6502 *mos6502) {
	//printf("ROR_ABS\n");
	mos6502->PC += 3;
	return 6;
}

/*
	BVS
	RELATIVE
	2 Bytes, 2** Cycles
*/
uint8_t	BVS_REL(_6502 *mos6502) {
	//printf("BVS_REL\n");
	mos6502->PC += 2;
	return 2;
}

/*
	ADC
	INDIRECT INDEXED
	2 Bytes, 5* Cycles
*/
uint8_t	ADC_INDY(_6502 *mos6502) {
	//printf("ADC_INDY\n");
	mos6502->PC += 2;
	return 5;
}

/*
	ADC
	ZERO PAGE X
	2 Bytes, 4 Cycles
*/
uint8_t	ADC_ZPX(_6502 *mos6502) {
	//printf("ASC_ZPX\n");
	mos6502->PC += 2;
	return 4;
}

/*
	ROR
	ZERO PAGE X
	2 Bytes, 6 Cycles
*/
uint8_t	ROR_ZPX(_6502 *mos6502) {
	//printf("ROR_ZPX\n");
	mos6502->PC += 2;
	return 6;
}

/*
	SEI
	IMPLIED
	1 Byte, 2 Cycles
*/
uint8_t	SEI_IMP(_6502 *mos6502) {
	//printf("SEI_IMP\n");
	mos6502->PC += 1;
	return 2;
}

/*
	ADC
	ABSOLUTE Y
	3 Bytes, 4* Cycles
*/
uint8_t	ADC_ABSY(_6502* mos6502) {
	//printf("ADC_ABSY\n");
	mos6502->PC += 3;
	return 4;
}

/*
	ADC
	ABSOLUTE X
	3 Bytes, 4* Cycles
*/
uint8_t	ADC_ABSX(_6502 *mos6502) {
	//printf("ADC_ABSX\n");
	mos6502->PC += 3;
	return 4;
}

/*
	ROR
	ABSOLUTE X
	3 Bytes, 7 Cycles
*/
uint8_t	ROR_ABSX(_6502 *mos6502) {
	//printf("ROR_ABSX\n");
	mos6502->PC += 3;
	return 7;
}

/*
	STA
	INDEXED INDIRECT
	2 Bytes, 6 Cycles
*/
uint8_t	STA_INDX(_6502 *mos6502) {
	//printf("STA_INDX\n");
	mos6502->PC += 2;
	return 6;
}

/*
	STY
	ZERO PAGE
	2 Bytes, 3 Cycles
*/
uint8_t	STY_ZP(_6502 *mos6502) {
	//printf("STY_ZP\n");
	mos6502->PC += 2;
	return 3;
}

/*
	STA
	ZERO PAGE
	2 Bytes, 3 Cycles
*/
uint8_t	STA_ZP(_6502 *mos6502) {
	//printf("STA_ZP\n");
	mos6502->PC += 2;
	return 3;
}

/*
	STX
	ZERO PAGE
	2 Bytes, 3 Cycles
*/
uint8_t	STX_ZP(_6502 *mos6502) {
	//printf("STX_ZP\n");
	mos6502->PC += 2;
	return 3;
}

/*
	DEY
	IMPLIED
	1 Byte, 2 Cycles
*/
uint8_t	DEY_IMP(_6502* mos6502) {
	//printf("DEY_IMP\n");
	mos6502->PC += 1;
	return 2;
}

/*
	TXA
	IMPLIED
	1 Byte, 2 Cycles
*/
uint8_t	TXA_IMP(_6502 *mos6502) {
	//printf("TXA_IMP\n");
	mos6502->PC += 1;
	return 2;
}

/*
	STY
	ABSOLUTE
	3 Bytes, 4 Cycles
*/
uint8_t	STY_ABS(_6502 *mos6502) {
	//printf("STY_ABS\n");
	mos6502->PC += 3;
	return 4;
}

/*
	STA
	ABSOLUTE
	3 Bytes, 4 Cycles
*/
uint8_t	STA_ABS(_6502 *mos6502) {
	//printf("STA_ABS\n");
	mos6502->PC += 3;
	return 4;
}

/*
	STX
	ABSOLUTE
	3 Bytes, 4 Cycles
*/
uint8_t	STX_ABS(_6502 *mos6502) {
	//printf("STX_ABS\n");
	mos6502->PC += 3;
	return 4;
}

/*
	BCC
	RELATIVE
	2 Bytes, 2** Cycles
*/
uint8_t	BCC_REL(_6502 *mos6502) {
	//printf("BCC_REL\n");
	mos6502->PC += 2;
	return 2;
}

/*
	STA
	INDIRECT INDEXED
	2 Bytes, 6 Cycles
*/
uint8_t	STA_INDY(_6502 *mos6502) {
	//printf("STA_INDY\n");
	mos6502->PC += 2;
	return 6;
}

/*
	STY
	ZERO PAGE X
	2 Bytes, 4 Cycles
*/
uint8_t	STY_ZPX(_6502 *mos6502) {
	//printf("STY_ZPX\n");
	mos6502->PC += 2;
	return 4;
}

/*
	STA
	ZERO PAGE X
	2 Bytes, 4 Cycles
*/
uint8_t	STA_ZPX(_6502 *mos6502) {
	//printf("STA_ZPX\n");
	mos6502->PC += 2;
	return 4;
}

/*
	STX
	ZERO PAGE Y
	2 Bytes, 4 Cycles
*/
uint8_t	STX_ZPY(_6502 *mos6502) {
	//printf("STX_ZPY\n");
	mos6502->PC += 2;
	return 4;
}

/*
	TYA
	IMPLIED
	1 Byte, 2 Cycles
*/
uint8_t	TYA_IMP(_6502 *mos6502) {
	//printf("TYA_IMP\n");
	mos6502->PC += 1;
	return 2;
}

/*
	STA
	ABSOLUTE Y
	3 Bytes, 5 Cycles
*/
uint8_t	STA_ABSY(_6502 *mos6502) {
	//printf("STA_ABSY\n");
	mos6502->PC += 3;
	return 5;
}

/*
	TXS
	IMPLIED
	1 Byte, 2 Cycles
*/
uint8_t	TXS_IMP(_6502 *mos6502) {
	//printf("TXS_IMP\n");
	mos6502->PC += 1;
	return 2;
}

/*
	STA
	ABSOLUTE X
	3 Bytes, 5 Cycles
*/
uint8_t	STA_ABSX(_6502 *mos6502) {
	//printf("STA_ABSX\n");
	mos6502->PC += 3;
	return 5;
}

/*
	LDY
	IMMEDIATE
	2 Bytes, 2 Cycles
*/
uint8_t	LDY_IMM(_6502 *mos6502) {
	//printf("LDY_IMM\n");
	mos6502->PC += 2;
	return 2;
}

/*
	LDA
	INDEXED INDIRECT
	2 Bytes, 6 Cycles
*/
uint8_t	LDA_INDX(_6502 *mos6502) {
	//printf("LDA_INDX\n");
	mos6502->PC += 2;
	return 6;
}

/*
	LDX
	IMMEDIATE
	2 Bytes, 2 Cycles
*/
uint8_t	LDX_IMM(_6502 *mos6502) {
	//printf("LDX_IMM\n");
	mos6502->PC += 2;
	return 2;
}

/*
	LDY
	ZERO PAGE
	2 Bytes, 3 Cycles
*/
uint8_t	LDY_ZP(_6502 *mos6502) {
	//printf("LDY_ZP\n");
	mos6502->PC += 2;
	return 3;
}

/*
	LDA
	ZERO PAGE
	2 Bytes, 3 Cycles
*/
uint8_t	LDA_ZP(_6502 *mos6502) {
	//printf("LDA_ZP\n");
	mos6502->PC += 2;
	return 3;
}

/*
	LDX
	ZERO PAGE
	2 Bytes, 3 Cycles
*/
uint8_t	LDX_ZP(_6502 *mos6502) {
	//printf("LDX_ZP\n");
	mos6502->PC += 2;
	return 3;
}

/*
	TAY
	IMPLIED
	1 Byte, 2 Cycles
*/
uint8_t	TAY_IMP(_6502 *mos6502) {
	//printf("TAY_IMP\n");
	mos6502->PC += 1;
	return 2;
}

/*
	LDA
	IMMEDIATE
	2 Bytes, 2 Cycles
*/
uint8_t	LDA_IMM(_6502 *mos6502) {
	//printf("LDA_IMM\n");
	mos6502->PC += 2;
	return 2;
}

/*
	TAX
	IMPLIED
	1 Byte, 2 Cycles
*/
uint8_t	TAX_IMP(_6502 *mos6502) {
	//printf("TAX_IMP\n");
	mos6502->PC += 1;
	return 2;
}

/*
	LDY
	ABSOLUTE
	3 Bytes, 4 Cycles
*/
uint8_t	LDY_ABS(_6502 *mos6502) {
	//printf("LDY_ABS\n");
	mos6502->PC += 3;
	return 4;
}

/*
	LDA
	ABSOLUTE
	3 Bytes, 4 Cycles
*/
uint8_t	LDA_ABS(_6502 *mos6502) {
	//printf("LDA_ABS\n");
	mos6502->PC += 3;
	return 4;
}

/*
	LDX
	ABSOLUTE
	3 Bytes, 4 Cycles
*/
uint8_t	LDX_ABS(_6502 *mos6502) {
	//printf("LDX_ABS\n");
	mos6502->PC += 3;
	return 4;
}

/*
	BCS
	RELATIVE
	2 Bytes, 2** Cycles
*/
uint8_t	BCS_REL(_6502 *mos6502) {
	//printf("BCS_REL\n");
	mos6502->PC += 2;
	return 2;
}

/*
	LDA
	INDIRECT INDEXED
	2 Bytes, 5* Cycles
*/
uint8_t	LDA_INDY(_6502 *mos6502) {
	//printf("LDA_INDY\n");
	mos6502->PC += 2;
	return 5;
}

/*
	LDY
	ZERO PAGE X
	2 Bytes, 4 Cycles
*/
uint8_t	LDY_ZPX(_6502 *mos6502) {
	//printf("LDY_ZPX\n");
	mos6502->PC += 2;
	return 4;
}

/*
	LDA
	ZERO PAGE X
	2 Bytes, 4 Cycles
*/
uint8_t	LDA_ZPX(_6502 *mos6502) {
	//printf("LDA_ZPX\n");
	mos6502->PC += 2;
	return 4;
}

/*
	LDX
	ZERO PAGE Y
	2 Bytes, 4 Cycles
*/
uint8_t	LDX_ZPY(_6502 *mos6502) {
	//printf("LDX_ZPY\n");
	mos6502->PC += 2;
	return 4;
}

/*
	CLV
	IMPLIED
	1 Byte, 2 Cycles
*/
uint8_t	CLV_IMP(_6502 *mos6502) {
	//printf("CLV_IMP\n");
	mos6502->PC += 1;
	return 2;
}

/*
	LDA
	ABSOLUTE Y
	3 Bytes, 4* Cycles
*/
uint8_t	LDA_ABSY(_6502 *mos6502) {
	//printf("LDA_ABSY\n");
	mos6502->PC += 3;
	return 4;
}

/*
	TSX
	IMPLIED
	1 Byte, 2 Cycles
*/
uint8_t	TSX_IMP(_6502 *mos6502) {
	//printf("TSX_IMP\n");
	mos6502->PC += 1;
	return 2;
}

/*
	LDY
	ABSOLUTE X
	3 Bytes, 4* Cycles
*/
uint8_t	LDY_ABSX(_6502 *mos6502) {
	//printf("LDY_ABSX\n");
	mos6502->PC += 3;
	return 4;
}

/*
	LDA
	ABSOLUTE X
	3 Bytes, 4* Cycles
*/
uint8_t	LDA_ABSX(_6502 *mos6502) {
	//printf("LDA_ABSX\n");
	mos6502->PC += 3;
	return 4;
}

/*
	LDX
	ABSOLUTE Y
	3 Bytes, 4* Cycles
*/
uint8_t	LDX_ABSY(_6502 *mos6502) {
	//printf("LDX_ABSY\n");
	mos6502->PC += 3;
	return 4;
}

/*
	CPY
	IMMEDIATE
	2 Bytes, 2 Cycles
*/
uint8_t	CPY_IMM(_6502 *mos6502) {
	//printf("CPY_IMM\n");
	mos6502->PC += 2;
	return 2;
}

/*
	CMP
	INDEXED INDIRECT
	2 Bytes, 6 Cycles
*/
uint8_t	CMP_INDX(_6502 *mos6502) {
	//printf("CMP_INDX\n");
	mos6502->PC += 2;
	return 6;
}

/*
	CPY
	ZERO PAGE
	2 Bytes, 3 Cycles
*/
uint8_t	CPY_ZP(_6502 *mos6502) {
	//printf("CPY_ZP\n");
	mos6502->PC += 2;
	return 3;
}

/*
	CMP
	ZERO PAGE
	2 Bytes, 3 Cycles
*/
uint8_t	CMP_ZP(_6502 *mos6502) {
	//printf("CMP_ZP\n");
	mos6502->PC += 2;
	return 3;
}

/*
	DEC
	ZERO PAGE
	2 Bytes, 5 Cycles
*/
uint8_t	DEC_ZP(_6502 *mos6502) {
	//printf("DEC_ZP\n");
	mos6502->PC += 2;
	return 5;
}

/*
	INY
	IMPLIED
	1 Byte, 2 Cycles
*/
uint8_t	INY_IMP(_6502 *mos6502) {
	//printf("INY_IMP\n");
	mos6502->PC += 1;
	return 2;
}

/*
	CMP
	IMMEDIATE
	2 Bytes, 2 Cycles
*/
uint8_t	CMP_IMM(_6502 *mos6502) {
	//printf("CMP_IMM\n");
	mos6502->PC += 2;
	return 2;
}

/*
	DEX
	IMPLIED
	1 Byte, 2 Cycles
*/
uint8_t	DEX_IMP(_6502 *mos6502) {
	//printf("DEX_IMP\n");
	mos6502->PC += 1;
	return 2;
}

/*
	CPY
	ABSOLUTE
	3 Bytes, 4 Cycles
*/
uint8_t	CPY_ABS(_6502 *mos6502) {
	//printf("CPY_ABS\n");
	mos6502->PC += 3;
	return 4;
}

/*
	CMP
	ABSOLUTE
	3 Bytes, 4 Cycles
*/
uint8_t	CMP_ABS(_6502 *mos6502) {
	//printf("CMP_ABS\n");
	mos6502->PC += 3;
	return 4;
}

/*
	DEC
	ABSOLUTE
	3 Bytes, 6 Cycles
*/
uint8_t	DEC_ABS(_6502 *mos6502) {
	//printf("DEC_ABS\n");
	mos6502->PC += 3;
	return 6;
}

/*
	BNE
	RELATIVE
	2 Bytes, 2** Cycles
*/
uint8_t	BNE_REL(_6502 *mos6502) {
	//printf("BNE_REL\n");
	mos6502->PC += 2;
	return 2;
}

/*
	CMP
	INDIRECT INDEXED
	2 Bytes, 5* Cycles
*/
uint8_t	CMP_INDY(_6502 *mos6502) {
	//printf("CMP_INDY\n");
	mos6502->PC += 2;
	return 5;
}

/*
	CMP
	ZERO PAGE X
	2 Bytes, 4 Cycles	
*/
uint8_t	CMP_ZPX(_6502 *mos6502) {
	//printf("CMP_ZPX\n");
	mos6502->PC += 2;
	return 4;
}

/*
	DEC
	ZERO PAGE X
	2 Bytes, 6 Cycles
*/
uint8_t	DEC_ZPX(_6502 *mos6502) {
	//printf("DEC_ZPX\n");
	mos6502->PC += 2;
	return 6;
}

/*
	CLD
	IMPLIED
	1 Bytes, 2 Cycles
*/
uint8_t	CLD_IMP(_6502 *mos6502) {
	//printf("CLD_IMP\n");
	mos6502->PC += 1;
	return 2;
}

/*
	CMP
	ABSOLUTE Y
	2 Bytes, 4* Cycles
*/
uint8_t	CMP_ABSY(_6502 *mos6502) {
	//printf("CMP_ABSY\n");
	mos6502->PC += 2;
	return 4;
}

/*
	CMP
	ABSOLUTE X
	2 Bytes, 4* Cycles
*/
uint8_t	CMP_ABSX(_6502 *mos6502) {
	//printf("CMP_ABSX\n");
	mos6502->PC += 2;
	return 4;
}

/*
	DEC
	ABSOLUTE X
	3 Bytes, 7 Cycles
*/
uint8_t	DEC_ABSX(_6502 *mos6502) {
	//printf("DEC_ABSX\n");
	mos6502->PC += 3;
	return 7;
}

/*
	CPX
	IMMEDIATE
	2 Bytes, 2 Cycles
*/
uint8_t	CPX_IMM(_6502 *mos6502) {
	//printf("CPX_IMM\n");
	mos6502->PC += 2;
	return 2;
}

/*
	SBC
	INDEXED INDIRECT
	2 Bytes, 6 Cycles
*/
uint8_t	SBC_INDX(_6502 *mos6502) {
	//printf("SBC_INDX\n");
	mos6502->PC += 2;
	return 6;
}

/*
	CPX
	ZERO PAGE
	2 Bytes, 3 Cycles
*/
uint8_t	CPX_ZP(_6502 *mos6502) {
	//printf("CPX_ZP\n");
	mos6502->PC += 2;
	return 3;
}

/*
	SBC
	ZERO PAGE
	2 Bytes, 3 Cycles
*/
uint8_t	SBC_ZP(_6502 *mos6502) {
	//printf("SBC_ZP\n");
	mos6502->PC += 2;
	return 3;
}

/*
	INC
	ZERO PAGE
	2 Bytes, 5 Cycles
*/
uint8_t	INC_ZP(_6502 *mos6502) {
	//printf("INC_ZP\n");
	mos6502->PC += 2;
	return 5;
}

/*
	INX
	IMPLIED
	1 Byte, 2 Cycles
*/
uint8_t	INX_IMP(_6502 *mos6502) {
	//printf("INX_IMP\n");
	mos6502->PC += 1;
	return 2;
}

/*
	SBC
	IMMEDIATE
	2 Bytes, 2 Cycles
*/
uint8_t	SBC_IMM(_6502 *mos6502) {
	//printf("SBC_IMM\n");
	mos6502->PC += 2;
	return 2;
}

/*
	NOP
	IMPLIED
	1 Byte, 2 Cycles
*/
uint8_t	NOP_IMP(_6502 *mos6502) {
	//printf("NOP_IMP\n");
	mos6502->PC += 1;
	return 2;
}

/*
	CPX
	ABSOLUTE
	3 Bytes, 4 Cycles
*/
uint8_t	CPX_ABS(_6502 *mos6502) {
	//printf("CPX_ABS\n");
	mos6502->PC += 3;
	return 4;
}

/*
	SBC
	ABSOLUTE
	3 Bytes, 4 Cycles
*/
uint8_t	SBC_ABS(_6502 *mos6502) {
	//printf("SBC_ABS\n");
	mos6502->PC += 3;
	return 4;
}

/*
	INC
	ABSOLUTE
	3 Bytes, 6 Cycles
*/
uint8_t	INC_ABS(_6502 *mos6502) {
	//printf("INC_ABS\n");
	mos6502->PC += 3;
	return 6;
}

/*
	BEQ
	RELATIVE
	2 Bytes, 2** Cycles
*/
uint8_t	BEQ_REL(_6502 *mos6502) {
	//printf("BEQ_REL\n");
	mos6502->PC += 2;
	return 2;
}

/*
	SBC
	INDIRECT INDEXED
	2 Bytes, 5* Cycles
*/
uint8_t	SBC_INDY(_6502 *mos6502) {
	//printf("SBC_INDY\n");
	mos6502->PC += 2;
	return 5;
}

/*
	SBC
	ZERO PAGE X
	2 Bytes, 4 Cycles
*/
uint8_t	SBC_ZPX(_6502 *mos6502) {
	//printf("SBC_ZPX\n");
	mos6502->PC += 2;
	return 4;
}

/*
	INC
	ZERO PAGE X
	2 Bytes, 6 Cycles
*/
uint8_t	INC_ZPX(_6502 *mos6502) {
	//printf("INC_ZPX\n");
	mos6502->PC += 2;
	return 6;
}

/*
	SED
	IMPLIED
	1 Byte, 2 Cycles
*/
uint8_t	SED_IMP(_6502 *mos6502) {
	//printf("SED_IMP\n");
	mos6502->PC += 1;
	return 2;
}

/*
	SBC
	ABSOLUTE Y
	2 Bytes, 4* Cycles
*/
uint8_t	SBC_ABSY(_6502 *mos6502) {
	//printf("SBC_ABSY\n");
	mos6502->PC += 2;
	return 4;
}

/*
	SBC
	ABSOLUTE X
	3 Bytes, 4* Cycles
*/
uint8_t	SBC_ABSX(_6502 *mos6502) {
	//printf("SBC_ABSX\n");
	mos6502->PC += 3;
	return 4;
}

/*
	INC
	ABSOLUTE X
	3 Bytes, 7 Cycles
*/
uint8_t	INC_ABSX(_6502 *mos6502) {
	//printf("INC_ABSX\n");
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
		mos6502->instructions[i] = X;
	//	MSD 0
	mos6502->instructions[0x00] = BRK_IMP;
	mos6502->instructions[0x01] = ORA_INDX;
	mos6502->instructions[0x05] = ORA_ZP;
	mos6502->instructions[0x06] = ASL_ZP;
	mos6502->instructions[0x08] = PHP_IMP;
	mos6502->instructions[0x09] = ORA_IMM;
	mos6502->instructions[0x0A] = ASL_ACC;
	mos6502->instructions[0x0D] = ORA_ABS;
	mos6502->instructions[0x0E] = ASL_ABS;
	//	MSD 1 
	mos6502->instructions[0x10] = BPL_REL;
	mos6502->instructions[0x11] = ORA_INDY;
	mos6502->instructions[0x15] = ORA_ZPX;
	mos6502->instructions[0x16] = ASL_ZPX;
	mos6502->instructions[0x18] = CLC_IMP;
	mos6502->instructions[0x19] = ORA_ABSY;
	mos6502->instructions[0x1D] = ORA_ABSX;
	mos6502->instructions[0x1E] = ASL_ABSX;
	//	MSD 2 
	mos6502->instructions[0x20] = JSR_ABS;
	mos6502->instructions[0x21] = AND_INDX;
	mos6502->instructions[0x24] = BIT_ZP;
	mos6502->instructions[0x25] = AND_ZP;
	mos6502->instructions[0x26] = ROL_ZP;
	mos6502->instructions[0x28] = PLP_IMP;
	mos6502->instructions[0x29] = AND_IMM;
	mos6502->instructions[0x2A] = ROL_ACC;
	mos6502->instructions[0x2C] = BIT_ABS;
	mos6502->instructions[0x2D] = AND_ABS;
	mos6502->instructions[0x2E] = ROL_ABS;
	//	MSD 3 
	mos6502->instructions[0x30] = BMI_REL;
	mos6502->instructions[0x31] = AND_INDY;
	mos6502->instructions[0x35] = AND_ZPX;
	mos6502->instructions[0x36] = ROL_ZPX;
	mos6502->instructions[0x38] = SEC_IMP;
	mos6502->instructions[0x39] = AND_ABSY;
	mos6502->instructions[0x3D] = AND_ABSX;
	mos6502->instructions[0x3E] = ROL_ABSX;
	//	MSD 4 
	mos6502->instructions[0x40] = RTI_IMP;
	mos6502->instructions[0x41] = EOR_INDX;
	mos6502->instructions[0x45] = EOR_ZP;
	mos6502->instructions[0x46] = LSR_ZP;
	mos6502->instructions[0x48] = PHA_IMP;
	mos6502->instructions[0x49] = EOR_IMM;
	mos6502->instructions[0x4A] = LSR_ACC;
	mos6502->instructions[0x4C] = JMP_ABS;
	mos6502->instructions[0x4D] = EOR_ABS;
	mos6502->instructions[0x4E] = LSR_ABS;
	//	MSD 5 
	mos6502->instructions[0x50] = BVS_REL;
	mos6502->instructions[0x51] = EOR_INDY;
	mos6502->instructions[0x55] = EOR_ZPX;
	mos6502->instructions[0x56] = LSR_ZPX;
	mos6502->instructions[0x58] = CLI_IMP;
	mos6502->instructions[0x59] = EOR_ABSY;
	mos6502->instructions[0x5D] = EOR_ABSX;
	mos6502->instructions[0x5E] = LSR_ABSX;
	//	MSD 6 
	mos6502->instructions[0x60] = RTS_IMP;
	mos6502->instructions[0x61] = ADC_INDX;
	mos6502->instructions[0x65] = ADC_ZP;
	mos6502->instructions[0x66] = ROR_ZP;
	mos6502->instructions[0x68] = PLA_IMP;
	mos6502->instructions[0x69] = ADC_IMM;
	mos6502->instructions[0x6A] = ROR_ACC;
	mos6502->instructions[0x6C] = JMP_IND;
	mos6502->instructions[0x6D] = ADC_ABS;
	mos6502->instructions[0x6E] = ROR_ABS;
	//	MSD 7 
	mos6502->instructions[0x70] = BVS_REL;
	mos6502->instructions[0x71] = ADC_INDY;
	mos6502->instructions[0x75] = ADC_ZPX;
	mos6502->instructions[0x76] = ROR_ZPX;
	mos6502->instructions[0x78] = SEI_IMP;
	mos6502->instructions[0x79] = ADC_ABSY;
	mos6502->instructions[0x7D] = ADC_ABSX;
	mos6502->instructions[0x7E] = ROR_ABSX;
	//	MSD 8 
	mos6502->instructions[0x81] = STA_INDX;
	mos6502->instructions[0x84] = STY_ZP;
	mos6502->instructions[0x85] = STA_ZP;
	mos6502->instructions[0x86] = STX_ZP;
	mos6502->instructions[0x88] = DEY_IMP;
	mos6502->instructions[0x8A] = TXA_IMP;
	mos6502->instructions[0x8C] = STY_ABS;
	mos6502->instructions[0x8D] = STA_ABS;
	mos6502->instructions[0x8E] = STX_ABS;
	//	MSD 9 
	mos6502->instructions[0x90] = BCC_REL;
	mos6502->instructions[0x91] = STA_INDY;
	mos6502->instructions[0x94] = STY_ZPX;
	mos6502->instructions[0x95] = STA_ZPX;
	mos6502->instructions[0x96] = STX_ZPY;
	mos6502->instructions[0x98] = TYA_IMP;
	mos6502->instructions[0x99] = STA_ABSY;
	mos6502->instructions[0x9A] = TXS_IMP;
	mos6502->instructions[0x9D] = STA_ABSX;
	//	MSD A 
	mos6502->instructions[0xA0] = LDY_IMM;
	mos6502->instructions[0xA1] = LDA_INDX;
	mos6502->instructions[0xA2] = LDX_IMM;
	mos6502->instructions[0xA4] = LDY_ZP;
	mos6502->instructions[0xA5] = LDA_ZP;
	mos6502->instructions[0xA6] = LDX_ZP;
	mos6502->instructions[0xA8] = TAY_IMP;
	mos6502->instructions[0xA9] = LDA_IMM;
	mos6502->instructions[0xAA] = TAX_IMP;
	mos6502->instructions[0xAC] = LDY_ABS;
	mos6502->instructions[0xAD] = LDA_ABS;
	mos6502->instructions[0xAE] = LDX_ABS;
	//	MSD B 
	mos6502->instructions[0xB0] = BCS_REL;
	mos6502->instructions[0xB1] = LDA_INDY;
	mos6502->instructions[0xB4] = LDY_ZPX;
	mos6502->instructions[0xB5] = LDA_ZPX;
	mos6502->instructions[0xB6] = LDX_ZPY;
	mos6502->instructions[0xB8] = CLV_IMP;
	mos6502->instructions[0xB9] = LDA_ABSY;
	mos6502->instructions[0xBA] = TSX_IMP;
	mos6502->instructions[0xBC] = LDY_ABSX;
	mos6502->instructions[0xBD] = LDA_ABSX;
	mos6502->instructions[0xBE] = LDX_ABSY;
	//	MSD C 
	mos6502->instructions[0xC0] = CPY_IMM;
	mos6502->instructions[0xC1] = CMP_INDX;
	mos6502->instructions[0xC4] = CPY_ZP;
	mos6502->instructions[0xC5] = CMP_ZP;
	mos6502->instructions[0xC6] = DEC_ZP;
	mos6502->instructions[0xC8] = INY_IMP;
	mos6502->instructions[0xC9] = CMP_IMM;
	mos6502->instructions[0xCA] = DEX_IMP;
	mos6502->instructions[0xCC] = CPY_ABS;
	mos6502->instructions[0xCD] = CMP_ABS;
	mos6502->instructions[0xCE] = DEC_ABS;
	//	MSD D
	mos6502->instructions[0xD0] = BNE_REL;
	mos6502->instructions[0xD1] = CMP_INDY;
	mos6502->instructions[0xD5] = CMP_ZPX;
	mos6502->instructions[0xD6] = DEC_ZPX;
	mos6502->instructions[0xD8] = CLD_IMP;
	mos6502->instructions[0xD9] = CMP_ABSY;
	mos6502->instructions[0xDD] = CMP_ABSX;
	mos6502->instructions[0xDE] = DEC_ABSX;
	//	MSD E 
	mos6502->instructions[0xE0] = CPX_IMM;
	mos6502->instructions[0xE1] = SBC_INDX;
	mos6502->instructions[0xE4] = CPX_ZP;
	mos6502->instructions[0xE5] = SBC_ZP;
	mos6502->instructions[0xE6] = INC_ZP;
	mos6502->instructions[0xE8] = INX_IMP;
	mos6502->instructions[0xE9] = SBC_IMM;
	mos6502->instructions[0xEA] = NOP_IMP;
	mos6502->instructions[0xEC] = CPX_ABS;
	mos6502->instructions[0xED] = SBC_ABS;
	mos6502->instructions[0xEE] = INC_ABS;
	//	MSD F 
	mos6502->instructions[0xF0] = BEQ_REL;
	mos6502->instructions[0xF1] = SBC_INDY;
	mos6502->instructions[0xF5] = SBC_ZPX;
	mos6502->instructions[0xF6] = INC_ZPX;
	mos6502->instructions[0xF8] = SED_IMP;
	mos6502->instructions[0xF9] = SBC_ABSY;
	mos6502->instructions[0xFD] = SBC_ABSX;
	mos6502->instructions[0xFE] = INC_ABSX;
}
