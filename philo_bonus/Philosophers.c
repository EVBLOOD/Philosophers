/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 19:45:46 by sakllam           #+#    #+#             */
/*   Updated: 2022/02/12 11:12:08 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosophers.h"

int	main(int argc, char **argv)
{
	t_data			*st;
	t_philo			*philos;

	if (argc >= 5)
	{
		st = ft_parsing(argc, argv);
		if (!st)
		{
			printf ("Something in your inputs is wrong!\n");
			return (1);
		}
		if (ft_getsted(st, &philos))
		{
			free (st);
			return (1);
		}
		if (ft_lifecycle(philos, st))
			return (1);
	}
	else
		printf("Sorry but the number of parameters is not enough!\n");
}
