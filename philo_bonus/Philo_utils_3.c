/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philo_utils_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 17:30:30 by sakllam           #+#    #+#             */
/*   Updated: 2022/02/13 15:31:47 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosophers.h"

long	ft_tmnwprime(void)
{
	struct timeval	x;

	gettimeofday(&x, NULL);
	return (x.tv_sec * 1000000 + x.tv_usec);
}

void	*ft_checkeatingtimes(void *x)
{
	int		counter;
	int		i;
	t_philo	*philos;

	philos = x;
	counter = 0;
	i = -1;
	while (1)
	{
		sem_wait(philos->bn->e_t);
		counter++;
		if (counter == philos->bn->n_phi)
		{
			sem_wait(philos->bn->print);
			exit (1);
		}
	}
	return (NULL);
}

int	ft_semclose(t_philo *philos)
{
	sem_unlink("fork");
	sem_unlink("print");
	sem_unlink("death");
	sem_unlink("e_t");
	sem_unlink("brothers");
	if (sem_close(philos->bn->brothers) != 0)
		return (1);
	if (sem_close(philos->bn->death) != 0)
		return (1);
	if (sem_close(philos->bn->e_t) != 0)
		return (1);
	if (sem_close(philos->bn->print) != 0)
		return (1);
	if (sem_close(philos->bn->forks) != 0)
		return (1);
	return (0);
}

void	ft_end(t_philo *philos, t_data *st)
{
	int	i;

	i = -1;
	if (philos->bn->nbtimeat != -1)
		kill(st->helper, SIGKILL);
	while (++i < st->n_phi)
		kill(philos[i].idpros, SIGKILL);
	if (ft_semclose(philos))
		printf ("error while closing sem");
	free(philos->bn);
	free(philos);
}

int	ft_lifecycle(t_philo *philos, t_data *st)
{
	int	i;

	i = -1;
	while (++i < st->n_phi)
	{		
		philos[i].idpros = fork();
		if (philos[i].idpros == 0)
			ft_philoroutine(&philos[i]);
	}
	i = -1;
	while (++i < st->n_phi)
		sem_post(philos->bn->brothers);
	if (philos->bn->nbtimeat != -1)
	{
		st->helper = fork();
		if (st->helper == 0)
			ft_checkeatingtimes(philos);
	}
	waitpid(-1, NULL, 0);
	ft_end(philos, st);
	return (0);
}
