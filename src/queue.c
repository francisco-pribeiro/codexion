/*
** queue.c
** Priority queue (min-heap) implementation.
** Used by each dongle to manage waiting coders.
** No standard library priority queue may be used.
*/

/*
	queue_push(queue, coder, priority):
		- insert coder with given priority
		- bubble up to maintain heap property

	queue_pop(queue):
		- remove and return coder with lowest priority value
		- bubble down to maintain heap property

	queue_peek(queue):
		- return coder with lowest priority without removing

	queue_remove(queue, coder):
		- remove a specific coder (e.g. if simulation stops mid-wait)
*/
