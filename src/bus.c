#include <mos6502.h> 

uint8_t	read_(uint8_t *ram, uint16_t addr) {
	return ram[addr];
}

void	write_(uint8_t *ram, uint16_t addr, uint8_t val) {
	ram[addr] = val;
}

uint8_t	load_ROM(_bus *bus, char *filename) {
	unsigned char buffer[PRGM_MSIZE];
	unsigned chars_read;
	FILE *file = fopen(filename, "rb");
	if (!file)
		return 0;

	/// / //	LOADING TO ROM
	memset(buffer, 0, sizeof(buffer));
	while ((chars_read = fread(buffer, 1, sizeof(buffer), file)) != 0) {
		if (chars_read + bus->rom_prgm_size > ADDR_RANGE) {
			fclose(file);
			return 0;
		}
		memcpy(bus->rom + bus->rom_prgm_size, buffer, chars_read);
		bus->rom_prgm_size += chars_read;
		memset(buffer, 0, sizeof(buffer));
	}
	fclose(file);
	if (!bus->rom_prgm_size)
		return 0;

	return 1;
}

void	bus_init(_bus *bus) {
	memset(bus->ram, 0, sizeof(bus->ram));
	memset(bus->rom, 0, sizeof(bus->rom));
	bus->ram_prgm_size = 0;
	bus->rom_prgm_size = 0;
	bus->bank_position = 0;
	bus->read = read_;
	bus->write = write_;
	bus->load_ROM = load_ROM;
	bus->ram[RSTV] = PRGM_START >> 8;
	bus->ram[RSTV+1] = PRGM_START & 0x00FF;
}

