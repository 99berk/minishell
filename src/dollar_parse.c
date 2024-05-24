/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bakgun <bakgun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:02:43 by bakgun            #+#    #+#             */
/*   Updated: 2024/05/24 12:55:27 by bakgun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"
#include <unistd.h>

static int	exit_status(t_vars *vars, int i)
{
	char	*deli;
	char	**tmp;

	deli = ft_substr(vars->input, i, 2);
	if (!deli)
		return (err_msg("Malloc error"), 0);
	tmp = split_string(vars->input, deli);
	null_free(&deli);
	if (!tmp)
		return (err_msg("Malloc error"), 0);
	null_free(&tmp[1]);
	tmp[1] = ft_itoa(vars->exit_stat);
	if (!tmp[1])
		return (err_msg("itoa error"), 0);
	if (!append_doubles(&vars->input, tmp, 1))
		return (free_doubles2((void **)tmp, 3), 0);
	free_doubles2((void **)tmp, 3);
	return (1);
}

static int	env_find_dollar(t_vars *vars, int i, int j)
{
	char	*var;
	char	**tmp;
	char	**original;

	original = vars->env;
	while (vars->input[++i] && vars->input[i] != ' '
		&& !is_quote(vars->input[i]))
		j++;
	var = ft_substr(vars->input, i - j, j);
	if (!var)
		return (err_msg("Substr error"), -1);
	tmp = split_string(vars->input, var);
	if (!tmp)
		return (null_free(&var), -1);
	j = find_in_env_var(vars->env, var + 1, double_counter(vars->env));
	if (null_free(&var) && null_free(&tmp[1]) && j != -1)
	{
		tmp[1] = ft_strdup(ft_strchr(vars->env[j], '=') + 1);
		if (!tmp[1])
			return (err_msg("Strdup err"), free_doubles2((void **)tmp, 3), -1);
	}
	vars->env = original;
	if (!append_doubles(&vars->input, tmp, 1) || !vars->input)
		return (free_doubles2((void **)tmp, 3), -1);
	return (free_doubles2((void **)tmp, 3), 1);
}

int	dolar_parse(t_vars *vars, int i)
{
	char	quote_type;
	int		in_quotes;

	if (!vars->input)
		return (0);
	quote_type = 0;
	in_quotes = 0;
	while (vars->input[++i])
	{
		if (quote_pass(vars->input, i, &quote_type, &in_quotes)
			|| (in_quotes && quote_type == '\''))
			continue ;
		else if (vars->input[i] == '$' && vars->input[i + 1] == '?')
		{
			if (!exit_status(vars, i))
				return (null_free(&vars->input), 0);
		}
		else if ((vars->input[i] == '$' && !is_quote(vars->input[i + 1]) \
		&& vars->input[i + 1] != '\0' && !is_space(vars->input[i + 1])
				&& env_find_dollar(vars, i -1, 0) == -1))
			return (null_free(&vars->input), 0);
		if (!vars->input || vars->input[i] == '\0')
			break ;
	}
	return (1);
}
