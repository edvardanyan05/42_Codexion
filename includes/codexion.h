/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmvarda <edmvarda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 22:31:36 by edmvarda          #+#    #+#             */
/*   Updated: 2026/10/27 23:02:05 by edmvarda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include "types.h"
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

int			ft_strcmp(const char *s1, const char *s2);
long long	ft_atoll(const char *str);

int			parse_args(int argc, char **argv, t_sim *sim);

#endif