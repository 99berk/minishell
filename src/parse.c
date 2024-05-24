/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bakgun <bakgun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:58 by bakgun            #+#    #+#             */
/*   Updated: 2024/05/24 12:55:27 by bakgun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"
#include <unistd.h>
#include <stdio.h>

static int	new_count(char ***ret, int count)
{
	int	i;
	int	j;

	i = -1;
	j = 0;
	while (count > ++i)
	{
		if (!(*ret)[i])
			continue ;
		j++;
	}
	return (j);
}

static int	strip_spaces(t_vars *vars)
{
	char	*tmp;

	tmp = strip(vars->input);
	if (!tmp)
		return (err_msg("Strip error"), 0);
	if (is_space(tmp[0]))
		return (null_free(&tmp), 0);
	null_free(&vars->input);
	vars->input = ft_strdup(tmp);
	if (null_free(&tmp), !vars->input)
		return (err_msg("Strdup error"), 0);
	return (1);
}

static char	**destroy_spaces(char ***ret, int count)
{
	int		i;
	int		j;
	int		n_count;
	char	**tmp;

	n_count = new_count(ret, count);
	if (n_count < 1)
		return (NULL);
	tmp = ft_calloc(n_count + 1, sizeof(char *));
	if (!tmp)
		return (err_msg("Allocation error"), free_doubles2((void **)(*ret),
			count), NULL);
	i = -1;
	j = -1;
	while (count > ++i)
	{
		if (!(*ret)[i] || !ft_strncmp((*ret)[i], "", 2))
			continue ;
		tmp[++j] = ft_strdup((*ret)[i]);
		if (!tmp[j])
			return (err_msg("Allocation error"), free_doubles(tmp),
				free_doubles2((void **)(*ret), count), NULL);
	}
	tmp[++j] = NULL;
	return (free_doubles2((void **)(*ret), count), tmp);
}

static int	slice_parse(t_vars *vars, char ***ret, int count)
{
	int		i;
	int		j;
	char	quote;
	int		in_quotes;

	i = -1;
	j = 0;
	quote = 0;
	in_quotes = 0;
	while (vars->input[++i])
	{
		quote_pass(vars->input, i, &quote, &in_quotes);
		if (!in_quotes && is_space(vars->input[i]))
		{
			(*ret)[++count] = ft_substr(vars->input, i - j, j);
			if (!(*ret)[count])
				return (err_msg("Substr error"), 0);
			j = 0;
			continue ;
		}
		j++;
	}
	(*ret)[++count] = ft_substr(vars->input, i - j, j);
	(*ret)[++count] = NULL;
	return (count);
}

int	parse(t_vars *vars, int i, int j)
{
	char	**ret;
	int		count;

	if (!strip_spaces(vars))
		return (null_free(&vars->input), 0);
	ret = ft_calloc(sizeof(char *), ft_strlen(vars->input) + 1);
	if (!ret)
		return (err_msg("Calloc error"), 0);
	count = slice_parse(vars, &ret, -1);
	if (!count)
		return (free_doubles(ret), 0);
	ret = destroy_spaces(&ret, count);
	if (ret)
		vars->input_parsed = malloc(sizeof(char *) * (double_counter(ret) + 2));
	if (!vars->input_parsed)
		return (err_msg("Malloc error"), free_doubles(ret), 0);
	while (ret[++i])
	{
		vars->input_parsed[++j] = destroy_quotes(ret[i]);
		if (!vars->input_parsed[j])
			return (free_doubles(ret), 0);
	}
	return (vars->input_parsed[++j] = NULL, free_doubles(ret), 1);
}
