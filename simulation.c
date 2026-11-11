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
static void	debug_and_refactor(t_coder *coder);

int	is_simulation_ended(t_sim *sim)
{
	int	ended;

	pthread_mutex_lock(&sim->sim_lock);
	ended = sim->simulation_ended;
	pthread_mutex_unlock(&sim->sim_lock);
	return (ended);
}

/*
** With a single coder there is only one dongle: it can be taken but
** never paired, so the coder waits until the monitor detects burnout.
*/
void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->sim->number_of_coders == 1)
	{
		take_dongle(coder, coder->left);
		while (!is_simulation_ended(coder->sim))
			usleep(500);
		return (NULL);
	}
	while (!is_simulation_ended(coder->sim))
	{
		if (!take_both_dongles(coder))
			break ;
		compile(coder);
		free_both_dongles(coder);
		debug_and_refactor(coder);
	}
	return (NULL);
}

static void	compile(t_coder *coder)
{
	pthread_mutex_lock(&coder->coder_lock);
	coder->last_compile_time = get_time_in_ms();
	coder->compile_count++;
	pthread_mutex_unlock(&coder->coder_lock);
	print_status(coder->sim, coder->coder_id, "is compiling");
	ft_usleep(coder->sim->time_to_compile, coder->sim);
}

static void	debug_and_refactor(t_coder *coder)
{
	print_status(coder->sim, coder->coder_id, "is debugging");
	ft_usleep(coder->sim->time_to_debug, coder->sim);
	print_status(coder->sim, coder->coder_id, "is refactoring");
	ft_usleep(coder->sim->time_to_refactor, coder->sim);
}
