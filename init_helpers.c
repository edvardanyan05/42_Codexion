/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmvarda <edmvarda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 20:56:38 by edmvarda          #+#    #+#             */
/*   Updated: 2026/11/05 21:05:45 by edmvarda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_heap_for_dongles(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		sim->dongles[i].heap = init_heap(sim->number_of_coders, sim->scheduler);
		if (!sim->dongles[i].heap && i != 0)
		{
			while (--i >= 0)
				free_heap(sim->dongles[i].heap);
			return (0);
		}
		else if (!sim->dongles[i].heap && i == 0)
			return (0);
		i++;
	}
	return (1);
}

void	destroy_coders(t_sim *sim, int i)
{
	printf("Mutex or Cond initialization failed\n");
	while (--i >= 0)
	{
		pthread_mutex_destroy(&sim->dongles[i].lock);
		pthread_cond_destroy(&sim->dongles[i].cond);
		pthread_mutex_destroy(&sim->coders[i].coder_lock);
	}
	free(sim->dongles);
	free(sim->coders);
}

void	init_coders_data(t_sim *sim, int i)
{
	sim->coders[i].coder_id = i + 1;
	sim->coders[i].sim = sim;
	sim->coders[i].compile_count = 0;
	sim->coders[i].is_dead = 0;
	sim->coders[i].last_compile_time = get_time_in_ms();
	sim->coders[i].left = &sim->dongles[i];
	sim->coders[i].right = &sim->dongles[(i + 1) % sim->number_of_coders];
}
