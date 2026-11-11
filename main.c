/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmvarda <edmvarda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 18:00:00 by edmvarda          #+#    #+#             */
/*   Updated: 2026/11/11 18:40:57 by edmvarda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	run_simulation(t_sim *sim);
static int	abort_simulation(t_sim *sim, int created);
static void	join_coders(t_sim *sim, int count);

int	main(int argc, char **argv)
{
	t_sim	sim;
	int		status;

	memset(&sim, 0, sizeof(t_sim));
	if (parse_args(argc, argv, &sim))
		return (1);
	if (init_simulation(&sim))
		return (1);
	status = run_simulation(&sim);
	cleanup_simulation(&sim);
	return (status);
}

static int	run_simulation(t_sim *sim)
{
	pthread_t	monitor;
	int			i;

	sim->start_time = get_time_in_ms();
	i = -1;
	while (++i < sim->number_of_coders)
		sim->coders[i].last_compile_time = sim->start_time;
	i = 0;
	while (i < sim->number_of_coders)
	{
		if (pthread_create(&sim->coders[i].thread, NULL, coder_routine,
				&sim->coders[i]) != 0)
			return (abort_simulation(sim, i));
		i++;
	}
	if (pthread_create(&monitor, NULL, monitor_routine, sim) != 0)
		return (abort_simulation(sim, i));
	join_coders(sim, sim->number_of_coders);
	pthread_join(monitor, NULL);
	return (0);
}

static int	abort_simulation(t_sim *sim, int created)
{
	end_simulation(sim);
	wake_all_dongles(sim);
	join_coders(sim, created);
	return (print_error("Error: thread creation failed"));
}

static void	join_coders(t_sim *sim, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
}
