/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmvarda <edmvarda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 21:40:09 by edmvarda          #+#    #+#             */
/*   Updated: 2026/10/30 22:14:26 by edmvarda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	take_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->lock);
	while (dongle->owend_id != 0 || get_time_in_ms() < dongle->cooldown_until)
		pthread_cond_wait(&dongle->cond, &dongle->lock);
	dongle->owend_id = coder->coder_id;
	print_status(coder->sim, coder->coder_id, "has taken a dongle");
	pthread_mutex_unlock(&dongle->lock);
}

void	free_dongle(t_sim *sim, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->lock);
	dongle->owend_id = 0;
	dongle->cooldown_until = get_time_in_ms() + sim->dongle_cooldown;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->lock);
}
