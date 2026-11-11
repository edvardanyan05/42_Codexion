/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmvarda <edmvarda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/27 22:31:36 by edmvarda          #+#    #+#             */
/*   Updated: 2026/11/05 19:58:09 by edmvarda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include "types.h"
# include "heap.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <limits.h>
# include <sys/time.h>
# include <unistd.h>

// utils.c
int			ft_strcmp(const char *s1, const char *s2);
long long	ft_atoll(const char *str);
int			print_error(const char *msg);

// time.c
long long	get_time_in_ms(void);
void		ft_usleep(long long duration_ms, t_sim *sim);
void		ms_to_timespec(long long ms, struct timespec *ts);

// parsing.c
int			parse_args(int argc, char **argv, t_sim *sim);

// init.c
int			init_simulation(t_sim *sim);

// init_helpers.c
int			init_index_sync(t_sim *sim, int i);
void		init_coders_data(t_sim *sim, int i);

// cleanup.c
void		cleanup_simulation(t_sim *sim);

// logging.c
void		print_status(t_sim *sim, int id, const char *status);

// dongle.c
int			take_dongle(t_coder *coder, t_dongle *dongle);
int			take_both_dongles(t_coder *coder);
void		free_dongle(t_sim *sim, t_dongle *dongle);
void		free_both_dongles(t_coder *coder);

// simulation.c
void		*coder_routine(void *arg);
int			is_simulation_ended(t_sim *sim);

// monitor.c
void		*monitor_routine(void *arg);
void		end_simulation(t_sim *sim);
void		wake_all_dongles(t_sim *sim);

#endif
