/*
** dongle.c
** Dongle acquisition, release, and cooldown logic.
*/

/*
	dongle_acquire(dongle, coder):
		- lock dongle mutex
		- enqueue coder in dongle's waiting queue (with timestamp or deadline)
		- loop:
			- if dongle available AND coder is first in queue AND cooldown elapsed:
				- dequeue coder
				- mark dongle as taken
				- break
			- else: wait on dongle condition variable
		- unlock dongle mutex

	dongle_release(dongle):
		- lock dongle mutex
		- mark dongle as available
		- record release timestamp (for cooldown)
		- signal waiting coders (broadcast)
		- unlock dongle mutex
*/
