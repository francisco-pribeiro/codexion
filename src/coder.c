/*
** coder.c
** Coder thread logic.
*/

/*
	coder_routine(coder):
		loop:
			- try to acquire left dongle
			- try to acquire right dongle
			- if both acquired:
				- log "has taken a dongle" (x2)
				- record last_compile_start
				- log "is compiling"
				- sleep(time_to_compile)
				- release both dongles (triggers cooldown)
				- increment compile count
				- if compile count >= number_of_compiles_required -> stop
				- log "is debugging"
				- sleep(time_to_debug)
				- log "is refactoring"
				- sleep(time_to_refactor)
			- check stop flag each iteration
			- check burnout: if time since last compile > time_to_burnout -> burned out
*/
