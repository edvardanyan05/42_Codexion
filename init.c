/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmvarda <edmvarda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 21:34:45 by edmvarda          #+#    #+#             */
/*   Updated: 2026/10/28 23:10:38 by edmvarda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_mutexes(t_sim *sim)
{
	if (pthread_mutex_init(&sim->print_lock, NULL) != 0)
	{
		printf("Mutex <<print_lock>> initialization failed\n");
		return (1);
	}
	if (pthread_mutex_init(&sim->sim_lock, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->print_lock);
		printf("Mutex <<sim_lock>> initialization failed\n");
		return (1);
	}
	return (0);
}

int	init_allocation(t_sim *sim)
{
	sim->coders = malloc(sizeof(t_coder) * sim->number_of_coders);
	if (!sim->coders)
	{
		printf("Memory allocation for <<Coders>> failed!\n");
		return (1);
	}
	sim->dongles = malloc(sizeof(pthread_mutex_t) * sim->number_of_coders);
	if (!sim->dongles)
	{
		free(sim->coders);
		printf("Memory allocation for <<Dongles>> failed!\n");
		return (1);
	}
	return (0);
}

static void	destroy_coders(t_sim *sim, int i)
{
	printf("Mutex <<dongles & coders.coder_lock>> initialization failed\n");
	while (--i >= 0)
	{
		pthread_mutex_destroy(&sim->dongles[i]);
		pthread_mutex_destroy(&sim->coders[i].coder_lock);
	}
	free(sim->dongles);
	free(sim->coders);
}

int	init_coders(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		if (pthread_mutex_init(&sim->dongles[i], NULL) != 0)
			return (destroy_coders(sim, i), 1);
		if (pthread_mutex_init(&sim->coders[i].coder_lock, NULL) != 0)
		{
			pthread_mutex_destroy(&sim->dongles[i]);
			return (destroy_coders(sim, i), 1);
		}
		sim->coders[i].id = i + 1;
		sim->coders[i].sim = sim;
		sim->coders[i].compilations_done = 0;
		sim->coders[i].last_compil_time = 0;
		sim->coders[i].left_dongle = &sim->dongles[i];
		sim->coders[i].right_dongle = &sim->dongles[(i + 1)
			% sim->number_of_coders];
		i++;
	}
	return (0);
}
