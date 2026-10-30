/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmvarda <edmvarda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/31 01:10:33 by edmvarda          #+#    #+#             */
/*   Updated: 2026/10/31 03:23:08 by edmvarda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	compile(t_coder *coder);

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->sim->number_of_coders == 1)
	{
		take_dongle(coder, coder->left);
		ft_usleep(coder->sim->time_to_burnout);
		return (NULL);
	}
	if (coder->coder_id % 2 == 0)
		ft_usleep(1);
	while (!is_simulation_ended(coder->sim))
	{
		take_both_dongles(coder);
		compile(coder);
		free_both_dongles(coder);
		print_status(coder->sim, coder->coder_id, "is debugging");
		ft_usleep(coder->sim->time_to_debug);
		print_status(coder->sim, coder->coder_id, "is refactoring");
		ft_usleep(coder->sim->time_to_refactor);
	}
	return (NULL);
}

int	is_simulation_ended(t_sim *sim)
{
	int	ended;

	pthread_mutex_lock(&sim->sim_lock);
	ended = sim->simulation_ended;
	pthread_mutex_unlock(&sim->sim_lock);
	return (ended);
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
