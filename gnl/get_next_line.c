/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42kocaeli.com.  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 13:39:05 by aaltinto          #+#    #+#             */
/*   Updated: 2023/08/02 13:39:09 by aaltinto         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*integrate_str(char *str, char *buf)
{
	char	*res;

	if (!str && buf)
	{
		res = ft_strdup2(buf);
		if (!res)
			return (NULL);
		return (res);
	}
	res = ft_strjoin2(str, buf);
	free_str(&str, 0);
	return (res);
}


char	*get_next_line(int fd)
{
	char		buf[BUFFER_SIZE + 1];
	ssize_t		rd;
	static char	*str;

	rd = 1;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (free_str(&str, 0));
	while (rd > 0)
	{
		rd = read(fd, buf, BUFFER_SIZE);
		if ((rd <= 0 && !str) || rd == -1)
			return (free_str(&str, 0));
		buf[rd] = '\0';
		str = integrate_str(str, buf);
	}
	return (free_str(&str, 1));
}
