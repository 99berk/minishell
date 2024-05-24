/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bakgun <bakgun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 15:46:23 by bakgun            #+#    #+#             */
/*   Updated: 2024/05/24 12:55:27 by bakgun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*dest;
	int		i;
	int		i2;

	if (!s)
		return (NULL);
	i2 = start;
	i = 0;
	if (!s)
		return (0);
	if (start > ft_strlen(s) || len == 0)
		return (ft_strdup(""));
	if (len > ft_strlen(s + start))
		len = ft_strlen(s + start);
	dest = (char *)malloc(len + 1);
	if (dest == NULL)
		return (NULL);
	while (len != 0)
	{
		dest[i++] = s[i2++];
		len--;
	}
	dest[i] = '\0';
	return (dest);
}
