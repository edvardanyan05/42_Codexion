/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmvarda <edmvarda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 21:40:09 by edmvarda          #+#    #+#             */
/*   Updated: 2026/11/05 21:52:44 by edmvarda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	take_dongle(t_coder *coder, t_dongle *dongle)
{
	t_heap_node	node;

	node.coder = coder;
	node.arrival_time = get_time_in_ms();
	node.deadline = coder->last_compile_time + coder->sim->time_to_burnout;
	pthread_mutex_lock(&dongle->lock);
	while (!is_simulation_ended(coder->sim) && (dongle->owner_id != 0
			|| get_time_in_ms() < dongle->cooldown_until
			|| (dongle->heap->size > 0
				&& dongle->heap->nodes[0].coder != coder)))
	{
		if (!is_node_in_heap(dongle->heap, node))
			push_heap(dongle->heap, node);
		if (dongle->owner_id == 0 && get_time_in_ms() < dongle->cooldown_until)
		{
			pthread_mutex_unlock(&dongle->lock);
			ft_usleep(1000);
			pthread_mutex_lock(&dongle->lock);
			continue ;
		}
		pthread_cond_wait(&dongle->cond, &dongle->lock);
	}
	if (is_simulation_ended(coder->sim))
	{
		pthread_mutex_unlock(&dongle->lock);
		return (0);
	}
	dongle->owner_id = coder->coder_id;
	pop_heap(dongle->heap);
	print_status(coder->sim, coder->coder_id, "has taken a dongle");
	pthread_mutex_unlock(&dongle->lock);
	return (1);
}

void	take_both_dongles(t_coder *coder)
{
	if (coder->left->dongle_id < coder->right->dongle_id)
	{
		take_dongle(coder, coder->left);
		take_dongle(coder, coder->right);
	}
	else
	{
		take_dongle(coder, coder->right);
		take_dongle(coder, coder->left);
	}
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
