/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philosophers_utils_1.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 10:35:17 by sakllam           #+#    #+#             */
/*   Updated: 2022/02/10 11:00:14 by sakllam          ###   ########.fr       */
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

long	ft_tmnw(void)
{
	struct timeval	x;

	gettimeofday(&x, NULL);
	return (x.tv_sec * 1000 + x.tv_usec / 1000);
}

long	ft_tmnwprime(void)
{
	struct timeval	x;

	gettimeofday(&x, NULL);
	return (x.tv_sec * 1000000 + x.tv_usec);
}

int	ft_philolast(t_data *st, t_philo **philos)
{
	int				i;

	i = -1;
	while (++i < st->numberofphilos)
	{
		(*philos)[i].id = i + 1;
		(*philos)[i].timeleft = 0;
		(*philos)[i].t_n = 0;
		(*philos)[i].lanched = 0;
		(*philos)[i].alive = 1;
		(*philos)[i].nbtimeeating = 0;
		(*philos)[i].st = ft_tmnw();
		if ((*philos)[i].st < 0)
		{
			free(*philos);
			return (1);
		}
		(*philos)[i].bn = st;
	}
	return (0);
}

int	ft_checkalive(t_philo *args)
{
	args->timeleft += args->timeleft - ft_tmnw();
	if (args->timeleft > args->bn->timetodie)
		return (1);
	return (0);
}
