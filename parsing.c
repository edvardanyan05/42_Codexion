/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmvarda <edmvarda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 22:29:33 by edmvarda          #+#    #+#             */
/*   Updated: 2026/10/27 23:01:38 by edmvarda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_valid_number(char *s)
{
	int	i;

	i = 0;
	if (!s || !s[0])
		return (0);
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	is_valid_scheduler(char *scheduler)
{
	if (!scheduler)
		return (0);
	if (ft_strcmp(scheduler, "fifo") != 0 && ft_strcmp(scheduler, "edf") != 0)
		return (0);
	return (1);
}

static int	parse_times(char **argv, t_sim *sim)
{
	if (!is_valid_number(argv[1]) || !is_valid_number(argv[2])
		|| !is_valid_number(argv[3]) || !is_valid_number(argv[4])
		|| !is_valid_number(argv[5]) || !is_valid_number(argv[6]))
	{
		printf("Error: Arguments must be positive integers!");
		return (1);
	}
	sim->number_of_coders = (int)ft_atoll(argv[1]);
	sim->time_to_burnout = ft_atoll(argv[2]);
	sim->time_to_compile = ft_atoll(argv[3]);
	sim->time_to_debug = ft_atoll(argv[4]);
	sim->time_to_refactor = ft_atoll(argv[5]);
	sim->dongle_cooldown = ft_atoll(argv[6]);
	return (0);
}

static int	parse_optional(int argc, char **argv, t_sim *sim)
{
	if (argc == 9)
	{
		if (!is_valid_number(argv[7]))
		{
			printf("Error: Number of compiles must be an integer!");
			return (1);
		}
		sim->number_of_compiles_required = (int)ft_atoll(argv[7]);
		sim->scheduler = argv[8];
	}
	else
	{
		sim->number_of_compiles_required = -1;
		sim->scheduler = argv[7];
	}
	return (0);
}

int	parse_args(int argc, char **argv, t_sim *sim)
{
	if (argc < 8 || argc > 9)
	{
		printf("Error: Invalid number of arguments!");
		return (1);
	}
	if (parse_times(argv, sim) || parse_optional(argc, argv, sim))
		return (1);
	if (!is_valid_scheduler(sim->scheduler))
	{
		printf("Error: Invalid scheduler! Use 'fifo' or 'edf'.");
		return (1);
	}
	sim->simulation_ended = 0;
	sim->start_time = 0;
	sim->coders = NULL;
	sim->dongles = NULL;
	return (0);
}
