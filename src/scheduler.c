/*
** scheduler.c
** Arbitration logic: decides which coder gets a dongle when multiple are waiting.
*/

/*
	fifo:
		- coders are served in order of request arrival time
		- queue ordered by timestamp

	edf (earliest deadline first):
		- coders are served by urgency: who will burn out soonest
		- deadline = last_compile_start + time_to_burnout
		- queue ordered by deadline (min first)
		- tie-breaker needed for equal deadlines (e.g. by coder id)

	Both policies use the same priority queue structure (queue.c),
	just with different comparison functions.
*/
