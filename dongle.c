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

static void	order_dongles(t_coder *coder, t_dongle **first, t_dongle **second);
static int	wait_for_both(t_coder *coder, t_dongle *first, t_dongle *second);
static void	grab_both(t_coder *coder, t_dongle *first, t_dongle *second);

int	take_both_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;
	int			ok;

	order_dongles(coder, &first, &second);
	pthread_mutex_lock(&first->lock);
	pthread_mutex_lock(&second->lock);
	ok = (join_queue(coder, first) && join_queue(coder, second)
			&& wait_for_both(coder, first, second));
	if (ok)
		grab_both(coder, first, second);
	pthread_mutex_unlock(&second->lock);
	pthread_mutex_unlock(&first->lock);
	return (ok);
}

void	free_both_dongles(t_coder *coder)
{
	release_dongle(coder->sim, coder->left);
	release_dongle(coder->sim, coder->right);
}

static void	order_dongles(t_coder *coder, t_dongle **first, t_dongle **second)
{
	*first = coder->left;
	*second = coder->right;
	if ((*first)->dongle_id > (*second)->dongle_id)
	{
		*first = coder->right;
		*second = coder->left;
	}
}

static int	wait_for_both(t_coder *coder, t_dongle *first, t_dongle *second)
{
	struct timespec	ts;

	while (!is_simulation_ended(coder->sim))
	{
		if (is_dongle_ready(coder, first) && is_dongle_ready(coder, second))
			return (1);
		pthread_mutex_unlock(&second->lock);
		timespec_from_now(&ts, 500);
		pthread_cond_timedwait(&first->cond, &first->lock, &ts);
		pthread_mutex_lock(&second->lock);
	}
	return (0);
}

static void	grab_both(t_coder *coder, t_dongle *first, t_dongle *second)
{
	pop_heap(first->heap);
	pop_heap(second->heap);
	first->owner_id = coder->coder_id;
	second->owner_id = coder->coder_id;
}
