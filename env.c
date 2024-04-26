/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:31 by aaltinto          #+#    #+#             */
/*   Updated: 2024/03/23 14:12:22 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include <stdio.h>

int	env_init(t_vars *vars, char **env)
{
	int	i;

	i = double_counter(env);
	vars->env = ft_calloc(sizeof(char *), (i + 1));
	if (!vars->env)
		return (err_msg("env couldn't set"), 0);
	i = -1;
	while (env[++i])
	{
		vars->env[i] = ft_strdup(env[i]);
		if (!vars->env[i])
			return (free_doubles(vars->env), err_msg("env couldn't set"), 0);
	}
	vars->env[i] = NULL;
	return (1);
}

char	**dup_env(t_vars *vars, char **to_dup)
{
	int		i;
	char	**new_env;

	i = double_counter(to_dup);
	vars->argc = double_counter(vars->input_parsed);
	new_env = malloc((i + vars->argc + 1) * sizeof(char *));
	if (!new_env)
		return (err_msg("Malloc error"), NULL);
	i = -1;
	while (to_dup[++i])
	{
		new_env[i] = ft_strdup(to_dup[i]);
		if (!new_env[i])
			return (err_msg("strdup error"), NULL);
	}
	new_env[i] = NULL;
	return (new_env);
}

int	re_init_env(t_vars *vars, int count, int del)
{
	char	**new_env;
	int		i;
	int		j;

	i = -1;
	j = -1;
	if (del == 0)
		return (1);
	new_env = malloc(sizeof(char *) * (count - del + 1));
	if (!new_env)
		return (0);
	while (count >= ++i)
	{
		if (!vars->env[i])
			continue ;
		new_env[++j] = ft_strdup(vars->env[i]);
		if (!new_env[j])
			return (free_doubles(new_env), 0);
	}
	free_doubles2((void **)vars->env, count);
	new_env[++j] = NULL;
	env_init(vars, new_env);
	free_doubles(new_env);
	return (1);
}

int	find_in_env(char **env, char *to_find, int count)
{
	int		i;
	char	**tmp;

	i = -1;
	while (count >= ++i)
	{
		if (!env[i])
			continue ;
		tmp = ft_split(env[i], '=');
		if (!tmp)
			return (err_msg("split error"), -1);
		if (ft_strncmp(tmp[0], to_find, ft_strlen(tmp[0])) == 0)
			return (free_doubles(tmp), i);
		free_doubles(tmp);
	}
	return (-1);
}
