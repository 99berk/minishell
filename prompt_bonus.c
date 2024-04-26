/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bakgun <bakgun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:13:05 by aaltinto          #+#    #+#             */
/*   Updated: 2024/04/24 16:42:01 by bakgun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include <unistd.h>
#include <stdio.h>
#include <readline/history.h>

int	is_builtin(t_vars *vars)
{
	char	*input;
	int		ret;

	input = strip(vars->input_parsed[0]);
	if (ft_strncmp(input, "cd", 2) == 0)
		ret = new_cd(vars);
	else if (ft_strncmp(input, "pwd", 4) == 0)
		ret = new_pwd(vars);
	else if (ft_strncmp(input, "echo", 5) == 0)
		ret = (echo(vars), 1);
	else if (ft_strncmp(input, "export", 7) == 0)
		ret = (new_export(vars, 1));
	else if (ft_strncmp(input, "env", 4) == 0)
		ret = (new_env(vars), 1);
	else if (ft_strncmp(input, "unset", 6) == 0)
		ret = (unset(vars, 0, double_counter(vars->env)));
	else
		ret = -1;
	if (ret != -1)
		vars->exit_stat = !ret;
	return (null_free(&input), ret);
}

int	something_familiar(t_vars *vars)
{
	char	*tmp;

	if (vars->input_parsed == NULL || is_empty(vars->input_parsed[0]))
		return (1);
	tmp = strip(vars->input_parsed[0]);
	if (!tmp)
		return (err_msg("Strip error"), 0);
	if (ft_strncmp("exit", tmp, 4) == 0)
		return (null_free(&tmp), 2);
	else if (ft_strncmp("hi", tmp, 3) == 0)
		return (null_free(&tmp), printf("hi baby 😘\n"), 1);
	else if (is_builtin(vars) != -1)
		return (null_free(&tmp), 1);
	else
		return (null_free(&tmp), 0);
}

int	check_input(t_vars *vars, int condition)
{
	int		in_quote;
	char	quotes;
	int		i;

	if (ft_strncmp("", vars->input, 2) == 0)
		return (vars->exit_stat = 127, 1);
	if (quote(vars), vars->hist != -1 && condition)
		add_history(vars->input);
	vars->bonus = 1;
	if (!wildcard_parse(vars))
		return (0);
	i = -1;
	in_quote = 0;
	quotes = 0;
	while (vars->input[++i])
	{
		quote_pass (vars->input, i, &quotes, &in_quote);
		if (!in_quote && vars->input[i] == '&')
			if (vars->input[++i] != '&' || vars->input[i + 1] == '&'
				|| vars->input[i + 1] == '|')
				return (err_msg(SYNTAX_ERR), 0);
	}
	if (!seek_operator(vars))
		return (0);
	return (3);
}

int	handle_prompt(t_vars *vars, int condition)
{
	int		ret;

	if (!vars->input)
		return (2);
	if (check_input(vars, condition) != 3)
		return (1);
	if (!dolar_parse(vars, -1) || pipe_parse(vars, -1))
		return (reset_fds(vars), 1);
	if (!condition && !vars->input)
		return (killer(vars), 1);
	if (!vars->input)
		return (reset_fds(vars));
	ret = open_fds_parse(vars);
	if (ret == 0)
		return (vars->exit_stat = 1, 0);
	if (!parse(vars))
		return (reset_fds(vars), 0);
	ret = something_familiar(vars);
	if (!ret)
		path_finder(vars, vars->input_parsed[0], vars->input_parsed);
	if (!reset_fds(vars))
		return (0);
	if (!condition)
		killer(vars);
	return (ret);
}
