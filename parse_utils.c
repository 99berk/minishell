/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alialtintoprak <alialtintoprak@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:55 by aaltinto          #+#    #+#             */
/*   Updated: 2024/04/16 15:48:21 by alialtintop      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include <stdlib.h>

char	**split_string(char *src, char *key)
{
	char	**result;
	char	*before;
	char	*after;
	char	*found;
	int		i;

	result = ft_calloc(3, sizeof(char *));
	if (result == NULL)
		return (err_msg("Memory allocation failed\n"), NULL);
	found = ft_strnstr(src, key, ft_strlen(src));
	if (found == NULL)
		return (NULL);
	before = src;
	*found = '\0';
	after = found + ft_strlen(key);
	i = -1;
	while (++i < 3)
		result[i] = NULL;
	if (ft_strncmp(before, "", 1) != 0)
		result[0] = ft_strdup(before);
	if (ft_strncmp(after, "", 1) != 0)
		result[2] = ft_strdup(after);
	result[1] = ft_strdup(key);
	return (result);
}

char	*strip(char *str)
{
	int	start;
	int	end;

	start = 0;
	end = ft_strlen(str) -1;
	if (!str)
		return (NULL);
	while (is_space(str[start]))
		start++;
	if (start >= end)
		return (ft_strdup(str));
	while (is_space(str[end]))
		end--;
	return (ft_substr(str, start, end - start + 1));
}

int	null_check(char **src)
{
	int	i;
	int	null_check;

	i = -1;
	null_check = 0;
	while (++i < 3)
		if (src[i] == NULL)
			null_check++;
	if (null_check == 3)
		return (1);
	return (0);
}

int	append_doubles(char **dest, char **src, int condition)
{
	int	i;
	int	len;
	int	max_index;

	if (!src)
		return (null_free(&(*dest)), 0);
	if (condition)
		max_index = 3;
	else
		max_index = double_counter(src);
	len = 0;
	i = -1;
	while (++i < max_index)
		if (src[i])
			len += ft_strlen(src[i]);
	if (null_free(&(*dest)), condition && null_check(src))
		return (1);
	*dest = ft_calloc(len + 1, sizeof(char));
	if (!(*dest))
		return (err_msg("Allocation error!"), 0);
	i = -1;
	while (++i < max_index)
		if (src[i])
			ft_strlcat(*dest, src[i], len + 1);
	return (1);
}

int	check_origin(t_vars *vars)
{
	int	check;

	check = 0;
	if (!vars->input)
		return (-1);
	if (ft_strncmp(vars->input, "", 1) == 0)
		return (-1);
	if (ft_strncmp(vars->input, "echo ", 5) == 0
		|| ft_strncmp(vars->input, "export ", 6) == 0
		|| ft_strncmp(vars->input, "exit ", 5) == 0)
		check = 1;
	vars->input_parsed
		= (char **)ft_calloc(ft_strlen(vars->input) + 1, sizeof(char *));
	if (!vars->input_parsed)
		return (err_msg("Allocation Error"), -1);
	return (check);
}
