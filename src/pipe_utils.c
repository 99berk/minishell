/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bakgun <bakgun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 15:37:19 by bakgun            #+#    #+#             */
/*   Updated: 2024/05/24 12:55:27 by bakgun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"
#include <stdio.h>

int	pipe_counter(t_vars *vars)
{
	int		i;
	int		pipe_count;
	int		in_quotes;
	char	quote_type;

	i = -1;
	pipe_count = 0;
	in_quotes = 0;
	quote_type = '\0';
	while (vars->input[++i])
	{
		if (quote_pass(vars->input, i, &quote_type, &in_quotes))
			continue ;
		if (!in_quotes && vars->input[i] == '|')
		{
			if (vars->bonus != 1 && vars->input[i + 1] == '|')
				return (err_msg("syntax error"), -1);
			pipe_count++;
		}
	}
	return (pipe_count);
}

char	**pipe_checker(char **ret)
{
	int		i;

	i = -1;
	while (ret[++i])
		if (is_empty(ret[i]))
			return (err_msg(SYNTAX_ERR), free_doubles(ret), NULL);
	return (ret);
}

int	input_parse_fill(t_vars *vars, int pipe_count)
{
	vars->input_parsed = split_pipes(vars, pipe_count, -1);
	if (!vars->input_parsed)
		return (null_free(&vars->input), 0);
	return (1);
}
