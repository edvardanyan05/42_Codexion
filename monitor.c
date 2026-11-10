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

static void	end_simulation(t_sim *sim);
static int	check_burnout(t_sim *sim, t_coder *coder);
static int	check_coders(t_sim *sim);

void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	while (!is_simulation_ended(sim))
	{
		if (check_coders(sim))
			break ;
		ft_usleep(1000);
	}
	return (NULL);
}

static void	end_simulation(t_sim *sim)
{
	pthread_mutex_lock(&sim->sim_lock);
	sim->simulation_ended = 1;
	pthread_mutex_unlock(&sim->sim_lock);
}

static int	check_burnout(t_sim *sim, t_coder *coder)
{
	long long	elapsed;

	elapsed = get_time_in_ms() - coder->last_compile_time;
	if (elapsed < sim->time_to_burnout)
		return (0);
	end_simulation(sim);
	print_status(sim, coder->coder_id, "burned out");
	return (1);
}

static int	check_coders(t_sim *sim)
{
	int		i;
	int		all_compiled;
	t_coder	*coder;

	i = 0;
	all_compiled = (sim->number_of_compiles_required != -1);
	while (i < sim->number_of_coders)
	{
		coder = &sim->coders[i];
		pthread_mutex_lock(&coder->coder_lock);
		if (check_burnout(sim, coder))
		{
			pthread_mutex_unlock(&coder->coder_lock);
			return (1);
		}
		if (coder->compile_count < sim->number_of_compiles_required)
			all_compiled = 0;
		pthread_mutex_unlock(&coder->coder_lock);
		i++;
	}
	if (all_compiled)
		end_simulation(sim);
	return (all_compiled);
}
