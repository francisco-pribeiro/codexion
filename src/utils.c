/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdinis-d <fdinis-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 16:35:54 by fdinis-d          #+#    #+#             */
/*   Updated: 2026/05/14 17:35:28 by fdinis-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	ms_sleep(long ms)
{
	usleep(ms * 1000);
}

void	log_state(t_simulation *sim, int coder_id, char *message)
{
	pthread_mutex_lock(&sim->log_mutex);
	printf("%ld %d %s\n", get_time_ms() - sim->start_time, coder_id, message);
	pthread_mutex_unlock(&sim->log_mutex);
}

int	cooldown_elapsed(t_dongle *dongle, t_simulation *sim)
{
	return (get_time_ms() - dongle->released_at >= sim->dongle_cooldown);
}
