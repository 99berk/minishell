/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bakgun <bakgun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:13:08 by aaltinto          #+#    #+#             */
/*   Updated: 2024/04/24 16:42:27 by bakgun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>

int	quote_pass(char *str, int i, char *quote_type, int *in_quotes)
{
	if (str[i] == '\"' || str[i] == '\'')
	{
		if (!(*quote_type))
			(*quote_type) = str[i];
		if ((*quote_type) == str[i])
		{
			(*in_quotes) = !(*in_quotes);
			if (!(*in_quotes))
				(*quote_type) = '\0';
			return (1);
		}
	}
	return (0);
}

int	get_input(char **new_input, t_vars *vars)
{
	*new_input = readline("> ");
	if (!(*new_input))
	{
		write(1, "\n", 1);
		err_msg("Quote error");
		vars->hist = -1;
		null_free(&vars->input);
		return (1);
	}
	return (0);
}

void	wait_close(int quote_check, int type, t_vars *vars)
{
	char	*new_input;
	char	*tmp;
	int		i;

	while (quote_check % 2 == 0)
	{
		i = -1;
		if (get_input(&new_input, vars))
			return ;
		while (new_input[++i])
		{
			if (type && new_input[i] == '\"')
				quote_check++;
			if (!type && new_input[i] == '\'')
				quote_check++;
		}
		tmp = ft_strjoin(vars->input, "\n");
		null_free(&vars->input);
		vars->input = ft_strjoin(tmp, new_input);
		null_free(&tmp);
		null_free(&new_input);
	}
	if (!quote(vars))
		return ;
}

int	quote(t_vars *vars)
{
	int	i;
	int	in_quote;

	in_quote = 0;
	i = -1;
	while (vars->input[++i])
	{
		if (vars->input[i] == '\"' && in_quote != 2)
		{
			if (!in_quote)
				in_quote = 1;
			else if (in_quote == 1)
				in_quote = 0;
		}
		else if (vars->input[i] == '\'' && in_quote != 1)
		{
			if (!in_quote)
				in_quote = 2;
			else if (in_quote == 2)
				in_quote = 0;
		}
	}
	if (in_quote != 0)
		wait_close(2, in_quote != 2, vars);
	return (in_quote != 0);
}
