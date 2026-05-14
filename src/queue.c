#include "codexion.h"

void	queue_push(t_dongle *dongle, t_coder *coder, t_simulation *sim)
{
	if (dongle->queue_size == 0 || strcmp(sim->scheduler, "fifo") == 0)
	{
		dongle->queue[dongle->queue_size].coder_id = coder->id;
		dongle->queue[dongle->queue_size].arrival_time = get_time_ms();
		dongle->queue[dongle->queue_size].deadline = coder->last_compile + sim->time_to_burnout;
	}
	else
	{
		if (dongle->queue[0].deadline > coder->last_compile + sim->time_to_burnout)
		{
			dongle->queue[1].coder_id = dongle->queue[0].coder_id;
			dongle->queue[1].arrival_time = dongle->queue[0].arrival_time;
			dongle->queue[1].deadline = dongle->queue[0].deadline;

			dongle->queue[0].coder_id = coder->id;
			dongle->queue[0].arrival_time = get_time_ms();
			dongle->queue[0].deadline = coder->last_compile + sim->time_to_burnout;
		}
		else
		{
			dongle->queue[dongle->queue_size].coder_id = coder->id;
			dongle->queue[dongle->queue_size].arrival_time = get_time_ms();
			dongle->queue[dongle->queue_size].deadline = coder->last_compile + sim->time_to_burnout;
		}
	}
	dongle->queue_size++;
}

// pop from 'top' of the queue and decrement its size
int	queue_pop(t_dongle *dongle)
{
	int	res;

	if (dongle->queue_size == 0)
		return (-1);
	res = dongle->queue[0].coder_id;
	if (dongle->queue_size == 2)
	{
		dongle->queue[0].coder_id = dongle->queue[1].coder_id;
		dongle->queue[0].arrival_time = dongle->queue[1].arrival_time;
		dongle->queue[0].deadline = dongle->queue[1].deadline;
	}
	dongle->queue_size--;
	return (res);
}
