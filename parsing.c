/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmvarda <edmvarda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 22:29:33 by edmvarda          #+#    #+#             */
/*   Updated: 2026/11/11 18:43:06 by edmvarda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_valid_number(char *s);
static void	set_values(char **argv, t_sim *sim);

int	parse_args(int argc, char **argv, t_sim *sim)
{
	int	i;

	if (argc != 9)
		return (print_error("Usage: ./codexion number_of_coders "
				"time_to_burnout time_to_compile time_to_debug "
				"time_to_refactor number_of_compiles_required "
				"dongle_cooldown scheduler"));
	i = 1;
	while (i < 8)
	{
		if (!is_valid_number(argv[i]))
			return (print_error("Error: arguments must be integers "
					"between 0 and 2147483647"));
		i++;
	}
	if (ft_strcmp(argv[8], "fifo") != 0 && ft_strcmp(argv[8], "edf") != 0)
		return (print_error("Error: scheduler must be 'fifo' or 'edf'"));
	set_values(argv, sim);
	if (sim->number_of_coders < 1)
		return (print_error("Error: number_of_coders must be at least 1"));
	return (0);
}

static int	is_valid_number(char *s)
{
	int	i;

	if (!s || !s[0])
		return (0);
	i = 0;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	if (i > 10)
		return (0);
	return (ft_atoll(s) <= INT_MAX);
}

static void	set_values(char **argv, t_sim *sim)
{
	sim->number_of_coders = (int)ft_atoll(argv[1]);
	sim->time_to_burnout = ft_atoll(argv[2]);
	sim->time_to_compile = ft_atoll(argv[3]);
	sim->time_to_debug = ft_atoll(argv[4]);
	sim->time_to_refactor = ft_atoll(argv[5]);
	sim->number_of_compiles_required = (int)ft_atoll(argv[6]);
	sim->dongle_cooldown = ft_atoll(argv[7]);
	sim->scheduler = argv[8];
}
