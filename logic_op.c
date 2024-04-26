/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic_op.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alialtintoprak <alialtintoprak@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 15:36:56 by alialtintop       #+#    #+#             */
/*   Updated: 2024/04/16 12:35:31 by alialtintop      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include <stdio.h>

int	count_op(t_vars *vars, int i)
{
	int		check;
	int		count;
	int		in_quotes;
	char	quote_type;

	count = 0;
	in_quotes = 0;
	check = 0;
	quote_type = 0;
	while (vars->input[++i])
	{
		if (quote_pass(vars->input, i, &quote_type, &in_quotes))
			continue ;
		if (!is_space(vars->input[i])
			&& (vars->input[i] != '|' && vars->input[i] != '&'))
			check = 0;
		if (!in_quotes && is_logic(vars->input[i], vars->input[i + 1]) && i++)
		{
			if (check == 1 || vars->input[i + 1] == '|')
				return (err_msg(SYNTAX_ERR), -1);
			check = 1;
			count ++;
		}
	}
	return (count++);
}

int	executer(t_vars **child, char **commands, int i, int i2)
{
	if (i != 0)
	{
		(*child)[i2].input = ft_substr(commands[i], 1, ft_strlen(commands[i]));
		if ((*child)[i2].input == NULL)
			return (err_msg("error message"), 0);
	}
	else
		(*child)[i2].input = ft_strdup(commands[i]);
	if (i == 0 || (commands[i] != NULL && ((commands[i][0] == '&' \
		&& (*child)[i2 - 1].exit_stat == 0) || (commands[i][0] == '|'
		&& (*child)[i2 - 1].exit_stat == 1))))
		handle_prompt(&((*child)[i2]), 0);
	if (commands[i] != NULL && commands[i][0] == '&'
		&& (*child)[i2].exit_stat)
		return (1);
	if (commands[i] != NULL && (commands[i][0] == '|'
		&& !(*child)[i2].exit_stat))
		return (1);
	return (0);
}

int	exec_commands(char **commands, t_vars *vars)
{
	int		i;
	int		check;
	int		i2;
	t_vars	*child;

	if (!ft_strncmp(commands[0], "", 2) || (!ft_strncmp(commands[double_counter \
	(commands) - 1], "&", 2) || !ft_strncmp(commands[double_counter(commands) - \
	1], "|", 2)))
		return (err_msg(SYNTAX_ERR), 0);
	child = malloc(sizeof(t_vars) * (double_counter(commands) + 1));
	if (!child)
		return (err_msg("Malloc error"), 0);
	i = -1;
	i2 = -1;
	while (commands[++i])
	{
		check = 0;
		marche(&child[++i2], vars->env, 0);
		if (executer(&child, commands, i, i2) && killer(&child[i2]))
			continue ;
		check = 1;
	}
	if (check == 1)
		i2 --;
	return (vars->exit_stat = child[i2].exit_stat, free(child), 1);
}

int	check_para(t_vars *vars)
{
	int	i;
	int	inpara;

	i = -1;
	inpara = 0;
	while (vars->input[++i])
	{
		if (vars->input[i] == 40)
		{
			inpara ++;
			vars->input[i] = 32;
		}
		if (vars->input[i] == 41)
		{
			if (!inpara)
				return (err_msg("paranthesis error"), null_free(&vars->input), 0);
			inpara --;
			vars->input[i] = 32;
		}
	}
	if (inpara != 0)
		return (err_msg("paranthesis error"), null_free(&vars->input), 0);
	return (1);
}

int	split_commands(t_vars *vars, char ***commands)
{
	int		i;
	int		j;
	int		index;

	i = 0;
	index = -1;
	if (!check_para(vars))
		return (0);
	while (vars->input[i] != '\0')
	{
		j = 0;
		if (!split_coms(vars, &j, i))
			return (0);
		(*commands)[++index] = ft_substr(vars->input, i, j);
		if ((*commands)[index] == NULL)
			return (err_msg("malloc error"), 0);
		i += j;
		if (vars->input[i] != '\0')
			i++;
	}
	return ((*commands)[++index] = NULL, 1);
}

int	seek_operator(t_vars *vars)
{
	int		count;
	char	**commands;

	if (!vars->input)
		return (0);
	count = count_op(vars, -1);
	if (count == -1)
		return (0);
	if (count == 0)
		return (1);
	commands = malloc(sizeof(char *) * (count + 2));
	if (!commands)
		return (err_msg("Malloc error"), 0);
	commands[0] = NULL;
	if (!split_commands(vars, &commands))
		return (free_doubles(commands), 0);
	if (!exec_commands(commands, vars))
		return (null_free(&vars->input), free_doubles(commands), 0);
	return (null_free(&vars->input), free_doubles(commands), 1);
}
