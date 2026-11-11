/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmvarda <edmvarda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 19:00:00 by edmvarda          #+#    #+#             */
/*   Updated: 2026/11/11 19:00:00 by edmvarda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	join_queue(t_coder *coder, t_dongle *dongle)
{
	t_heap_node	node;

	node.coder = coder;
	node.ticket = dongle->next_ticket;
	dongle->next_ticket++;
	node.deadline = coder->last_compile_time + coder->sim->time_to_burnout;
	return (push_heap(dongle->heap, node));
}

int	is_dongle_ready(t_coder *coder, t_dongle *dongle)
{
	return (dongle->owner_id == 0
		&& dongle->heap->size > 0
		&& dongle->heap->nodes[0].coder == coder
		&& get_time_in_ms() >= dongle->cooldown_until);
}

void	release_dongle(t_sim *sim, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->lock);
	dongle->owner_id = 0;
	dongle->cooldown_until = get_time_in_ms() + sim->dongle_cooldown;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->lock);
}
