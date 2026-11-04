/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmvarda <edmvarda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/04 14:02:50 by edmvarda          #+#    #+#             */
/*   Updated: 2026/11/04 14:02:51 by edmvarda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

typedef struct s_heap_node
{
	t_coder		*coder;
	long long	arrival_time;
	long long	deadline;
}				t_heap_node;

typedef struct s_heap
{
	t_heap_node	*nodes;
	int			capacity;
	int			size;
}				t_heap;
