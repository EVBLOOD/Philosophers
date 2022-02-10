/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 19:45:49 by sakllam           #+#    #+#             */
/*   Updated: 2022/02/10 10:58:27 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdlib.h>
# include <unistd.h>

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
}	t_data;
typedef struct s_philo
{
	int				id;
	long			cleanup;
	long			st;
	int				nbtimeeating;
	int				alive;
	long			timeleft;
	long			timeprintf;
	long			t_n;
	long			lanched;
	pthread_t		idtread;
	pthread_mutex_t	printf;
	pthread_mutex_t	*w_ptr;
	pthread_mutex_t	died;
	pthread_mutex_t	*died_ptr;
	pthread_mutex_t	mutexright;
	pthread_mutex_t	*mutexleft;
	t_data			*bn;
}	t_philo;

t_data		*ft_parsing(int parnomber, char **parms);
long long	ft_atoiprime(const char *nptr);
void		*ft_philoroutine(void *args);
void		ft_putstr(char *str);
void		ft_putnbr(long nbr);
int			ft_checkdeath(t_philo *philos, t_data *st);
int			ft_philosteptwo(t_data *st, t_philo **philos);
int			ft_philostepone(t_data *st, t_philo **philos);
int			ft_getsted(t_data *st, t_philo **philos);
void		ft_freeanddestroy(t_philo *philos, t_data *st);
int			ft_creatingthreads(t_philo *philos, t_data *st);
int			ft_isitnull(char *msg, void *vartocheck);
long		ft_tmnw(void);
long		ft_tmnwprime(void);
int			ft_philolast(t_data *st, t_philo **philos);
int			ft_checkalive(t_philo *args);
int			ft_printf(char *str, pthread_mutex_t *ptr, int id, long time);
void		ft_usleep(long stop, long now);
void		ft_waitforbrothers(t_philo	*x);
#endif