/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdinis-d <fdinis-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 16:35:54 by fdinis-d          #+#    #+#             */
/*   Updated: 2026/05/17 02:58:27 by fdinis-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	has_stoped(t_simulation *sim)
{
	pthread_mutex_lock(&sim->stop_mutex);
	if (sim->stop)
	{
		pthread_mutex_unlock(&sim->stop_mutex);
		return (1);
	}
	pthread_mutex_unlock(&sim->stop_mutex);
	return (0);
}

int	init_simulation(t_simulation *sim)
{
	sim->coders = malloc(sizeof(t_coder) * sim->number_of_coders);
	sim->dongles = malloc(sizeof(t_dongle) * sim->number_of_coders);
	if (!sim->coders || !sim->dongles)
		return (0);
	pthread_mutex_init(&sim->log_mutex, NULL);
	pthread_mutex_init(&sim->stop_mutex, NULL);
	sim->stop = 0;
	sim->start_time = get_time_ms();
	return (1);
}

void	init_coder_and_dongles(t_simulation *sim)
{
	int	i;
	int	next;

	i = 0;
	while (i < sim->number_of_coders)
	{
		next = (i + 1) % sim->number_of_coders;
		sim->coders[i].id = i + 1;
		sim->dongles[i].id = i + 1;
		sim->coders[i].left_dongle = &sim->dongles[i];
		sim->coders[i].right_dongle = &sim->dongles[next];
		sim->coders[i].sim = sim;
		sim->coders[i].compile_count = 0;
		sim->coders[i].last_compile = sim->start_time;
		sim->dongles[i].is_available = 1;
		sim->dongles[i].queue_size = 0;
		sim->dongles[i].released_at = 0;
		pthread_mutex_init(&sim->dongles[i].mutex, NULL);
		pthread_cond_init(&sim->dongles[i].cond, NULL);
		pthread_mutex_init(&sim->coders[i].coder_mutex, NULL);
		i++;
	}
}

void	run_simulation(t_simulation *sim, pthread_t *coder_threads)
{
	pthread_t	monitor;
	int			i;

	i = 0;
	pthread_create(&monitor, NULL, monitor_routine, sim);
	while (i < sim->number_of_coders)
	{
		if (pthread_create(&coder_threads[i], NULL,
				coder_routine, &sim->coders[i]) != 0)
		{
			sim->stop = 1;
			break ;
		}
		i++;
	}
	pthread_join(monitor, NULL);
	i = 0;
	while (i < sim->number_of_coders)
		pthread_join(coder_threads[i++], NULL);
}

void	cleanup(t_simulation *sim, pthread_t *coder_threads)
{
	int	i;

	pthread_mutex_destroy(&sim->log_mutex);
	pthread_mutex_destroy(&sim->stop_mutex);
	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		pthread_cond_destroy(&sim->dongles[i].cond);
		pthread_mutex_destroy(&sim->coders[i].coder_mutex);
		i++;
	}
	free(sim->coders);
	free(sim->dongles);
	free(coder_threads);
}
