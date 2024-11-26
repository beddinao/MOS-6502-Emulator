#include <stdio.h>
#include <stdint.h>

void	set_flag(uint8_t *va, uint8_t pos, uint8_t bit) {
	uint8_t	sr = *va;

	switch (pos) {
		case 'N':   bit ? (sr |= 128) : (sr ^= 128); break;
		case 'V':   bit ? (sr |= 64) : (sr ^= 64); break;
		case 'B':	bit ? (sr |= 16) : (sr ^= 16); break;
		case 'D':	bit ? (sr |= 8) : (sr ^= 8); break;
		case 'I':	bit ? (sr |= 4) : (sr ^= 4); break;
		case 'Z':	bit ? (sr |= 2) : (sr ^= 2); break;
		case 'C':	bit ? (sr |= 1) : (sr ^= 1); break;
	}

	*va = sr;
}

uint8_t	get_flag(uint8_t *va, uint8_t pos) {
	uint8_t	sr = *va;

	switch (pos) {
		case 'N': return (sr >> 7) & 1;
		case 'V': return (sr >> 6) & 1;
		case 'B': return (sr >> 4) & 1;
		case 'D': return (sr >> 3) & 1;
		case 'I': return (sr >> 2) & 1;
		case 'Z': return (sr >> 1) & 1;
		case 'C': return sr & 1;
		default: return 0x0;
	}
}


int     main() {
	uint16_t    a = 34918;  // 10001000 01100110
			    // 136      102
	uint8_t     fn = a >> 8;
	uint8_t     sn = a & 0x00FF;
	uint16_t    b = fn << 8 | sn;

	printf("fn %i, sn %i = b %i\n", fn, sn, b);
	//  fn 136, sn 102 = b 34918



	uint8_t     sr = 202;  // 1100 1010 (202)
			   // NV-B DIZC

	uint8_t     N = (sr >> 7) & 1,
		  V = (sr >> 6) & 1,
		  B = (sr >> 4) & 1,
		  D = (sr >> 3) & 1,
		  I = (sr >> 2) & 1,
		  Z = (sr >> 1) & 1,
		  C = sr & 1;

	uint8_t     asr = N<<7|V<<6|0<<5|B<<4|D<<3|I<<2|Z<<1|C;

	printf("sr %i, asr %i, %i%i%i%i %i%i%i%i\n", sr, asr, N, V, 0, B, D, I, Z, C);
	//  sr 202, asr 202, 1100 1010


	// 1001 1100 (156)
	// NV-B DIZC

	uint8_t     fs = 0;

	fs |= 128;
	fs |= 16;
	fs |= 8;
	fs |= 4;

	printf("fs %i 10011100\n", fs);
	// fs 156


	uint8_t     ls = 147; //  1001 0011 (147)

	ls ^= 2;
	ls ^= 1;

	printf("ls %i 10010000\n", ls);
	// ls 144

	uint8_t     va = 0;


	set_flag(&va, 'B', 1);
	printf("va %i\n", va);

	printf("va 16: %i\n", get_flag(&va, 'B'));
}
