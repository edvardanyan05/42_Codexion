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
		printf("Mutex <print_lock> initialization failed\n");
		return (1);
	}
	if (pthread_mutex_init(&sim->sim_lock, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->print_lock);
		printf("Mutex <sim_lock> initialization failed\n");
		return (1);
	}
	return (0);
}

int	init_allocation(t_sim *sim)
{
	sim->coders = malloc(sizeof(t_coder) * sim->number_of_coders);
	if (!sim->coders)
	{
		printf("Memory allocation for Coders failed!\n");
		return (1);
	}
	sim->dongles = malloc(sizeof(pthread_mutex_t) * sim->number_of_coders);
	if (!sim->dongles)
	{
		free(sim->coders);
		printf("Memory allocation for Dongles failed!\n");
		return (1);
	}
	return (0);
}

int	init_coders(t_sim *sim)
{
	int	i;
	int	j;

	i = -1;
	j = 0;
	while (++i < sim->number_of_coders - 1)
	{
		if (pthread_mutex_init(&sim->dongles[i], NULL) != 0)
		{
			while (j < i)
			{
				pthread_mutex_destroy(&sim->dongles[j]);
				pthread_mutex_destroy(&sim->coders[j++]);
			}
			return (free(sim->dongles), free(sim->coders), 1);
		}
		if (pthread_mutex_init(&sim->coders[i], NULL) != 0)
		{
			while (j < i)
			{
				pthread_mutex_destroy(&sim->dongles[j]);
				pthread_mutex_destroy(&sim->coders[j++]);
			}
			pthread_mutex_destroy(&sim->dongles[i]);
			return (free(sim->dongles), free(sim->coders), 1);
		}
	}
	return (0);
}
