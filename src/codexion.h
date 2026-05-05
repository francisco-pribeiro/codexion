/*
** codexion.h
** All types, structs, and prototypes used across the project.
*/

/* ---- includes ---- */
/* pthread, sys/time, etc. */

/* ---- config struct ---- */
/*
	Holds all parsed arguments:
	- number_of_coders
	- time_to_burnout
	- time_to_compile
	- time_to_debug
	- time_to_refactor
	- number_of_compiles_required
	- dongle_cooldown
	- scheduler (fifo or edf)
*/

/* ---- dongle struct ---- */
/*
	One per coder (circular arrangement).
	- mutex to protect access
	- cooldown: timestamp of last release
	- waiting queue (for scheduler)
	- available flag
*/

/* ---- queue / scheduler struct ---- */
/*
	Priority queue (min-heap).
	Used by each dongle to decide which coder gets it next.
	- fifo: ordered by request arrival time
	- edf: ordered by deadline (last_compile_start + time_to_burnout)
*/

/* ---- coder struct ---- */
/*
	One per coder thread.
	- id (1 to number_of_coders)
	- pointer to left and right dongle
	- last_compile_start timestamp
	- compile count
	- pointer to shared simulation state
*/

/* ---- monitor struct ---- */
/*
	Shared simulation state watched by the monitor thread.
	- stop flag
	- mutex + condition variable to signal stop
	- pointer to all coders (to check burnout)
*/

/* ---- prototypes ---- */
/* parser.c    */
/* coder.c     */
/* dongle.c    */
/* scheduler.c */
/* monitor.c   */
/* queue.c     */
/* utils.c     */
