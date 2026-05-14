/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdinis-d <fdinis-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 16:35:54 by fdinis-d          #+#    #+#             */
/*   Updated: 2026/05/14 18:37:51 by fdinis-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	queue_push_fifo(t_dongle *dongle, t_coder *coder, t_simulation *sim)
{
	int		qs;
	long	last_compile;

	pthread_mutex_lock(&coder->coder_mutex);
	last_compile = coder->last_compile;
	pthread_mutex_unlock(&coder->coder_mutex);
	qs = dongle->queue_size;
	dongle->queue[qs].coder_id = coder->id;
	dongle->queue[qs].arrival_time = get_time_ms();
	dongle->queue[qs].deadline = last_compile + sim->time_to_burnout;
}

static void	queue_push_edf(t_dongle *dongle, t_coder *coder, t_simulation *sim)
{
	int		qs;
	long	last_compile;

	pthread_mutex_lock(&coder->coder_mutex);
	last_compile = coder->last_compile;
	pthread_mutex_unlock(&coder->coder_mutex);
	qs = dongle->queue_size;
	if (dongle->queue[0].deadline > last_compile + sim->time_to_burnout)
	{
		dongle->queue[1].coder_id = dongle->queue[0].coder_id;
		dongle->queue[1].arrival_time = dongle->queue[0].arrival_time;
		dongle->queue[1].deadline = dongle->queue[0].deadline;
		dongle->queue[0].coder_id = coder->id;
		dongle->queue[0].arrival_time = get_time_ms();
		dongle->queue[0].deadline = last_compile + sim->time_to_burnout;
	}
	else
	{
		dongle->queue[qs].coder_id = coder->id;
		dongle->queue[qs].arrival_time = get_time_ms();
		dongle->queue[qs].deadline = last_compile + sim->time_to_burnout;
	}
}

void	queue_push(t_dongle *dongle, t_coder *coder, t_simulation *sim)
{
	if (dongle->queue_size == 0 || strcmp(sim->scheduler, "fifo") == 0)
		queue_push_fifo(dongle, coder, sim);
	else
		queue_push_edf(dongle, coder, sim);
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
