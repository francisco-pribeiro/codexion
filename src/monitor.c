/*
** monitor.c
** Monitor thread: detects burnout and stops the simulation.
*/

/*
	monitor_routine(monitor):
		loop:
			- check each coder:
				- if time since last compile > time_to_burnout:
					- log "X burned out" (within 10ms of actual burnout)
					- set stop flag
					- wake all waiting threads
					- return
			- check if all coders have reached number_of_compiles_required:
				- set stop flag
				- return
			- sleep a small interval before next check (e.g. 1ms)
*/
