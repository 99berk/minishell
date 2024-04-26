/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42kocaeli.com.  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 14:12:02 by aaltinto          #+#    #+#             */
/*   Updated: 2023/07/11 14:12:04 by aaltinto         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_numlen(int num)
{
	int	i;

	if (num == 0)
		return (1);
	i = 0;
	while (num != 0 && ++i)
		num /= 10;
	return (i);
}

char	*ft_itoa(int n)
{
	int		len;
	char	*str;
	long	nbr;

	nbr = n;
	len = ft_numlen(nbr);
	if (n < 0 && ++len)
		nbr = -nbr;
	str = ft_calloc(len + 1, 1);
	if (!str)
		return (NULL);
	while (--len != -1)
	{
		str[len] = (nbr % 10 + 48);
		nbr /= 10;
	}
	if (n < 0)
		str[0] = '-';
	return (str);
}
