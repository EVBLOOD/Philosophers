/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 19:45:46 by sakllam           #+#    #+#             */
/*   Updated: 2022/02/09 16:33:50 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosophers.h"

int	ft_isitnull(char *msg, void *vartocheck)
{
	if (!vartocheck)
	{
		printf("%s\n", msg);
		return (1);
	}
	return (0);
}

long	ft_timenow()
{
	struct timeval	x;

	gettimeofday(&x, NULL);
	return (x.tv_sec * 1000 + x.tv_usec / 1000);
}

long	ft_timenowprime()
{
	struct timeval	x;

	gettimeofday(&x, NULL);
	return (x.tv_sec * 1000000 + x.tv_usec);
}

int	ft_philolast(t_data *start, t_philo **philos)
{
	int				i;
	struct timeval	current_time;

	i = -1;
	while (++i < start->numberofphilos)
	{
		(*philos)[i].philoid = i + 1;
		(*philos)[i].timeleft = 0;
		(*philos)[i].time_new = 0;
		(*philos)[i].lanched = 0;
		(*philos)[i].alive = 1;
		(*philos)[i].nbtimeeating = 0;
		(*philos)[i].start = ft_timenow();
		if ((*philos)[i].start < 0)
		{
			free(*philos);
			return (1);
		}
		(*philos)[i].about = start;
	}
	return (0);
}

int	ft_checkalive(t_philo *args)
{
	args->timeleft +=  args->timeleft - ft_timenow();
	if (args->timeleft > args->about->timetodie)
		return (1);
	return (0);
}

int	ft_philosteptwo(t_data *start, t_philo **philos)
{
	int	i;
	
	if (pthread_mutex_init(&(*philos)[0].died, NULL) || pthread_mutex_init(&(*philos)[0].printf, NULL))
	{
		printf ("error init mutex\n");
		return (1);
	}
	i = -1;
	while (++i < start->numberofphilos)
	{
		(*philos)[i].died_ptr = &(*philos)[0].died;
		(*philos)[i].printf_ptr = &(*philos)[0].printf;
	}
	if (ft_philolast(start, philos))
		return (1);
	return (0);
}

int	ft_philostepone(t_data *start, t_philo **philos)
{
	int				i;
	
	if (ft_isitnull("error in mem allocation", *philos))
		return (1);
	i = -1;
	while (++i < start->numberofphilos)
		if (pthread_mutex_init(&(*philos)[i].mutexright, NULL))
		{
			printf ("error init mutex\n");
			return (1);
		}
	i = -1;
	while (++i < start->numberofphilos)
		(*philos)[i].mutexleft = &((*philos)[((i + 1) % start->numberofphilos)].mutexright);
	if (ft_philosteptwo(start, philos))
		return (1);
	return (0);
}

int	ft_getstarted(t_data *start, t_philo **philos)
{
	struct timeval	current_time;
	int				i;

	*philos = malloc(sizeof(t_philo) * start->numberofphilos);
	if (ft_philostepone(start, philos))
		return (1);
	return (0);
}

int	ft_creatingthreads(t_philo *philos, t_data *start)
{
	int	i;
	
	i = -1;
	while (++i < start->numberofphilos)
	{
		if (pthread_create(&(philos[i].idtread), NULL, &ft_philoroutine, (void *)&philos[i]))
		{
			printf ("error creating thread\n");
			return (1);
		}
	}
	return (0);
}

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

// void	ft_usleep(long stop, long now)
// {
// 	long	end;

// 	end = now + stop;
// 	while (!(ft_timenow() >= end / 1000));
// }


void	ft_usleep(long stop, long now)
{
	long	end;

	end = now + stop;
	while (!(ft_timenow() >= end / 1000))
	{
		usleep(1000);
	}
	
}

void	*ft_philoroutine(void *args)
{
	t_philo	*x;

	x = args;
	x->time_new = 0;
	pthread_mutex_lock(x->printf_ptr);
	x->about->hsab++;
	pthread_mutex_unlock(x->printf_ptr);
	while (1)
	{
		if (x->about->hsab == x->about->numberofphilos)
		{
			pthread_mutex_lock(x->printf_ptr);
			printf("start => %ld\n", x->about->hsab);
			pthread_mutex_unlock(x->printf_ptr);
			break;
		}
	}
	x->cleanup = 0;
	x->about->start = ft_timenow();
	x->time_new = x->about->start;
	if (!(x->philoid % 2))
		ft_usleep(15000, ft_timenowprime());
	while (1)
	{
		pthread_mutex_lock(&(x->mutexright));
		ft_printf(" has taken a fork\n", x->printf_ptr, x->philoid, ft_timenow() - x->about->start);
		pthread_mutex_lock(x->mutexleft);
		ft_printf(" has taken a fork\n", x->printf_ptr, x->philoid, ft_timenow() - x->about->start);
		x->time_new = ft_timenow();
		ft_printf(" is eating\n", x->printf_ptr, x->philoid, ft_timenow() - x->about->start);
		// usleep(x->about->timetoeat);
		ft_usleep(x->about->timetoeat,ft_timenowprime());
		pthread_mutex_unlock(x->mutexleft);
		pthread_mutex_unlock(&(x->mutexright));
		ft_printf(" is sleeping\n", x->printf_ptr, x->philoid, ft_timenow() - x->about->start);
		// usleep (x->about->timetosleep);
		ft_usleep(x->about->timetosleep, ft_timenowprime());
		ft_printf(" is thinking\n", x->printf_ptr, x->philoid, ft_timenow() - x->about->start);
	}
	return (NULL);
}


// void	*ft_philoroutine(void *args)
// {
// 	t_philo	*x;

// 	x = args;
// 	pthread_mutex_lock(x->printf_ptr);
// 	x->about->hsab++;
// 	pthread_mutex_unlock(x->printf_ptr);
// 	while (x->about->hsab != x->about->numberofphilos);
// 	x->time_new = 0;
// 	x->timeprintf = 0;
// 	x->lanched = 0;
// 	x->timeleft = 0;
// 	if (!(x->philoid % 2))
// 	{
// 		// x->timeprintf = (x->about->timetodie * 1000) / 2;
// 		x->timeleft = x->about->timetodie / 2;
// 		x->lanched = x->about->timetodie / 2;
// 		usleep((x->about->timetodie * 1000) / 2);
// 	}
// 	while (1)
// 	{
// 		x->time_new += ft_timenow() + x->about->timetodie - x->timeleft;
// 		if (!(x->philoid % 2))
// 		{
// 			// usleep(x->about->timetoeat);
// 			x->timeprintf += x->about->timetoeat / 1000;
// 		}
// 		pthread_mutex_lock(&(x->mutexright));
// 		pthread_mutex_lock(x->printf_ptr);
// 		printf ("%ld %d has taken a fork\n", x->timeprintf, x->philoid);
// 		pthread_mutex_unlock(x->printf_ptr);
// 		pthread_mutex_lock(x->mutexleft);
// 		pthread_mutex_lock(x->printf_ptr);
// 		printf ("%ld %d has taken a fork\n", x->timeprintf, x->philoid);
// 		pthread_mutex_unlock(x->printf_ptr);
// 		pthread_mutex_lock(x->printf_ptr);
// 		x->time_new = 0;
// 		printf ("%ld %d is eating\n", x->timeprintf, x->philoid);
// 		pthread_mutex_unlock(x->printf_ptr);
// 		x->timeleft = 0;
// 		if (!(x->philoid % 2))
// 			x->timeleft = x->about->timetoeat / 1000 + x->lanched;
// 		if (x->about->timetoeat / 1000 + x->timeleft > x->about->timetodie)
// 		{
// 			x->timeprintf += x->about->timetodie + 1;
// 			usleep((x->about->timetoeat  + (x->timeleft * 1000)) - (x->about->timetodie * 1000));
// 			pthread_mutex_lock(x->died_ptr);
// 			x->alive = 0;
// 			return (NULL);
// 		}
// 		else
// 		{
// 			x->timeprintf += x->about->timetoeat / 1000;
// 			usleep(x->about->timetoeat);
// 			if (++x->nbtimeeating == x->about->nbtimeat)
// 				x->about->counter++;
// 		}
// 		// x->time_new = ft_timenow() + x->about->timetodie * 1000;
// 		// usleep(x->about->timetoeat);
// 		pthread_mutex_unlock(x->mutexleft);
// 		pthread_mutex_unlock(&(x->mutexright));
// 		pthread_mutex_lock(x->printf_ptr);
// 		printf ("%ld %d is sleeping\n", x->timeprintf, x->philoid);
// 		if (x->about->timetoeat + x->about->timetosleep + x->lanched > x->about->timetodie * 1000)
// 		{
// 			usleep((x->about->timetoeat + x->about->timetosleep + x->lanched * 1000) - x->about->timetodie * 1000);
// 			x->timeprintf += ((x->about->timetoeat + x->about->timetosleep) / 1000 + x->lanched) - x->about->timetodie + 1;
// 			pthread_mutex_lock(x->died_ptr);
// 			x->alive = 0;
// 			return (NULL);
// 		}
// 		else
// 		{
// 			usleep(x->about->timetosleep);
// 			x->timeprintf += x->about->timetosleep / 1000;
// 		}
// 		pthread_mutex_unlock(x->printf_ptr);
// 		// usleep (x->about->timetosleep);
// 		x->timeleft = 0;
// 		// x->lanched = 0;
// 		pthread_mutex_lock(x->printf_ptr);
// 		printf ("%ld %d is thinking\n", x->timeprintf, x->philoid);
// 		pthread_mutex_unlock(x->printf_ptr);
// 	}
// 	return (NULL);
// }



int	main(int argc, char **argv)
{
	t_data			*start;
	t_philo			*philos;
	int				i;

	if (argc >= 5)
	{
		start = ft_parsing(argc, argv);
		if (!start)
		{
			printf ("Something in your inputs is wrong!\n");
			return (1);
		}
		if (ft_getstarted(start, &philos))
		{
			free (start);
			return (1);
		}
		
		if (ft_creatingthreads(philos, start))
			return (1);
		i = -1;
		while (++i < start->numberofphilos)
			pthread_detach(philos[i].idtread);
		// int	test;

		// test = 1;
		while (1)
		{
			i = -1;
			// if (start->numberofphilos == start->hsab && test)
			// {
				
			// }
			while (++i < start->numberofphilos && philos[i].about->hsab == philos[i].about->numberofphilos && philos[i].time_new)
			{
				// if (philos[i].alive)
				// {
					start->time_corr = ft_timenow();
					pthread_mutex_lock(philos[i].died_ptr);
					if (start->time_corr > philos[i].time_new + philos[i].about->timetodie + philos[i].cleanup && philos[i].time_new)
					{
						pthread_mutex_lock(philos[i].printf_ptr);
						printf ("%ld %d is died\n", start->time_corr - philos[i].about->start, philos[i].philoid);
						return (0); // daba
					}
					pthread_mutex_unlock(philos[i].died_ptr);
				// }
				// else
				// {
				// 	pthread_mutex_lock(philos[i].printf_ptr);
				// 	printf ("%ld %d is died\n", ft_timenow() - philos[i].start, philos[i].philoid);
				// 	return (0); // daba
				// }
				// daba
			}
		}
	}
	else
	{
		printf("Sorry but the number of parameters is not enough!\n");
	}
}



		

// void	*ft_philoroutinealsoold(void *args)
// {
// 	t_philo	*x;
	
// 	x = args;
// 	// pthread_mutex_lock(x->printf_ptr);
// 	pthread_mutex_lock(x->died_ptr);
// 	x->about->hsab++;
// 	printf("%d \n", x->about->hsab); 
// 	pthread_mutex_unlock(x->died_ptr);
// 	// pthread_mutex_unlock(x->printf_ptr);
// 	while (x->about->hsab != x->about->numberofphilos);
// 	if (x->philoid % 2 == 0)
// 		usleep((x->about->timetodie * 1000) / 2);
// 	x->time_new = ft_timenow();
// 	x->timeprintf = 0;
// 	while (1)
// 	{
// 		pthread_mutex_lock(&(x->mutexright));
// 	x->timeleft = ft_timenowprime(); //1 added
// 		pthread_mutex_lock(x->printf_ptr);
// 		printf ("%ld %d has taken a fork\n", ft_timenow() - x->start, x->philoid);
// 		pthread_mutex_unlock(x->printf_ptr);
// 	x->timeprintf += ft_timenowprime() - x->timeleft; //2
// 		pthread_mutex_lock(x->mutexleft);
// 	x->timeleft = ft_timenowprime(); //1
// 		pthread_mutex_lock(x->printf_ptr);
// 		printf ("%ld %d has taken a fork\n", ft_timenow() - x->start, x->philoid);
// 		pthread_mutex_unlock(x->printf_ptr);
// 	x->timeprintf += ft_timenowprime() - x->timeleft; //2
// 	x->timeleft = ft_timenowprime(); //1
// 		pthread_mutex_lock(x->printf_ptr);
// 		printf ("%ld %d is eating\n", ft_timenow() - x->start, x->philoid);
// 		pthread_mutex_unlock(x->printf_ptr);
// 	x->timeprintf = ft_timenowprime() - x->timeleft; //2
// 		if (x->about->timetoeat <= x->about->timetodie * 1000)
// 		{
// 			usleep(x->about->timetoeat);
// 			if (++x->nbtimeeating == x->about->nbtimeat)
// 				x->about->counter++;
// 		}
// 		else
// 		{
// 			usleep(x->about->timetodie * 1000 + 1000);
// 			return (NULL);
// 		}
// 		usleep (x->about->timetoeat);
// 		pthread_mutex_unlock(x->mutexleft);
// 		pthread_mutex_unlock(&(x->mutexright));
// 		x->time_new = ft_timenow();
// 	x->timeleft = ft_timenowprime(); //1
// 		pthread_mutex_lock(x->printf_ptr);
// 		printf ("%ld %d is sleeping\n", ft_timenow() - x->start, x->philoid);
// 		pthread_mutex_unlock(x->printf_ptr);
// 	x->timeprintf += ft_timenowprime() - x->timeleft; //2
		
// 		if (x->about->timetoeat + x->about->timetosleep > x->about->timetodie * 1000)
// 		// {
// 			usleep(x->about->timetodie * 1000 - x->about->timetosleep + 1000);
// 			// return (NULL);
// 		// }
// 		else
// 			usleep (x->about->timetosleep);
// 		// usleep (x->about->timetosleep);
// 	x->timeleft = ft_timenowprime(); //1
// 		pthread_mutex_lock(x->printf_ptr);
// 		printf ("%ld %d is thinking\n", ft_timenow() - x->start, x->philoid);
// 		pthread_mutex_unlock(x->printf_ptr);
// 	x->timeprintf += ft_timenowprime() - x->timeleft; //2
// 	}
// 	return (NULL);
// }



// int	ft_philoeat(t_philo *args)
// {
// 	pthread_mutex_lock(args->printf_ptr);
// 	printf ("%ld %d is eating\n", ft_timenow() - args->start), args->philoid);
// 	pthread_mutex_unlock(args->printf_ptr);



// 	if (args->about->timetodie > args->timeleft)
// 		args->timeleft = args->about->timetodie;
// 	else
// 	{
// 		args->timeleft = args->about->timetoeat;
// 		args->nbtimeeating++;
// 	}
// 	usleep(args->timeleft);
// 	pthread_mutex_lock(args->died_ptr);
// 	if (args->about->nbtimeat == args->nbtimeeating || args->about->timetodie < args->timeleft)
// 		return (1);
// 	pthread_mutex_unlock(args->died_ptr);
// 	return (0);
// }

// int	ft_philosleep(t_philo *args)
// {
// 	pthread_mutex_lock(args->printf_ptr);
// 	printf ("%ld %d is sleeping\n", ft_timetoprint(ft_timenow() - args->start), args->philoid);
// 	pthread_mutex_unlock(args->printf_ptr);


// 	args->timeleft += args->about->timetosleep;
// 		// args->timeleft = args->about->timetodie - args->timeleft;
// 	if (args->timeleft > args->about->timetodie)
// 		usleep(args->about->timetodie - args->timeleft);
// 	else
// 		usleep(args->about->timetosleep);
// 	pthread_mutex_lock(args->died_ptr);
// 	if (args->about->timetodie < args->timeleft)
// 		return (1);
// 	pthread_mutex_unlock(args->died_ptr);
// 	return (0);
// }

// int	ft_philothinks(t_philo *args)
// {
// 	pthread_mutex_lock(args->printf_ptr);
// 	printf ("%ld %d is thinking\n", ft_timetoprint(ft_timenow() - args->start), args->philoid);
// 	pthread_mutex_unlock(args->printf_ptr);



// 	return (0);
// }

// int	ft_philodied(t_philo *args)
// {
// 	// if (args->nbtimeeating == args->about->nbtimeat)
// 	// {
// 	// 	pthread_mutex_lock(args->printf_ptr);
// 	// 	return (1);
// 	// }
// 	pthread_mutex_lock(args->printf_ptr);
// 	printf ("%ld %d is died\n", ft_timetoprint(ft_timenow() - args->start), args->philoid);



// 	args->alive = 0;
// 	return (1);
// }

// int	ft_philotokefork(t_philo *args)
// {
// 	while (pthread_mutex_lock(&(args->mutexright)))
// 	{
// 		pthread_mutex_lock(args->died_ptr);
// 		if (ft_checkalive(args))
// 			return (1);
// 		pthread_mutex_unlock(args->died_ptr);
// 	}




// 	pthread_mutex_lock(args->printf_ptr);
// 	printf ("%ld %d has taken a fork\n", ft_timetoprint(ft_timenow() - args->start), args->philoid);
// 	pthread_mutex_unlock(args->printf_ptr);
	
	
	
	
// 	while (pthread_mutex_lock(args->mutexleft))
// 	{
// 		pthread_mutex_lock(args->died_ptr);
// 		if (ft_checkalive(args))
// 			return (1);
// 		pthread_mutex_unlock(args->died_ptr);
// 	}




// 	pthread_mutex_lock(args->printf_ptr);
// 	printf ("%ld %d has taken a fork\n", ft_timetoprint(ft_timenow() - args->start), args->philoid);
// 	pthread_mutex_unlock(args->printf_ptr);
	
	
	
	
	
// 	if (ft_philoeat(args))
// 		return (1);
// 	pthread_mutex_unlock(&(args->mutexright));
// 	pthread_mutex_unlock(args->mutexleft);
// 	return (0);
// }




// void	*ft_philoroutineold(void *args)
// {
// 	t_philo	*x;

// 	x = args;
// 	x->time_new = x->start;
// 	x->timeleft = 0;
// 	if (!(x->philoid % 2))
// 	{
// 		x->timeleft = x->about->timetodie / 2;
// 		usleep(x->about->timetodie / 2);
// 	}
// 	while (1)
// 	{
// 		// //start first fork;
// 		pthread_mutex_lock(&(x->mutexright));
// 		// {
// 		// 	x->timeleft += ft_timenow() - x->time_new;
// 		// 	pthread_mutex_lock(x->died_ptr);
// 		// 	if (x->timeleft > x->about->timetodie)
// 		// 	{
// 		// 		x->alive = 0;
// 		// 		return (NULL);
// 		// 	}
// 		// }
// 		// pthread_mutex_unlock(x->died_ptr);
// 		pthread_mutex_lock(x->printf_ptr);
// 		printf ("%ld %d has taken a fork\n", ft_timetoprint(ft_timenow() - x->start), x->philoid);
// 		pthread_mutex_unlock(x->printf_ptr);
// 		//start second fork;
// 		if (&((*x).mutexright) == x->mutexleft)
// 		{
// 			usleep(x->about->timetodie + 1000);
// 			pthread_mutex_lock(x->died_ptr);
// 			x->alive = 0;
// 			return (NULL);
// 		}
// 		while (pthread_mutex_lock(x->mutexleft))
// 		{
// 			pthread_mutex_lock(x->died_ptr);
// 			if (x->timeleft > x->about->timetodie)
// 			{
// 				x->alive = 0;
// 				return (NULL);
// 			}
// 			pthread_mutex_unlock(x->died_ptr);
// 		}
// 			x->timeleft += ft_timenow() - x->time_new;
// 		pthread_mutex_lock(x->printf_ptr);
// 		printf ("%ld %d has taken a fork\n", ft_timetoprint(ft_timenow() - x->start), x->philoid);
// 		pthread_mutex_unlock(x->printf_ptr);
// 		//start of eating
// 		x->time_new = ft_timenow();
// 		pthread_mutex_lock(x->printf_ptr);
// 		printf ("%ld %d is eating\n", ft_timetoprint(ft_timenow() - x->start), x->philoid);
// 		pthread_mutex_unlock(x->printf_ptr);
// 		x->timeleft = x->about->timetoeat;
// 		if (x->timeleft <= x->about->timetodie)
// 		{
// 			usleep(x->about->timetoeat);
// 			x->nbtimeeating++;
// 			if (x->nbtimeeating == x->about->nbtimeat)
// 				x->about->counter++;
// 			pthread_mutex_lock(x->died_ptr);
// 			if (x->about->counter == x->about->numberofphilos)
// 				return (NULL);
// 			pthread_mutex_unlock(x->died_ptr);
// 		}
// 		else
// 		{
// 			usleep(x->about->timetodie);
// 			pthread_mutex_lock(x->died_ptr);
// 			x->alive = 0;
// 			return (NULL);
// 		}
// 		//end of eating
// 		pthread_mutex_unlock(x->mutexleft);
// 		pthread_mutex_unlock(&(x->mutexright));
// 		//end of forks and eating
// 		//start of sleeping
// 		pthread_mutex_lock(x->printf_ptr);
// 		printf ("%ld %d is sleeping\n", ft_timetoprint(ft_timenow() - x->start), x->philoid);
// 		pthread_mutex_unlock(x->printf_ptr);
// 		x->timeleft += x->about->timetosleep;
// 		if (x->timeleft > x->about->timetodie)
// 		{
// 			usleep(x->about->timetodie - x->about->timetosleep + 1000);
// 			x->alive = 0;
// 			pthread_mutex_lock(x->died_ptr);
// 			return (NULL);
// 		}
// 		else
// 			usleep (x->about->timetosleep);
// 		// pthread_mutex_unlock(x->died_ptr);
// 		// if (x->timeleft > x->about->timetodie)
// 		// {
// 		// 	usleep (x->timeleft - x->about->timetodie);
// 		// 	pthread_mutex_lock(x->died_ptr);
// 		// 	x->alive = 0;
// 		// 	return (NULL);
// 		// }
// 		//end of sleeping
// 		//start of thinking
// 		pthread_mutex_lock(x->printf_ptr);
// 		printf ("%ld %d is thinking\n", ft_timetoprint(ft_timenow() - x->start), x->philoid);
// 		pthread_mutex_unlock(x->printf_ptr);
// 		//end of of thinking
// 	}
// 	return (NULL);
// }
// int	ft_creatingthreads(t_philo **philos, t_data *start)
// {
// 	int	i;
	
// 	i = -1;
	

// 	i = -1;
// 	while (++i < start->numberofphilos)
// 		if (pthread_create(&((*philos)->idtread), NULL, &ft_philoroutine, (void *)*philos))
// 		{
// 			printf ("error creating thread\n");
// 			return (1);
// 		}
// 	return (0);
// }

// int	ft_checkforlive(t_philo **philos, t_data *start)
// {
// 	int	i;

// 	while (1)
// 	{
// 		i = -1;
// 		while (++i < start->numberofphilos)
// 		{
// 			pthread_mutex_lock((*philos[i]).died_ptr);
// 			if ((*philos[i]).about->counter == (*philos[i]).about->numberofphilos)
// 				return (0); // daba
// 			(*philos[i]).timeleft = ft_timenow() - (*philos[i]).time_new;
// 			if ((*philos[i]).timeleft > (*philos[i]).about->timetodie + 999)
// 			{
// 				pthread_mutex_lock((*philos[i]).printf_ptr);
// 				printf ("%ld %d is died\n", ft_timetoprint(ft_timenow() - (*philos[i]).start), (*philos[i]).philoid);
// 				return (0); // daba
// 			}
// 			pthread_mutex_unlock((*philos[i]).died_ptr);
// 		}
// 	}
// }
		// while (1)
		// {
		// 	i = -1;
		// 	while (++i < start->numberofphilos)
		// 	{
		// 		if (!philos[i].alive)
		// 		{
		// 			pthread_mutex_lock(philos[i].printf_ptr);
		// 			printf ("%ld %d is died\n", ft_timetoprint(ft_timenow() - philos[i].start), philos[i].philoid);
		// 			i = -1;
		// 			while (++i < start->numberofphilos)
		// 			{
		// 				pthread_detach(philos[i].idtread);
		// 				pthread_mutex_destroy(&(philos[i].mutexright));
		// 			}
		// 			pthread_mutex_destroy(philos[0].printf_ptr);
		// 			pthread_mutex_destroy(philos[0].died_ptr);
		// 			return (0);
		// 		}
		// 		if (start->nbtimeat > 0 && philos[i].about->counter == start->numberofphilos)
		// 		{
		// 			i = -1;
		// 			while (++i < start->numberofphilos)
		// 			{
		// 				pthread_detach(philos[i].idtread);
		// 				pthread_mutex_destroy(&(philos[i].mutexright));
		// 			}
		// 			pthread_mutex_destroy(philos[0].printf_ptr);
		// 			pthread_mutex_destroy(philos[0].died_ptr);
		// 			return (0);
		// 		}
				
		// 	}

		// }
		// if (ft_creatingthreads(&philos, start))
		// {
		// 	free (start);
		// 	free(philos);
		// 	return (1);
		// }

		// i = -1;
		// while (++i < start->numberofphilos)
		// 	if (pthread_create(&philos[i].idtread, NULL, &ft_philoroutine, (void *)&philos[i]))
		// 	{
		// 		printf ("error creating thread\n");
		// 		return (1);
		// 	}