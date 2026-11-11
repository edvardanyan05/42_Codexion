/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmvarda <edmvarda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 21:40:09 by edmvarda          #+#    #+#             */
/*   Updated: 2026/11/11 18:41:50 by edmvarda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	wait_for_dongle(t_coder *coder, t_dongle *dongle)
{
	struct timespec	ts;

	while (!is_simulation_ended(coder->sim) && (dongle->owner_id != 0
			|| dongle->heap->nodes[0].coder != coder
			|| get_time_in_ms() < dongle->cooldown_until))
	{
		if (dongle->owner_id == 0 && dongle->heap->nodes[0].coder == coder)
		{
			ms_to_timespec(dongle->cooldown_until, &ts);
			pthread_cond_timedwait(&dongle->cond, &dongle->lock, &ts);
		}
		else
			pthread_cond_wait(&dongle->cond, &dongle->lock);
	}
}

int	take_dongle(t_coder *coder, t_dongle *dongle)
{
	t_heap_node	node;

	pthread_mutex_lock(&dongle->lock);
	node.coder = coder;
	node.ticket = dongle->next_ticket;
	dongle->next_ticket++;
	node.deadline = coder->last_compile_time + coder->sim->time_to_burnout;
	if (!push_heap(dongle->heap, node))
	{
		pthread_mutex_unlock(&dongle->lock);
		return (0);
	}
	wait_for_dongle(coder, dongle);
	if (is_simulation_ended(coder->sim))
	{
		pthread_mutex_unlock(&dongle->lock);
		return (0);
	}
	pop_heap(dongle->heap);
	dongle->owner_id = coder->coder_id;
	print_status(coder->sim, coder->coder_id, "has taken a dongle");
	pthread_mutex_unlock(&dongle->lock);
	return (1);
}

int	take_both_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	first = coder->left;
	second = coder->right;
	if (first->dongle_id > second->dongle_id)
	{
		first = coder->right;
		second = coder->left;
	}
	if (!take_dongle(coder, first))
		return (0);
	if (!take_dongle(coder, second))
	{
		free_dongle(coder->sim, first);
		return (0);
	}
	return (1);
}

void	free_dongle(t_sim *sim, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->lock);
	dongle->owner_id = 0;
	dongle->cooldown_until = get_time_in_ms() + sim->dongle_cooldown;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->lock);
}

void	free_both_dongles(t_coder *coder)
{
	free_dongle(coder->sim, coder->left);
	free_dongle(coder->sim, coder->right);
}
