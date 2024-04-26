/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alialtintoprak <alialtintoprak@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 15:36:15 by alialtintop       #+#    #+#             */
/*   Updated: 2024/04/13 15:36:16 by alialtintop      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include "gnl/get_next_line.h"
#include "libft/libft.h"

int	get_list(t_vars *vars, int *pipe_fd)
{
	close(pipe_fd[1]);
	vars->output = get_next_line(pipe_fd[0]);
	if (!vars->output)
		return (0);
	return (1);
}

int	exec_ls(t_vars *vars)
{
	int		p_id;
	int		pipe_fd[2];
	char	**argv;

	if (pipe(pipe_fd) == -1)
		return (perror("pipe"), 0);
	p_id = fork();
	if (p_id == -1)
		return (perror("fork"), 0);
	if (p_id == 0)
	{
		argv = ft_split("ls", ' ');
		if (!argv)
			return (killer(vars), exit(EXIT_FAILURE), 0);
		close(pipe_fd[0]);
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			return (killer(vars), perror("dup2"), 0);
		if (execve("/bin/ls", argv, vars->env) == -1)
			return (killer(vars), perror("execve"), close(pipe_fd[1]),
				close(pipe_fd[0]), exit(EXIT_FAILURE), 0);
	}
	if (!get_list(vars, pipe_fd))
		return (close(pipe_fd[0]), waitpid(p_id, NULL, 0), 0);
	close(pipe_fd[0]);
	return (waitpid(p_id, NULL, 0), 1);
}

int	transform_output(t_vars *vars)
{
	char	**splitted;
	char	*tmp;
	int		i;

	splitted = ft_split(vars->output, '\n');
	null_free(&vars->output);
	if (!splitted)
		return (perror("split"), 0);
	i = -1;
	while (splitted[++i])
	{
		if (splitted[i + 1] == NULL)
			break ;
		tmp = ft_strdup(splitted[i]);
		null_free(&splitted[i]);
		splitted[i] = ft_strjoin(tmp, " ");
		null_free(&tmp);
	}
	return (append_doubles(&vars->output, splitted, 0),
		free_doubles(splitted), 1);
}

int	wildcard_parse(t_vars *vars)
{
	int		i;
	char	quote_type;
	int		in_quotes;

	if (!vars->input)
		return (0);
	i = -1;
	quote_type = 0;
	in_quotes = 0;
	while (vars->input[++i])
	{
		if (quote_pass(vars->input, i, &quote_type, &in_quotes) || in_quotes)
			continue ;
		else if (vars->input[i] == '*')
		{
			if (!wildcard(vars, i, 1))
				return (null_free(&vars->input), 0);
		}
		else if (vars->input[i] == ' ' && vars->input[i + 1] == '*')
			if (!wildcard(vars, ++i, 0))
				return (null_free(&vars->input), 0);
		if (vars->input[i] == '\0')
			break ;
	}
	return (1);
}
