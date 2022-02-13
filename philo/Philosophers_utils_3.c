/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philosophers_utils_3.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 10:35:14 by sakllam           #+#    #+#             */
/*   Updated: 2022/02/13 15:48:21 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosophers.h"

int	ft_printf(char *str, pthread_mutex_t *ptr, int id, long time)
{
	if (pthread_mutex_lock(ptr))
		return (1);
	ft_putnbr(time);
	ft_putstr("	");
	ft_putnbr(id);
	ft_putstr(str);
	if (pthread_mutex_unlock(ptr))
		return (1);
	return (0);
}

void	ft_usleep(long stop, long now)
{
	long	end;

	end = now + stop;
	while (!(ft_tmnw() >= end / 1000))
	{
		usleep(50);
	}
}

void	ft_waitforbrothers(t_philo	*x)
{
	x->t_n = 0;
	x->bn->counter = 0;
	pthread_mutex_lock(x->w_ptr);
	x->bn->hsab++;
	pthread_mutex_unlock(x->w_ptr);
	while (1)
		if (x->bn->hsab == x->bn->numberofphilos)
			break ;
	x->bn->st = ft_tmnw();
	x->t_n = x->bn->st;
}

void	*ft_philoroutine(void *args)
{
	t_philo	*x;

	x = args;
	ft_waitforbrothers(x);
	if (!(x->id % 2))
		ft_usleep(15000, ft_tmnwprime());
	while (1)
	{
		pthread_mutex_lock(&(x->mutexright));
		ft_printf(" has taken a fork\n", x->w_ptr, x->id, ft_tmnw() - x->bn->st);
		pthread_mutex_lock(x->mutexleft);
		ft_printf(" has taken a fork\n", x->w_ptr, x->id, ft_tmnw() - x->bn->st);
		x->t_n = ft_tmnw();
		ft_printf(" is eating\n", x->w_ptr, x->id, ft_tmnw() - x->bn->st);
		ft_usleep(x->bn->timetoeat, ft_tmnwprime());
		if (++x->nbtimeeating == x->bn->nbtimeat)
				x->bn->counter++;
		pthread_mutex_unlock(x->mutexleft);
		pthread_mutex_unlock(&(x->mutexright));
		ft_printf(" is sleeping\n", x->w_ptr, x->id, ft_tmnw() - x->bn->st);
		ft_usleep(x->bn->timetosleep, ft_tmnwprime());
		ft_printf(" is thinking\n", x->w_ptr, x->id, ft_tmnw() - x->bn->st);
	}
	return (NULL);
}

int	ft_checkdeath(t_philo *x, t_data *st)
{
	int	i;

	while (1)
	{
		i = -1;
		while (++i < st->numberofphilos && x[i].bn->hsab
			== x[i].bn->numberofphilos && x[i].t_n)
		{
			if (x[i].bn->counter == st->numberofphilos)
			{
				pthread_mutex_lock(x[i].w_ptr);
				return (1);
			}
			st->t_c = ft_tmnw();
			pthread_mutex_lock(x[i].died_ptr);
			if (st->t_c > x[i].t_n + x[i].bn->timetodie)
			{
				pthread_mutex_lock(x[i].w_ptr);
				printf ("%ld %d is died\n", st->t_c - x[i].bn->st, x[i].id);
				return (0);
			}
			pthread_mutex_unlock(x[i].died_ptr);
		}
	}
	return (1);
}
