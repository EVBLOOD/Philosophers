/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 19:45:49 by sakllam           #+#    #+#             */
/*   Updated: 2022/02/09 11:08:30 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	PHILOSOPHERS_H
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
	long	start;
	long		hsab;
	long	time_corr;
}	t_data;
typedef struct s_philo
{
	int				philoid;
	long			cleanup;
	long			start;
	int				nbtimeeating;
	int				alive;
	long			timeleft;
	long			timeprintf;
	long			time_new;
	long			lanched;
	pthread_t		idtread;
	pthread_mutex_t	printf;
	pthread_mutex_t	*printf_ptr;
	pthread_mutex_t	died;
	pthread_mutex_t	*died_ptr;
	pthread_mutex_t	mutexright;
	pthread_mutex_t	*mutexleft;
	t_data			*about;
}	t_philo;
t_data		*ft_parsing(int parnomber, char **parms);
long long	ft_atoiprime(const char *nptr);
void		*ft_philoroutine(void *args);
void		ft_putstr(char *str);
void		ft_putnbr(long nbr);
#endif