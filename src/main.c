#include "mos6502.h"

void	sig_handle(int s) {
	pthread_mutex_lock(&thread_data->halt_mutex);
	thread_data->halt = 1;
	pthread_mutex_unlock(&thread_data->halt_mutex);
	pthread_join(thread_data->worker, NULL);
	pthread_mutex_destroy(&thread_data->halt_mutex);
	pthread_mutex_destroy(&thread_data->data_mutex);
	free(thread_data->mos6502->bus);
	free(thread_data->mos6502);
	free(thread_data);
	exit(s);
}

int	main(int c, char **v) {
	if (c != 2) {
		printf("usage ./mos6502 [ROM]\n");
		return 1;
	}
	srand(time(0));

	/// / //		BUS
	_bus	*bus = malloc(sizeof(_bus));
	if (!bus) 
		return 1;
	memset(bus, 0, sizeof(_bus));
	bus->reset = bus_init;
	bus->reset(bus);

	//// / /		CPU
	_6502	*mos6502 = malloc(sizeof(_6502));
	if (!mos6502) { 
		free(bus);
		return 1;
	}
	memset(mos6502, 0, sizeof(_6502));
	mos6502->bus = bus;
	mos6502->reset = cpu_init;
	mos6502->reset(mos6502);

	//// / /		THREAD INFO
	thread_data = malloc(sizeof(_worker));
	if (!thread_data) {
		free(bus);
		free(mos6502);
		return 1;
	}
	memset(thread_data, 0, sizeof(_worker));
	thread_data->mos6502 = mos6502;

	// / ///		ROM
	if (!bus->load_ROM(bus, v[1])) {
		printf("failed to load program to memory\n");
		free(mos6502);
		free(bus);
		return 1;
	}
	mos6502->load_ROM(bus);

	/// // /		OUTPUT WORKER
	pthread_mutex_init(&thread_data->halt_mutex, NULL);
	pthread_mutex_init(&thread_data->data_mutex, NULL);
	pthread_create(&thread_data->worker, NULL, print_state, thread_data);

	// / //		CLEAN
	signal(SIGINT, sig_handle);
	signal(SIGQUIT, sig_handle);
	signal(SIGTERM, sig_handle);

	/// / //		CYCLE
	mos6502->instruction_cycle(thread_data);
}
