#include "mos6502.h"

void	load_instructions(_6502* mos6502) {
	//
	(void)mos6502;
}

int	main(int c, char **v)
{
	(void)v;
	srand(time(0));

	_6502	*mos6502 = malloc(sizeof(_6502));
	if (!mos6502) 
		return 1;
	memset(mos6502, 0, sizeof(_6502));


	/// /// /		LOADING INSTRUCTIONS
	load_instructions(mos6502);

	/// / //		CYCLE
	instruction_cycle(mos6502);
}
