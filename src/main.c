/*
** main.c
** Entry point. Ties everything together.
*/

/*
	1. parse arguments -> fill config struct
	2. initialize dongles (one per coder, circular)
	3. initialize monitor struct
	4. spawn coder threads (one per coder)
	5. spawn monitor thread
	6. wait for simulation to stop (join monitor)
	7. join all coder threads
	8. cleanup (free everything, destroy mutexes)
*/
