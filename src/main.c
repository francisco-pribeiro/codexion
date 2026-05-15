/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdinis-d <fdinis-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 16:35:54 by fdinis-d          #+#    #+#             */
/*   Updated: 2026/05/15 16:25:36 by fdinis-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int ac, char **av)
{
	t_simulation	sim;
	pthread_t		*coder_threads;

	if (!parse_args(ac, av, &sim))
		return (1);
	coder_threads = malloc(sizeof(pthread_t) * sim.number_of_coders);
	if (!coder_threads)
		return (1);
	if (!init_simulation(&sim))
		return (free(coder_threads), 1);
	init_coder_and_dongles(&sim);
	run_simulation(&sim, coder_threads);
	cleanup(&sim, coder_threads);
	return (0);
}
