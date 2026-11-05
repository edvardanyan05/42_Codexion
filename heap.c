/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmvarda <edmvarda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/04 14:41:12 by edmvarda          #+#    #+#             */
/*   Updated: 2026/11/05 20:33:44 by edmvarda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heap.h"

int		enlarge_heap(t_heap *heap);
void	push_sort_heap(t_heap *heap);
void	pop_sort_heap(t_heap *heap);

t_heap	*init_heap(int capacity, char *scheduler)
{
	t_heap	*heap;

	heap = malloc(sizeof(t_heap));
	if (!heap)
		return (0);
	heap->nodes = malloc(sizeof(t_heap_node) * capacity);
	if (!heap->nodes)
	{
		free(heap);
		heap = 0;
		return (0);
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
	if (heap->nodes)
	{
		free(heap->nodes);
		heap->nodes = 0;
	}
	free(heap);
	heap = 0;
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
		return (0);
	res = heap->nodes[0].coder;
	heap->nodes[0].coder = heap->nodes[heap->size - 1].coder;
	heap->nodes[0].arrival_time = heap->nodes[heap->size - 1].arrival_time;
	heap->nodes[0].deadline = heap->nodes[heap->size - 1].deadline;
	heap->nodes[heap->size - 1].coder = 0;
	heap->nodes[heap->size - 1].arrival_time = -1;
	heap->nodes[heap->size - 1].deadline = -1;
	heap->size--;
	pop_sort_heap(heap);
	return (res);
}
