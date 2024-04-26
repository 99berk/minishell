/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:34 by aaltinto          #+#    #+#             */
/*   Updated: 2024/03/24 18:00:50 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include <unistd.h>
#include <stdio.h>

int	wexitstatus(int status)
{
	return ((status >> 8) & 0x000000ff);
}

int	pipe_exec(char *path, t_vars *vars, char **argv)
{
	int		p_id;
	int		status;

	p_id = fork();
	if (p_id == -1)
		return (perror("fork"), 0);
	if (p_id == 0)
	{
		if (execve(path, argv, vars->env) == -1)
		{
			killer(vars);
			return (perror("execve"), exit(EXIT_FAILURE), 0);
		}
	}
	waitpid(p_id, &status, 0);
	vars->exit_stat = wexitstatus(status);
	return (vars->exit_stat);
}

int	check_command(char *cmd, char *tmp, char **argv, t_vars *vars)
{
	char	*this_is_the_way;

	this_is_the_way = ft_strjoin(cmd, tmp);
	if (!this_is_the_way)
		return (err_msg("Join error"), -1);
	if (access(this_is_the_way, F_OK) == 0)
		return (null_free(&tmp), pipe_exec(this_is_the_way, vars, argv),
			null_free(&this_is_the_way), 1);
	return (null_free(&this_is_the_way), 0);
}

int	execute(t_vars *vars, char *cmd, char **argv, char	**split_path)
{
	char	*tmp;
	char	*tmp2;
	int		i;

	i = -1;
	if (access(cmd, F_OK) == 0)
		return (pipe_exec(cmd, vars, argv), 1);
	tmp = ft_strjoin("/", cmd);
	if (!tmp)
		return (err_msg("Join error"), -1);
	while (split_path && split_path[++i])
		if (check_command(split_path[i], tmp, argv, vars))
			return (1);
	tmp2 = getcwd(NULL, 0);
	if (!tmp2)
		return (perror("getcwd"), -1);
	if (check_command(tmp2, tmp, argv, vars))
		return (1);
	return (null_free(&tmp), null_free(&tmp2), 0);
}

int	path_finder(t_vars *vars, char *cmd, char **argv)
{
	char	**split_path;
	char	*err;
	int		i;

	if (argv == NULL)
		return (0);
	i = find_in_env(vars->env, "PATH=", double_counter(vars->env));
	if (i != -1)
	{
		split_path = ft_split(vars->env[i], ':');
		if (!split_path)
			return (err_msg("Split error!"), 0);
	}
	else
		split_path = NULL;
	i = execute(vars, cmd, argv, split_path);
	if (i)
		return (free_doubles(split_path), 1);
	if (i == -1)
		return (free_doubles(split_path), 0);
	free_doubles(split_path);
	err = ft_strjoin("minishell: command not found: ", cmd);
	return (err_msg(err), null_free(&err), 0);
}
