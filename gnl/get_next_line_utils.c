/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42kocaeli.com.  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 13:39:54 by aaltinto          #+#    #+#             */
/*   Updated: 2023/08/02 13:39:56 by aaltinto         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen2(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strdup2(const char *s)
{
	size_t	len;
	char	*res;
	int		i;

	i = 0;
	len = ft_strlen2(s) + 1;
	res = (char *)malloc(sizeof(char) * len);
	if (!res)
		return (NULL);
	while (*s)
		res[i++] = *s++;
	res[i] = '\0';
	return (res);
}

char	*ft_substr2(char const *s, size_t start, size_t len)
{
	char	*dest;
	size_t	i;
	size_t	i2;
	size_t	s_len;

	s_len = ft_strlen2(s);
	if (!s || len <= 0)
		return (NULL);
	if (len > s_len)
		len = s_len;
	dest = malloc(sizeof(char) * (len + 1));
	if (!dest)
		return (NULL);
	i = start;
	i2 = 0;
	while (i < s_len && i2 < len)
	{
		dest[i2] = s[i];
		i++;
		i2++;
	}
	dest[i2] = '\0';
	return (dest);
}

char	*ft_strjoin2(char *str, char *buf)
{
	size_t	i;
	size_t	len1;
	size_t	len2;
	char	*res;

	if (!str || !buf)
		return (NULL);
	res = (char *)malloc(ft_strlen2(str) + ft_strlen2(buf) + 1);
	if (!res)
		return (NULL);
	i = 0;
	len1 = 0;
	while (str[len1] != '\0')
		res[i++] = str[len1++];
	len2 = 0;
	while (buf[len2] != '\0')
		res[i++] = buf[len2++];
	res[i] = '\0';
	return (res);
}

void	*free_str(char **str, int i)
{
	char	*line;

	if (!*str)
		return (NULL);
	if (i == 0)
	{
		if (*str)
		{
			free(*str);
			*str = NULL;
		}
		return (NULL);
	}
	else if (i == 1)
	{
		line = ft_strdup2(*str);
		free(*str);
		*str = NULL;
		return (line);
	}
	return (NULL);
}
