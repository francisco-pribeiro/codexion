/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdinis-d <fdinis-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 16:35:54 by fdinis-d          #+#    #+#             */
/*   Updated: 2026/05/14 16:35:54 by fdinis-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int ac, char **av)
{
	int				i;
	t_simulation	sim;
	pthread_t		monitor;
	pthread_t		*coder_threads;

	if (!parse_args(ac, av, &sim))
		return (1);
	coder_threads = malloc(sizeof(pthread_t) * sim.number_of_coders);
	if (!coder_threads)
		return (1);
	init_simulation(&sim);
	init_coder_and_dongles(&sim);
	pthread_create(&monitor, NULL, monitor_routine, &sim);
	i = 0;
	while (i < sim.number_of_coders)
	{
		pthread_create(&coder_threads[i], NULL, coder_routine, &sim.coders[i]);
		i++;
	}
	pthread_join(monitor, NULL);
	i = 0;
	while (i < sim.number_of_coders)
		pthread_join(coder_threads[i++], NULL);
	cleanup(&sim, coder_threads);
	return (0);
}
