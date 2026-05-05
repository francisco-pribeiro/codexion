/*
** parser.c
** Argument parsing and validation.
*/

/*
	parse(argc, argv, config):
		- check argument count (exactly 8)
		- validate each argument:
			- all numeric args must be positive integers
			- scheduler must be "fifo" or "edf"
		- fill config struct
		- return error if anything is invalid
*/
