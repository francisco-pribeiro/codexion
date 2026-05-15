/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdinis-d <fdinis-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 16:35:54 by fdinis-d          #+#    #+#             */
/*   Updated: 2026/05/14 17:33:14 by fdinis-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// safely stops simulation locking the mutex
static void	stop_simulation(t_simulation *sim)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&sim->stop_mutex);
	sim->stop = 1;
	pthread_mutex_unlock(&sim->stop_mutex);
	while (i < sim->number_of_coders)
	{
		pthread_mutex_lock(&sim->dongles[i].mutex);
		pthread_cond_broadcast(&sim->dongles[i].cond);
		pthread_mutex_unlock(&sim->dongles[i].mutex);
		i++;
	}
}

// check if all coders compiled required times
static int	all_compiled_enough(t_simulation *sim, t_coder *coders)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_mutex_lock(&coders[i].coder_mutex);
		if (coders[i].compile_count < sim->number_of_compiles_required)
		{
			pthread_mutex_unlock(&coders[i].coder_mutex);
			return (0);
		}
		pthread_mutex_unlock(&coders[i++].coder_mutex);
	}
	return (1);
}

// check of any coder burned out
static int	any_burned_out(t_simulation *sim, t_coder *coders)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_mutex_lock(&coders[i].coder_mutex);
		if (coders[i].compile_count < sim->number_of_compiles_required
			&& get_time_ms() - coders[i].last_compile >= sim->time_to_burnout)
		{
			pthread_mutex_unlock(&coders[i].coder_mutex);
			log_state(sim, coders[i].id, "burned out");
			stop_simulation(sim);
			return (1);
		}
		pthread_mutex_unlock(&coders[i].coder_mutex);
		i++;
	}
	return (0);
}

// function passed to monitor thread - monitors burn outs
void	*monitor_routine(void *arg)
{
	t_simulation	*sim;
	t_coder			*coders;

	sim = (t_simulation *)arg;
	coders = sim->coders;
	while (1)
	{
		if (any_burned_out(sim, coders))
			return (NULL);
		if (all_compiled_enough(sim, coders))
		{
			stop_simulation(sim);
			return (NULL);
		}
		ms_sleep(1);
	}
}
