/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philosophers_utils_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 10:35:40 by sakllam           #+#    #+#             */
/*   Updated: 2022/02/13 15:48:20 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosophers.h"

int	ft_philosteptwo(t_data *st, t_philo **philos)
{
	int	i;

	if (pthread_mutex_init(&(*philos)[0].died, NULL)
		|| pthread_mutex_init(&(*philos)[0].printf, NULL))
	{
		printf ("error init mutex\n");
		return (1);
	}
	i = -1;
	while (++i < st->numberofphilos)
	{
		(*philos)[i].died_ptr = &(*philos)[0].died;
		(*philos)[i].w_ptr = &(*philos)[0].printf;
	}
	if (ft_philolast(st, philos))
		return (1);
	return (0);
}

int	ft_philostepone(t_data *st, t_philo **philos)
{
	int	i;

	if (ft_isitnull("error in mem allocation", *philos))
		return (1);
	i = -1;
	while (++i < st->numberofphilos)
	{
		if (pthread_mutex_init(&(*philos)[i].mutexright, NULL))
		{
			printf ("error init mutex\n");
			return (1);
		}
	}
	i = -1;
	while (++i < st->numberofphilos)
		(*philos)[i].mutexleft
			= &((*philos)[((i + 1) % st->numberofphilos)].mutexright);
	if (ft_philosteptwo(st, philos))
		return (1);
	return (0);
}

int	ft_getsted(t_data *st, t_philo **philos)
{
	*philos = malloc(sizeof(t_philo) * st->numberofphilos);
	if (ft_philostepone(st, philos))
		return (1);
	return (0);
}

void	ft_freeanddestroy(t_philo *philos, t_data *st)
{
	int	i;

	i = -1;
	while (++i < st->numberofphilos)
		pthread_mutex_destroy(philos[i].mutexleft);
	pthread_mutex_destroy(philos->w_ptr);
	pthread_mutex_destroy(philos->died_ptr);
	free(philos);
	free(st);
}

int	ft_creatingthreads(t_philo *philos, t_data *st)
{
	int	i;

	i = -1;
	while (++i < st->numberofphilos)
	{
		if (pthread_create(&(philos[i].idpros),
				NULL, &ft_philoroutine, (void *)&philos[i]))
		{
			printf ("error creating thread\n");
			return (1);
		}
	}
	i = -1;
	while (++i < st->numberofphilos)
		pthread_detach(philos[i].idpros);
	ft_checkdeath(philos, st);
	ft_freeanddestroy(philos, st);
	return (0);
}
