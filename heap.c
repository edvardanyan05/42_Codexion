/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmvarda <edmvarda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/04 14:41:12 by edmvarda          #+#    #+#             */
/*   Updated: 2026/11/11 18:42:09 by edmvarda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_heap	*init_heap(int capacity, char *scheduler)
{
	t_heap	*heap;

	heap = malloc(sizeof(t_heap));
	if (!heap)
		return (NULL);
	heap->nodes = malloc(sizeof(t_heap_node) * capacity);
	if (!heap->nodes)
	{
		free(heap);
		return (NULL);
	}
	heap->capacity = capacity;
	heap->scheduler = scheduler;
	heap->size = 0;
	return (heap);
}

void	free_heap(t_heap *heap)
{
	if (!heap)
		return ;
	free(heap->nodes);
	free(heap);
}

int	push_heap(t_heap *heap, t_heap_node node)
{
	if (heap->size >= heap->capacity && !enlarge_heap(heap))
		return (0);
	heap->nodes[heap->size] = node;
	heap->size++;
	push_sort_heap(heap);
	return (1);
}

t_coder	*pop_heap(t_heap *heap)
{
	t_coder	*res;

	if (heap->size == 0)
		return (NULL);
	res = heap->nodes[0].coder;
	heap->nodes[0] = heap->nodes[heap->size - 1];
	heap->size--;
	pop_sort_heap(heap);
	return (res);
}
