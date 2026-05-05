/*
** utils.c
** Timing and logging helpers.
*/

/*
	get_time_ms():
		- returns current time in milliseconds using gettimeofday()

	log_state(monitor, coder_id, message):
		- lock log mutex
		- print: "[timestamp] [coder_id] [message]"
		- unlock log mutex

	ms_sleep(ms):
		- sleep for given milliseconds using usleep()
*/
