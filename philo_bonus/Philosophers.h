/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 19:45:49 by sakllam           #+#    #+#             */
/*   Updated: 2022/02/13 15:32:13 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <stdio.h>
# include <pthread.h>
# include <sys/wait.h>
# include <semaphore.h>
# include <sys/time.h>
# include <sys/types.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>

typedef struct s_data
{
	int		n_phi;
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
	pid_t	helper;
}	t_data;
typedef struct s_philo
{
	int				id;
	pid_t			idpros;
	int				nbtimeeating;
	long			t_n;
	pthread_t		listener;
	t_data			*bn;
}	t_philo;

int			ft_philosteptwo(t_data *st, t_philo **philos);
int			ft_philostepone(t_data *st, t_philo **philos);
int			ft_isitnull(char *msg, void *vartocheck);
int			ft_philolast(t_data *st, t_philo **philos);
long		ft_tmnw(void);
void		ft_usleep(long stop, long now);
int			ft_getsted(t_data *st, t_philo **philos);
int			ft_printf(char *str, t_philo *x, int id, long time);
void		*ft_watcher(void *args);
void		*ft_philoroutine(void *args);
long		ft_tmnwprime(void);
void		*ft_checkeatingtimes(void *x);
int			ft_semclose(t_philo *philos);
int			ft_lifecycle(t_philo *philos, t_data *st);
int			allnumbers(char	*data);
t_data		*ft_parsing(int parnomber, char **parms);
int			ft_atoi(const char *nptr);
long long	ft_atoiprime(const char *nptr);
void		ft_putstr(char *str);
void		ft_putnbr(long nbr);
void		ft_waitforbrothers(t_philo *x);
#endif