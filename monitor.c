/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmvarda <edmvarda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/31 03:15:04 by edmvarda          #+#    #+#             */
/*   Updated: 2026/11/10 21:51:27 by edmvarda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	check_burnout(t_sim *sim, t_coder *coder);
static int	check_coders(t_sim *sim);

void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	while (!check_coders(sim))
		usleep(500);
	wake_all_dongles(sim);
	return (NULL);
}

void	end_simulation(t_sim *sim)
{
	pthread_mutex_lock(&sim->sim_lock);
	sim->simulation_ended = 1;
	pthread_mutex_unlock(&sim->sim_lock);
}

/*
** Wakes every coder sleeping on a dongle condition variable so that it
** sees simulation_ended and leaves. Taking the dongle lock before the
** broadcast guarantees no coder is between its check and its wait.
*/
void	wake_all_dongles(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		pthread_mutex_lock(&sim->dongles[i].lock);
		pthread_cond_broadcast(&sim->dongles[i].cond);
		pthread_mutex_unlock(&sim->dongles[i].lock);
		i++;
	}
}

static int	check_burnout(t_sim *sim, t_coder *coder)
{
	pthread_mutex_lock(&coder->coder_lock);
	if (get_time_in_ms() - coder->last_compile_time < sim->time_to_burnout)
	{
		pthread_mutex_unlock(&coder->coder_lock);
		return (0);
	}
	end_simulation(sim);
	print_status(sim, coder->coder_id, "burned out");
	pthread_mutex_unlock(&coder->coder_lock);
	return (1);
}

static int	check_coders(t_sim *sim)
{
	int	i;
	int	all_compiled;

	if (is_simulation_ended(sim))
		return (1);
	i = 0;
	all_compiled = 1;
	while (i < sim->number_of_coders)
	{
		if (check_burnout(sim, &sim->coders[i]))
			return (1);
		pthread_mutex_lock(&sim->coders[i].coder_lock);
		if (sim->coders[i].compile_count < sim->number_of_compiles_required)
			all_compiled = 0;
		pthread_mutex_unlock(&sim->coders[i].coder_lock);
		i++;
	}
	if (all_compiled)
		end_simulation(sim);
	return (all_compiled);
}
