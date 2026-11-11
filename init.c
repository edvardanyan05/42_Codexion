/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmvarda <edmvarda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 21:14:08 by edmvarda          #+#    #+#             */
/*   Updated: 2026/11/11 18:42:26 by edmvarda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_global_mutexes(t_sim *sim);
static int	init_allocation(t_sim *sim);
static int	init_heaps(t_sim *sim);
static int	init_coders(t_sim *sim);

int	init_simulation(t_sim *sim)
{
	if (init_global_mutexes(sim) || init_allocation(sim) || init_heaps(sim)
		|| init_coders(sim))
	{
		cleanup_simulation(sim);
		return (1);
	}
	return (0);
}

static int	init_global_mutexes(t_sim *sim)
{
	if (pthread_mutex_init(&sim->print_lock, NULL) != 0)
		return (print_error("Error: mutex initialization failed"));
	if (pthread_mutex_init(&sim->sim_lock, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->print_lock);
		return (print_error("Error: mutex initialization failed"));
	}
	sim->locks_ready = 1;
	return (0);
}

static int	init_allocation(t_sim *sim)
{
	sim->dongles = malloc(sizeof(t_dongle) * sim->number_of_coders);
	if (!sim->dongles)
		return (print_error("Error: memory allocation failed"));
	memset(sim->dongles, 0, sizeof(t_dongle) * sim->number_of_coders);
	sim->coders = malloc(sizeof(t_coder) * sim->number_of_coders);
	if (!sim->coders)
		return (print_error("Error: memory allocation failed"));
	memset(sim->coders, 0, sizeof(t_coder) * sim->number_of_coders);
	return (0);
}

static int	init_heaps(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		sim->dongles[i].heap = init_heap(sim->number_of_coders, sim->scheduler);
		if (!sim->dongles[i].heap)
			return (print_error("Error: memory allocation failed"));
		i++;
	}
	return (0);
}

static int	init_coders(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		if (init_index_sync(sim, i))
			return (print_error("Error: mutex/cond initialization failed"));
		sim->initialized = i + 1;
		init_coders_data(sim, i);
		i++;
	}
	return (0);
}
