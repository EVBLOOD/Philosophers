# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Philo_utils_1.x                                    :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/10 17:30:30 by sakllam           #+#    #+#              #
#    Updated: 2022/02/13 15:15:29 by sakllam          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

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
	st->e_t = sem_open("e_t", O_CREAT, S_IRUSR | S_IWUSR, 1);
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
			printf ("%ld %d is died\n", checker -  x->bn->st,  x->id);
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
	sem_wait(x->bn->brothers);
	x->bn->st = ft_tmnw();
	pthread_create(&(x->listener),
		NULL, &ft_watcher, (void *)x);
	x->t_n = ft_tmnw();
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
	free(x->bn);
	free(x);
	return (NULL);
}


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
			// if (ft_semclose(philos))
			// 	printf ("error while closing sem");
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
	if (sem_close(philos->bn->e_t)!= 0)
		return (1);
	if (sem_close(philos->bn->print)!= 0)
		return (1);
	if (sem_close(philos->bn->forks)!= 0)
		return (1);
	return (0);
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
		kill(st->helper, SIGKILL);
	// sem_post(philos->bn->e_t);
	i = -1;
	while (++i < st->n_phi)
		kill(philos[i].idpros, SIGKILL);
	if (ft_semclose(philos))
		printf ("error while closing sem");
	free(philos->bn);
	free(philos);
	return (0);
}