/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 19:45:49 by sakllam           #+#    #+#             */
/*   Updated: 2022/02/12 22:41:20 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <stdio.h>
# include <pthread.h>
# include <semaphore.h>
# include <sys/time.h>
#include <sys/types.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>

typedef struct s_data
{
	int		numberofphilos;
	long	timetodie;
	long	timetoeat;
	long	timetosleep;
	long	nbtimeat;
	int		counter;
	long	st;
	long	hsab;
	long	t_c;
	sem_t	*forks;
	sem_t	*print;
	sem_t	*death;
	sem_t	*e_t;
	sem_t	*brothers;
}	t_data;
typedef struct s_philo
{
	int				id;
	pid_t			idpros;
	int				nbtimeeating;
	long			t_n;
	pthread_t	listener;
	t_data			*bn;
}	t_philo;
int	ft_atoi(const char *nptr);
long long	ft_atoiprime(const char *nptr);
void	ft_putstr(char *str);
void	ft_putnbr(long nbr);
t_data	*ft_parsing(int parnomber, char **parms);
int	ft_philolast(t_data *st, t_philo **philos);
int	ft_isitnull(char *msg, void *vartocheck);
long	ft_tmnw(void);
void	ft_usleep(long stop, long now);
int	ft_getsted(t_data *st, t_philo **philos);
void	*ft_philoroutine(void *args);
int	ft_lifecycle(t_philo *philos, t_data *st);
long	ft_tmnwprime(void);
int	ft_printf(char *str, t_philo *x, int id, long time);
#endif