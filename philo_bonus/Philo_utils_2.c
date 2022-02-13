/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philo_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 17:30:30 by sakllam           #+#    #+#             */
/*   Updated: 2022/02/13 15:30:43 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosophers.h"

void	ft_usleep(long stop, long now)
{
	long	end;

	end = now + stop;
	while (!(ft_tmnw() >= end / 1000))
	{
		usleep(50);
	}
}

int	ft_getsted(t_data *st, t_philo **philos)
{
	*philos = malloc(sizeof(t_philo) * st->n_phi);
	if (ft_philostepone(st, philos))
		return (1);
	return (0);
}

int	ft_printf(char *str, t_philo *x, int id, long time)
{
	if (sem_wait(x->bn->print))
		return (1);
	ft_putnbr(time);
	ft_putstr("	");
	ft_putnbr(id);
	ft_putstr(str);
	if (sem_post(x->bn->print))
		return (1);
	return (0);
}

void	*ft_watcher(void *args)
{
	t_philo	*x;
	long	checker;
	int		i;
	int		tmp;

	x = args;
	while (1)
	{
		checker = ft_tmnw();
		sem_wait(x->bn->death);
		if (checker > x->t_n + x->bn->timetodie)
		{
			sem_wait(x->bn->print);
			printf ("%ld %d	is died\n", checker - x->bn->st, x->id);
			i = x->id;
			tmp = i;
			exit (0);
		}
		sem_post(x->bn->death);
	}
	return (NULL);
}

void	*ft_philoroutine(void *args)
{
	t_philo	*x;

	x = args;
	ft_waitforbrothers(x);
	if (!(x->id % 2))
		ft_usleep(15000, ft_tmnw() * 1000);
	while (1)
	{
		sem_wait(x->bn->forks);
		ft_printf(" has taken a fork\n", x, x->id, ft_tmnw() - x->bn->st);
		sem_wait(x->bn->forks);
		ft_printf(" has taken a fork\n", x, x->id, ft_tmnw() - x->bn->st);
		x->t_n = ft_tmnw();
		ft_printf(" is eating\n", x, x->id, ft_tmnw() - x->bn->st);
		ft_usleep(x->bn->timetoeat, ft_tmnw() * 1000);
		sem_post(x->bn->forks);
		sem_post(x->bn->forks);
		if (++x->nbtimeeating == x->bn->nbtimeat)
			sem_post(x->bn->e_t);
		ft_printf(" is sleeping\n", x, x->id, ft_tmnw() - x->bn->st);
		ft_usleep(x->bn->timetosleep, ft_tmnw() * 1000);
		ft_printf(" is thinking\n", x, x->id, ft_tmnw() - x->bn->st);
	}
	return (NULL);
}
