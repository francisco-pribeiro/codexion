/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdinis-d <fdinis-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 16:35:54 by fdinis-d          #+#    #+#             */
/*   Updated: 2026/05/15 14:02:04 by fdinis-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	ft_is_digit(char *s)
{
	int	i;

	i = -1;
	while (s[++i])
		if (s[i] < '0' || s[i] > '9')
			return (0);
	return (i > 0);
}

int	ft_is_valid_int(char *s)
{
	long	n;

	if (!ft_is_digit(s))
		return (0);
	n = 0;
	while (*s)
	{
		if (n > (INT_MAX - (*s - '0')) / 10)
			return (0);
		n = n * 10 + (*s++ - '0');
	}
	return (n > 0);
}

static int	validate_numeric(char **argv)
{
	int	i;

	i = 1;
	while (i < 7)
	{
		if (!ft_is_valid_int(argv[i]))
		{
			printf("Error: '%s' "
				"Argument must be a positive integer\n", argv[i]);
			return (0);
		}
		i++;
	}
	if (!ft_is_digit(argv[7]))
	{
		printf("Error: '%s' "
			"Argument must be a non-negative integer\n", argv[7]);
		return (0);
	}
	return (1);
}

int	validate_args(int argc, char **argv)
{
	char	*sched;
	int		j;

	if (argc != 9)
	{
		printf("Error: Invalid number of arguments\n");
		return (0);
	}
	if (!validate_numeric(argv))
		return (0);
	sched = argv[8];
	j = -1;
	while (sched[++j])
		if (sched[j] >= 'A' && sched[j] <= 'Z')
			sched[j] += 32;
	if (strcmp(sched, "fifo") != 0 && strcmp(sched, "edf") != 0)
	{
		printf("Error: '%s' must be 'fifo' or 'edf'\n", argv[8]);
		return (0);
	}
	return (1);
}

int	parse_args(int ac, char **av, t_simulation *sim)
{
	if (!validate_args(ac, av))
		return (0);
	sim->number_of_coders = atoi(av[1]);
	sim->time_to_burnout = atoi(av[2]);
	sim->time_to_compile = atoi(av[3]);
	sim->time_to_debug = atoi(av[4]);
	sim->time_to_refactor = atoi(av[5]);
	sim->number_of_compiles_required = atoi(av[6]);
	sim->dongle_cooldown = atoi(av[7]);
	sim->scheduler = av[8];
	return (1);
}
