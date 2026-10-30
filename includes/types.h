/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edmvarda <edmvarda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/26 17:50:03 by edmvarda          #+#    #+#             */
/*   Updated: 2026/10/28 21:13:07 by edmvarda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

# include <pthread.h>

typedef struct s_coder	t_coder;

typedef struct s_dongle
{
	int				dongle_id;
	int				owner_id;
	long long		cooldown_until;
	pthread_mutex_t	lock;
	pthread_cond_t	cond;
}					t_dongle;

typedef struct s_sim
{
	int				number_of_coders;
	long long		time_to_burnout;
	long long		time_to_compile;
	long long		time_to_debug;
	long long		time_to_refactor;
	int				number_of_compiles_required;
	long long		dongle_cooldown;
	char			*scheduler;

	long long		start_time;
	int				simulation_ended;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	sim_lock;

	t_coder			*coders;
	t_dongle		*dongles;
}					t_sim;

typedef struct s_coder
{
	int				coder_id;
	int				compile_count;
	int				is_dead;
	long long		last_compile_time;
	pthread_t		thread;
	pthread_mutex_t	coder_lock;
	t_dongle		*left;
	t_dongle		*right;
	t_sim			*sim;

}					t_coder;

#endif
