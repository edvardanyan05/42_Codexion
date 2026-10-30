/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmvarda <edmvarda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 21:14:08 by edmvarda          #+#    #+#             */
/*   Updated: 2026/10/30 21:24:24 by edmvarda         ###   ########.fr       */
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
	sim->dongles = malloc(sizeof(t_dongle) * sim->number_of_coders);
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

static void	init_coders_data(t_sim *sim, int i)
{
	sim->coders[i].coder_id = i + 1;
	sim->coders[i].sim = sim;
	sim->coders[i].compile_count = 0;
	sim->coders[i].is_dead = 0;
	sim->coders[i].last_compile_time = 0;
	sim->coders[i].left = &sim->dongles[i];
	sim->coders[i].right = &sim->dongles[(i + 1) % sim->number_of_coders];
}

int	init_coders(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		sim->dongles[i].dongle_id = i + 1;
		sim->dongles[i].owner_id = 0;
		sim->dongles[i].cooldown_until = 0;
		if (pthread_mutex_init(&sim->dongles[i].lock, NULL) != 0)
			return (destroy_coders(sim, i), 1);
		if (pthread_cond_init(&sim->dongles[i].cond, NULL) != 0)
		{
			pthread_mutex_destroy(&sim->dongles[i].lock);
			return (destroy_coders(sim, i), 1);
		}
		if (pthread_mutex_init(&sim->coders[i].coder_lock, NULL) != 0)
		{
			pthread_mutex_destroy(&sim->dongles[i].lock);
			pthread_cond_destroy(&sim->dongles[i].cond);
			return (destroy_coders(sim, i), 1);
		}
		init_coders_data(sim, i);
		i++;
	}
	return (0);
}
