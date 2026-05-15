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

static int	wait_for_slot(t_dongle *dongle, t_coder *coder)
{
	while ((!dongle->is_available || dongle->queue[0].coder_id != coder->id)
		&& !has_stoped(coder->sim))
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	if (has_stoped(coder->sim))
	{
		queue_pop(dongle);
		pthread_mutex_unlock(&dongle->mutex);
		return (0);
	}
	return (1);
}

static int	wait_cooldown(t_dongle *dongle, t_coder *coder)
{
	while (!cooldown_elapsed(dongle, coder->sim))
	{
		pthread_mutex_unlock(&dongle->mutex);
		if (has_stoped(coder->sim))
		{
			pthread_mutex_lock(&dongle->mutex);
			queue_pop(dongle);
			pthread_mutex_unlock(&dongle->mutex);
			return (0);
		}
		usleep(100);
		pthread_mutex_lock(&dongle->mutex);
	}
	return (1);
}

int	dongle_acquire(t_dongle *dongle, t_coder *coder, t_simulation *sim)
{
	pthread_mutex_lock(&dongle->mutex);
	queue_push(dongle, coder, sim);
	if (!wait_for_slot(dongle, coder) || !wait_cooldown(dongle, coder))
		return (0);
	queue_pop(dongle);
	dongle->is_available = 0;
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
