/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 19:45:43 by sakllam           #+#    #+#             */
/*   Updated: 2022/02/10 10:33:06 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Philosophers.h"

int	allnumbers(char	*data)
{
	int	i;

	i = 0;
	if (data[i] == '-')
		return (1);
	while (data[i])
	{
		if (!(data[i] >= '0' && data[i] <= '9'))
			return (1);
		i++;
	}
	return (0);
}

t_data	*ft_parsing(int parnomber, char **parms)
{
	t_data	*ret;
	int		i;

	i = 0;
	while (parms[++i])
		if (allnumbers(parms[i]))
			return (NULL);
	i = 0;
	while (parms[++i])
		if (ft_atoiprime(parms[i]) <= 0)
			return (NULL);
	ret = malloc(sizeof(t_data) * 1);
	if (parnomber == 6)
		ret->nbtimeat = ft_atoiprime(parms[5]);
	else
		ret->nbtimeat = -1;
	ret->counter = 0;
	ret->numberofphilos = ft_atoiprime(parms[1]);
	ret->timetodie = ft_atoiprime(parms[2]);
	ret->timetoeat = ft_atoiprime(parms[3]) * 1000;
	ret->timetosleep = ft_atoiprime(parms[4]) * 1000;
	ret->hsab = 0;
	return (ret);
}
