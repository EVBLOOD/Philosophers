/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philo_utils_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 17:30:30 by sakllam           #+#    #+#             */
/*   Updated: 2022/02/13 15:26:33 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosophers.h"

int	ft_philosteptwo(t_data *st, t_philo **philos)
{
	int	i;

	i = -1;
	if (ft_philolast(st, philos))
		return (1);
	return (0);
}

int	ft_philostepone(t_data *st, t_philo **philos)
{
	int	i;

	if (ft_isitnull("error in allocating mem", *philos))
		return (1);
	i = -1;
	sem_unlink("fork");
	sem_unlink("print");
	sem_unlink("death");
	sem_unlink("e_t");
	sem_unlink("brothers");
	st->forks = sem_open("fork", O_CREAT, S_IRUSR | S_IWUSR, st->n_phi);
	st->print = sem_open("print", O_CREAT, S_IRUSR | S_IWUSR, 1);
	st->death = sem_open("death", O_CREAT, S_IRUSR | S_IWUSR, 1);
	st->e_t = sem_open("e_t", O_CREAT, S_IRUSR | S_IWUSR, 0);
	st->brothers = sem_open("brothers", O_CREAT, S_IRUSR | S_IWUSR, 0);
	if (st->forks == SEM_FAILED || st->print == SEM_FAILED
		|| st->death == SEM_FAILED || st->e_t == SEM_FAILED)
	{
		printf ("error in opening semaphore\n");
		return (1);
	}
	if (ft_philosteptwo(st, philos))
		return (1);
	return (0);
}

int	ft_isitnull(char *msg, void *vartocheck)
{
	if (!vartocheck)
	{
		printf("%s\n", msg);
		return (1);
	}
	return (0);
}

int	ft_philolast(t_data *st, t_philo **philos)
{
	int				i;

	i = -1;
	while (++i < st->n_phi)
	{
		(*philos)[i].id = i + 1;
		(*philos)[i].t_n = 0;
		(*philos)[i].nbtimeeating = 0;
		(*philos)[i].bn = st;
	}
	return (0);
}

long	ft_tmnw(void)
{
	struct timeval	x;

	gettimeofday(&x, NULL);
	return (x.tv_sec * 1000 + x.tv_usec / 1000);
}
