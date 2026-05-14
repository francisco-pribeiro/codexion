/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdinis-d <fdinis-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 16:35:54 by fdinis-d          #+#    #+#             */
/*   Updated: 2026/05/14 16:35:54 by fdinis-d         ###   ########.fr       */
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

void	init_simulation(t_simulation *sim)
{
	sim->coders = malloc(sizeof(t_coder) * sim->number_of_coders);
	sim->dongles = malloc(sizeof(t_dongle) * sim->number_of_coders);
	if (!sim->coders || !sim->dongles)
		exit(1);
	pthread_mutex_init(&sim->log_mutex, NULL);
	pthread_mutex_init(&sim->stop_mutex, NULL);
	sim->stop = 0;
	sim->start_time = get_time_ms();
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
