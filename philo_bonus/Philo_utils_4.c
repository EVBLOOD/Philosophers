/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philo_utils_4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 15:31:03 by sakllam           #+#    #+#             */
/*   Updated: 2022/02/13 15:31:31 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosophers.h"

void	ft_waitforbrothers(t_philo *x)
{
	sem_wait(x->bn->brothers);
	x->bn->st = ft_tmnw();
	pthread_create(&(x->listener),
		NULL, &ft_watcher, (void *)x);
	x->t_n = ft_tmnw();
}
