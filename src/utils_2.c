/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bakgun <bakgun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 14:10:28 by bakgun            #+#    #+#             */
/*   Updated: 2024/05/24 12:55:27 by bakgun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <stdio.h>
#include <unistd.h>

int	is_empty(char *str)
{
	int	i;

	if (!str || ft_strncmp(str, "", 1) == 0)
		return (1);
	i = -1;
	while (str[++i])
		if (!is_space(str[i]))
			return (0);
	return (1);
}

int	in_para(t_vars *vars, int i, int *para)
{
	int	j;

	if (vars->input[i] == 40 || vars->input[i] == 41)
	{
		if (vars->input[i] == 40)
			(*para)++;
		if (vars->input[i] == 41)
			(*para)--;
		if (vars->input[i] == 40 && vars->input[i + 1] != '\0'
			&& !ft_isalnum(vars->input[i + 1]))
		{
			j = i + 1;
			while (vars->input[++j])
				if (ft_isalnum(vars->input[j]))
					break ;
			if (!ft_isalnum(vars->input[j]))
				return (err_msg(SYNTAX_ERR), 0);
		}
	}
	if ((*para) < 0)
		return (err_msg(SYNTAX_ERR), 0);
	return (1);
}

int	destroy_para(t_vars *vars, int para)
{
	int		i;
	int		j;
	char	*dest;

	dest = malloc(sizeof(char) * (ft_strlen(vars->input) + 1));
	if (!dest)
		return (err_msg("Malloc error"), 1);
	i = -1;
	j = -1;
	while (vars->input[++i])
	{
		if (!in_para(vars, i, &para))
			return (null_free(&dest), 1);
		if (vars->input[i] == 40 || vars->input[i] == 41)
			continue ;
		dest[++j] = vars->input[i];
	}
	if (null_free(&vars->input) && para != 0)
		return (err_msg(SYNTAX_ERR), null_free(&dest), 1);
	dest[++j] = '\0';
	vars->input = ft_strdup(dest);
	if (null_free(&dest) && !vars->input)
		return (err_msg("strdup error"), 1);
	return (0);
}

int	reset_fds(t_vars *vars)
{
	if (vars->file_created)
		if (dup2(vars->origin_stdout, STDOUT_FILENO) == -1)
			return (perror("dup2"), 0);
	if (vars->file_opened)
	{
		if (dup2(vars->origin_stdin, STDIN_FILENO) == -1)
			return (perror("dup2"), 0);
	}
	return (1);
}
