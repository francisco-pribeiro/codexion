/*
** parser.c
** Argument parsing and validation.
*/

#include "codexion.h"


// atencao que nao esta a verificar se é um int
int	ft_is_digit(char *s)
{
	int	i;

	i = -1;
	while (s[++i])
		if (s[i] < '0' || s[i] > '9')
			return (0);
	return (i > 0);
}

char	*ft_to_lower(char *s)
{
	char	*start;

	start = s;
	while (*s)
	{
		if (*s >= 'A' && *s <= 'Z')
			*s += 32;
		s++;
	}
	return (start);
}

int validate_args(int argc, char **argv)
{
	int i;

	if (argc != 9)
	{
		printf("Error: Invalid number of arguments\n");
		return (0);
	}
	i = 1;
	while (i < 8)
	{
		if (!ft_is_digit(argv[i]))
		{
			printf("Error: '%s' Argument must be a positive integer\n", argv[i]);
			return (0);
		}
		i++;
	}
	if (strcmp(ft_to_lower(argv[8]), "fifo") != 0 && strcmp(ft_to_lower(argv[8]), "edf") != 0)
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
