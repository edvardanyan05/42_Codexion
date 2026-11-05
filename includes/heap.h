/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmvarda <edmvarda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/04 14:02:50 by edmvarda          #+#    #+#             */
/*   Updated: 2026/11/05 19:37:57 by edmvarda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAP_H
# define HEAP_H

# include "codexion.h"

typedef struct s_heap_node
{
	t_coder		*coder;
	long long	arrival_time;
	long long	deadline;
}				t_heap_node;

typedef struct s_heap
{
	t_heap_node	*nodes;
	char		*scheduler;
	int			capacity;
	int			size;
}				t_heap;

t_heap			*init_heap(int capacity, char *scheduler);
void			free_heap(t_heap *heap);
int				push_heap(t_heap *heap, t_heap_node node);
t_coder			*pop_heap(t_heap *heap);

#endif