/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:13 by aaltinto          #+#    #+#             */
/*   Updated: 2024/03/19 20:01:33 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int	get_oldpwd(t_vars *vars)
{
	int		i;
	char	**tmp;

	i = find_in_env(vars->env, "OLDPWD=", double_counter(vars->env));
	if (i == -1)
		return (err_msg("minishell: cd: OLDPWD not set"), 0);
	tmp = ft_split(vars->env[i], '=');
	if (!tmp)
		return (err_msg("Malloc error"), 0);
	ft_putendl_fd(tmp[1], 1);
	free_doubles(tmp);
	return (1);
}

int	create_line(t_vars *vars, char *tmp, int i)
{
	char	**new_env;

	new_env = dup_env(vars, vars->env);
	if (!new_env)
		return (0);
	i = double_counter(vars->env);
	new_env[i] = ft_strdup(tmp);
	if (!new_env[i])
		return (err_msg("Malloc error"), free_doubles(new_env), 0);
	new_env[++i] = NULL;
	if (free_doubles(vars->env) && !env_init(vars, new_env))
		return (free_doubles(new_env), 0);
	free_doubles2((void **)new_env, i + vars->argc - 1);
	return (1);
}

int	set_env(t_vars *vars, char *to_find, char *to_set)
{
	int		i;
	char	*tmp;

	i = find_in_env(vars->env, to_find, double_counter(vars->env));
	tmp = ft_strjoin(to_find, to_set);
	if (!tmp)
		return (err_msg("Malloc error"), 0);
	if (i != -1 && null_free(&vars->env[i]))
	{
		vars->env[i] = ft_strdup(tmp);
		if (!vars->env[i])
			return (err_msg("Malloc error"), 0);
	}
	else
		if (!create_line(vars, tmp, i))
			return (0);
	return (null_free(&tmp), 1);
}

char	*get_env(t_vars *vars, char *to_find)
{
	int		i;
	char	**splitted;
	char	*tmp;

	i = find_in_env(vars->env, to_find, double_counter(vars->env));
	if (i == -1)
		return (NULL);
	splitted = ft_split(vars->env[i], '=');
	if (!splitted)
		return (NULL);
	tmp = ft_strdup(splitted[1]);
	free_doubles(splitted);
	if (!tmp)
		return (NULL);
	return (tmp);
}

int	new_cd(t_vars *vars)
{
	char	*old_path;
	char	*new_path;
	int		i;

	old_path = getcwd(NULL, 0);
	if (!old_path)
	{
		perror("getcwd");
		i = find_in_env(vars->env, "PWD=", double_counter(vars->env));
		if (i == -1)
			old_path = ft_strdup("..");
		else
			old_path = ft_strdup(vars->env[i] + 4);
	}
	if (ft_strncmp(vars->input_parsed[1], "-", 2) == 0)
		return (null_free(&old_path), get_oldpwd(vars));
	if (chdir(vars->input_parsed[1]) != 0)
		return (perror("minishell: cd"), null_free(&old_path), 0);
	new_path = getcwd(NULL, 0);
	if (!new_path)
		return (perror("minishell: getcwd"), null_free(&old_path), 0);
	if (!set_env(vars, "PWD=", new_path) || !set_env(vars, "OLDPWD=", old_path))
		return (null_free(&new_path), null_free(&old_path), 0);
	return (null_free(&new_path), null_free(&old_path), 1);
}
