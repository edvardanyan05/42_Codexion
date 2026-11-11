/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmvarda <edmvarda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 17:19:40 by edmvarda          #+#    #+#             */
/*   Updated: 2026/11/11 17:42:19 by edmvarda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	free_heaps(t_sim *sim);

/*
** Safe to call after a partial initialization:
** only initialized mutexes/conds are destroyed, NULL pointers are skipped.
*/
void	cleanup_simulation(t_sim *sim)
{
	int	i;

	if (!sim)
		return ;
	i = 0;
	while (i < sim->initialized)
	{
		pthread_mutex_destroy(&sim->coders[i].coder_lock);
		pthread_mutex_destroy(&sim->dongles[i].lock);
		pthread_cond_destroy(&sim->dongles[i].cond);
		i++;
	}
	sim->initialized = 0;
	free_heaps(sim);
	if (sim->locks_ready)
	{
		pthread_mutex_destroy(&sim->print_lock);
		pthread_mutex_destroy(&sim->sim_lock);
		sim->locks_ready = 0;
	}
	free(sim->coders);
	sim->coders = NULL;
	free(sim->dongles);
	sim->dongles = NULL;
}

static void	free_heaps(t_sim *sim)
{
	int	i;

	if (!sim->dongles)
		return ;
	i = 0;
	while (i < sim->number_of_coders)
	{
		free_heap(sim->dongles[i].heap);
		sim->dongles[i].heap = NULL;
		i++;
	}
}
