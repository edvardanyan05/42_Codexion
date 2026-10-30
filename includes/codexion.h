/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmvarda <edmvarda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 22:31:36 by edmvarda          #+#    #+#             */
/*   Updated: 2026/10/28 22:33:03 by edmvarda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include "types.h"
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

// utils.c
int			ft_strcmp(const char *s1, const char *s2);
long long	ft_atoll(const char *str);

// parsing.c
int			parse_args(int argc, char **argv, t_sim *sim);

// init.c
int			init_mutexes(t_sim *sim);
int	init_allocation(t_sim *sim);
int	init_coders(t_sim *sim);

// logging.c
void		print_status(t_sim *sim, int id, const char *status);

#endif