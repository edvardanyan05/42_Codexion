/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmvarda <edmvarda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/04 14:02:50 by edmvarda          #+#    #+#             */
/*   Updated: 2026/11/05 21:27:39 by edmvarda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAP_H
# define HEAP_H

typedef struct s_coder	t_coder;

typedef struct s_heap_node
{
	t_coder				*coder;
	long long			ticket;
	long long			deadline;
}						t_heap_node;

typedef struct s_heap
{
	t_heap_node			*nodes;
	char				*scheduler;
	int					capacity;
	int					size;
}						t_heap;

// heap.c
t_heap					*init_heap(int capacity, char *scheduler);
void					free_heap(t_heap *heap);
int						push_heap(t_heap *heap, t_heap_node node);
t_coder					*pop_heap(t_heap *heap);

// heap_helpers.c
int						enlarge_heap(t_heap *heap);
void					push_sort_heap(t_heap *heap);
void					pop_sort_heap(t_heap *heap);

#endif
