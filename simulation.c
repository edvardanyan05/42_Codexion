/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmvarda <edmvarda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/31 01:10:33 by edmvarda          #+#    #+#             */
/*   Updated: 2026/11/10 22:19:38 by edmvarda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	compile(t_coder *coder);
static void	print_debug_refactoring(t_coder *coder);

int	is_simulation_ended(t_sim *sim)
{
	int	ended;

	pthread_mutex_lock(&sim->sim_lock);
	ended = sim->simulation_ended;
	pthread_mutex_unlock(&sim->sim_lock);
	return (ended);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->sim->number_of_coders == 1)
	{
		take_dongle(coder, coder->left);
		while (!is_simulation_ended(coder->sim))
			ft_usleep(1000);
		return (NULL);
	}
	if (coder->coder_id % 2 == 0)
		ft_usleep(1000);
	while (!is_simulation_ended(coder->sim))
	{
		take_both_dongles(coder);
		if (is_simulation_ended(coder->sim))
		{
			free_both_dongles(coder);
			break ;
		}
		compile(coder);
		free_both_dongles(coder);
		print_debug_refactoring(coder);
	}
	return (NULL);
}

static void	print_debug_refactoring(t_coder *coder)
{
	print_status(coder->sim, coder->coder_id, "is debugging");
	ft_usleep(coder->sim->time_to_debug);
	print_status(coder->sim, coder->coder_id, "is refactoring");
	ft_usleep(coder->sim->time_to_refactor);
}

static void	compile(t_coder *coder)
{
	pthread_mutex_lock(&coder->coder_lock);
	coder->last_compile_time = get_time_in_ms();
	coder->compile_count++;
	pthread_mutex_unlock(&coder->coder_lock);
	print_status(coder->sim, coder->coder_id, "is compiling");
	ft_usleep(coder->sim->time_to_compile);
}
