/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdinis-d <fdinis-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 16:35:54 by fdinis-d          #+#    #+#             */
/*   Updated: 2026/05/14 16:52:06 by fdinis-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	grab_dongles(t_coder *coder)
{
	t_simulation	*sim;
	t_dongle		*first;
	t_dongle		*second;

	sim = coder->sim;
	if (coder->id % 2 == 0)
	{
		first = coder->left_dongle;
		second = coder->right_dongle;
	}
	else
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	if (!dongle_acquire(first, coder, sim))
		return (0);
	log_state(sim, coder->id, "has taken a dongle");
	if (!dongle_acquire(second, coder, sim))
	{
		dongle_release(first);
		return (0);
	}
	log_state(sim, coder->id, "has taken a dongle");
	return (1);
}

static void	compile(t_coder *coder)
{
	t_simulation	*sim;

	sim = coder->sim;
	pthread_mutex_lock(&coder->coder_mutex);
	coder->last_compile = get_time_ms();
	pthread_mutex_unlock(&coder->coder_mutex);
	log_state(sim, coder->id, "is compiling");
	ms_sleep(sim->time_to_compile);
	dongle_release(coder->left_dongle);
	dongle_release(coder->right_dongle);
	pthread_mutex_lock(&coder->coder_mutex);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->coder_mutex);
}

void	*coder_routine(void *arg)
{
	t_coder			*coder;
	t_simulation	*sim;

	coder = (t_coder *)arg;
	sim = coder->sim;
	while (1)
	{
		if (!grab_dongles(coder))
			break ;
		compile(coder);
		if (has_stoped(sim)
			|| coder->compile_count >= sim->number_of_compiles_required)
			break ;
		log_state(sim, coder->id, "is debugging");
		ms_sleep(sim->time_to_debug);
		if (has_stoped(sim))
			break ;
		log_state(sim, coder->id, "is refactoring");
		ms_sleep(sim->time_to_refactor);
		if (has_stoped(sim))
			break ;
	}
	return (NULL);
}
