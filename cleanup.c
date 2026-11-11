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

static void	cleanup_simulation_helper(t_sim *sim);

void	cleanup_simulation(t_sim *sim)
{
	int	i;

	if (!sim)
		return ;
	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_mutex_destroy(&sim->coders[i].coder_lock);
		pthread_mutex_destroy(&sim->dongles[i].lock);
		pthread_cond_destroy(&sim->dongles[i].cond);
		free_heap(sim->dongles[i].heap);
		i++;
	}
	cleanup_simulation_helper(sim);
}

static void	cleanup_simulation_helper(t_sim *sim)
{
	pthread_mutex_destroy(&sim->print_lock);
	pthread_mutex_destroy(&sim->sim_lock);
	if (sim->coders)
	{
		free(sim->coders);
		sim->coders = 0;
	}
	if (sim->dongles)
	{
		free(sim->dongles);
		sim->dongles = 0;
	}
}
