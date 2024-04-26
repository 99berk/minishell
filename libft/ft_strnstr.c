/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42kocaeli.com.  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 13:26:32 by aaltinto          #+#    #+#             */
/*   Updated: 2023/07/07 14:02:03 by aaltinto         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	f;

	i = 0;
	if (needle == haystack || ft_strlen(needle) == 0)
		return ((char *)haystack);
	while (haystack[i] && i < len)
	{
		f = 0;
		while (haystack[i + f] == needle[f] && i + f < len)
		{
			if (needle[f + 1] == '\0')
				return ((char *)(haystack + i));
			f++;
		}
		i++;
	}
	return ((void *)0);
}
