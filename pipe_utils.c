/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 15:37:19 by alialtintop       #+#    #+#             */
/*   Updated: 2024/04/26 17:47:43 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
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
			return (err_msg(SYNTAX_ERR), NULL);
	return (ret);
}
