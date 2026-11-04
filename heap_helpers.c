/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmvarda <edmvarda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/04 16:09:33 by edmvarda          #+#    #+#             */
/*   Updated: 2026/11/04 19:37:58 by edmvarda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heap.h"

int	compare_nodes_heap(t_heap_node *n1, t_heap_node *n2, char *scheduler)
{
	if (ft_strcmp(scheduler, "fifo") == 0)
		return (n1->arrival_time < n2->arrival_time);
	else if (ft_strcmp(scheduler, "edf") == 0)
		return (n1->deadline < n2->deadline);
	return (0);
}

int	enlarge_heap(t_heap *heap)
{
	int			i;
	t_heap_node	*tmp;

	tmp = malloc(sizeof(t_heap_node) * (heap->capacity * 2));
	if (!tmp)
		return (0);
	heap->capacity *= 2;
	i = -1;
	while (++i < heap->size)
		tmp[i] = heap->nodes[i];
	free(heap->nodes);
	heap->nodes = tmp;
	return (1);
}

void	push_sort_heap(t_heap *heap)
{
	int			i;
	t_heap_node	tmp;

	i = heap->size - 1;
	while (i != 0)
	{
		if (compare_nodes_heap(&heap->nodes[(i - 1) / 2], &heap->nodes[i],
				heap->scheduler))
			break ;
		tmp = heap->nodes[(i - 1) / 2];
		heap->nodes[(i - 1) / 2] = heap->nodes[i];
		heap->nodes[i] = tmp;
		i = (i - 1) / 2;
	}
}

void	pop_sort_heap(t_heap *heap)
{
	int			i;
	int			smallest;
	int			left;
	int			right;
	t_heap_node	tmp;

	i = 0;
	while (2 * i + 1 < heap->size)
	{
		left = 2 * i + 1;
		right = 2 * i + 2;
		smallest = i;
		if (left < heap->size && compare_nodes_heap(&heap->nodes[left],
				&heap->nodes[smallest], heap->scheduler))
			smallest = left;
		if (right < heap->size && compare_nodes_heap(&heap->nodes[right],
				&heap->nodes[smallest], heap->scheduler))
			smallest = right;
		if (smallest == i)
			break ;
		tmp = heap->nodes[i];
		heap->nodes[i] = heap->nodes[smallest];
		heap->nodes[smallest] = tmp;
		i = smallest;
	}
}
