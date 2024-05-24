/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bakgun <bakgun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 11:09:49 by bakgun            #+#    #+#             */
/*   Updated: 2024/05/24 12:55:27 by bakgun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"
#include <stdlib.h>

int	is_logic(int chr, int chr2)
{
	if (chr == '\0')
		return (0);
	if (chr2 == '\0')
		return (0);
	return ((chr == '&' && chr2 == '&')
		|| (chr == '|' && chr2 == '|'));
}

int	split_cmds(t_vars *vars, int *p, int i)
{
	int	j;
	int	para;

	j = *p;
	para = 0;
	while (vars->input[i + j] != '\0')
	{
		if (!in_para(vars, i + j, &para))
			return (0);
		if (para == 0 && is_logic(vars->input[i + j], vars->input[i + j + 1]))
			break ;
		j++;
	}
	if (para != 0)
		return (err_msg(SYNTAX_ERR), 0);
	*p = j;
	return (1);
}

int	check_commands(char **commands, t_vars **child)
{
	if (!ft_strncmp(commands[0], "", 2) || (!ft_strncmp(commands[double_counter \
	(commands) - 1], "&", 2) || !ft_strncmp(commands[double_counter(commands) - \
	1], "|", 2)))
		return (err_msg(SYNTAX_ERR), 1);
	*child = malloc(sizeof(t_vars) * (double_counter(commands) + 1));
	if (!(*child))
		return (err_msg("Malloc error"), 1);
	return (0);
}
