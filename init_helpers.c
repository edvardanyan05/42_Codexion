/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmvarda <edmvarda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 20:56:38 by edmvarda          #+#    #+#             */
/*   Updated: 2026/11/11 18:42:15 by edmvarda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_index_sync(t_sim *sim, int i)
{
	if (pthread_mutex_init(&sim->dongles[i].lock, NULL) != 0)
		return (1);
	if (pthread_cond_init(&sim->dongles[i].cond, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->dongles[i].lock);
		return (1);
	}
	if (pthread_mutex_init(&sim->coders[i].coder_lock, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->dongles[i].lock);
		pthread_cond_destroy(&sim->dongles[i].cond);
		return (1);
	}
	return (0);
}

void	init_coders_data(t_sim *sim, int i)
{
	sim->dongles[i].dongle_id = i + 1;
	sim->dongles[i].owner_id = 0;
	sim->dongles[i].cooldown_until = 0;
	sim->dongles[i].next_ticket = 0;
	sim->coders[i].coder_id = i + 1;
	sim->coders[i].sim = sim;
	sim->coders[i].compile_count = 0;
	sim->coders[i].last_compile_time = 0;
	sim->coders[i].left = &sim->dongles[i];
	sim->coders[i].right = &sim->dongles[(i + 1) % sim->number_of_coders];
}
