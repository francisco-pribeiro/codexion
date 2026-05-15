/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdinis-d <fdinis-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 16:35:54 by fdinis-d          #+#    #+#             */
/*   Updated: 2026/05/14 16:55:59 by fdinis-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	cond_wait_cooldown(t_dongle *dongle, t_simulation *sim)
{
	struct timespec	ts;
	long			remaining;

	remaining = sim->dongle_cooldown - (get_time_ms() - dongle->released_at);
	if (remaining < 1)
		remaining = 1;
	set_timeout(&ts, remaining);
	pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
}

int	dongle_acquire(t_dongle *dongle, t_coder *coder, t_simulation *sim)
{
	pthread_mutex_lock(&dongle->mutex);
	queue_push(dongle, coder, sim);
	while (1)
	{
		if (dongle->is_available
			&& coder->id == dongle->queue[0].coder_id
			&& cooldown_elapsed(dongle, sim))
		{
			queue_pop(dongle);
			dongle->is_available = 0;
			break ;
		}
		if (has_stoped(sim))
		{
			pthread_mutex_unlock(&dongle->mutex);
			return (0);
		}
		cond_wait_cooldown(dongle, sim);
	}
	pthread_mutex_unlock(&dongle->mutex);
	return (1);
}

void	dongle_release(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->is_available = 1;
	dongle->released_at = get_time_ms();
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}
