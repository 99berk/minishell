/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bakgun <bakgun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:25 by bakgun            #+#    #+#             */
/*   Updated: 2024/05/24 12:55:27 by bakgun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>
#include <stdio.h>
#include "../libft/libft.h"

int	new_pwd(t_vars *vars)
{
	char	*pwd;
	int		i;
	char	**splitted;

	i = find_in_env(vars->env, "PWD", double_counter(vars->env));
	if (i == -1)
	{
		pwd = getcwd(NULL, 0);
		if (!pwd)
			return (perror("getcwd"), 0);
		return (ft_putendl_fd(pwd, 1), null_free(&pwd), 1);
	}
	splitted = ft_split(vars->env[i], '=');
	if (!splitted)
		return (err_msg("Malloc error"), null_free(&vars->env[i]),
			new_pwd(vars), 0);
	ft_putendl_fd(splitted[1], 1);
	return (free_doubles(splitted), 1);
}
