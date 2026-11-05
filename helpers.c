/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmvarda <edmvarda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 20:56:38 by edmvarda          #+#    #+#             */
/*   Updated: 2026/11/05 20:57:34 by edmvarda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_heap_for_dongles(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		sim->dongles[i].heap = init_heap(sim->number_of_coders, sim->scheduler);
		if (!sim->dongles[i].heap && i != 0)
		{
			while (--i >= 0)
				free_heap(sim->dongles[i].heap);
			return (0);
		}
		else if (!sim->dongles[i].heap && i == 0)
			return (0);
		i++;
	}
	return (1);
}
