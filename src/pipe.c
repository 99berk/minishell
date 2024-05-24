/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bakgun <bakgun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:04 by bakgun            #+#    #+#             */
/*   Updated: 2024/05/24 12:55:27 by bakgun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>
#include <stdio.h>
#include "../libft/libft.h"

char	**split_pipes(t_vars *vars, int pipe_count, int i)
{
	int		i2;
	int		j;
	int		in_quotes;
	char	quote;
	char	**ret;

	ret = malloc(sizeof(char *) * (pipe_count + 2));
	if (!ret)
		return (err_msg("Malloc error"), (void *)0);
	i2 = -1;
	in_quotes = 0;
	quote = 0;
	while (vars->input[++i])
	{
		j = 0;
		while (vars->input[i] && (in_quotes || vars->input[i] != '|') && ++j)
			quote_pass(vars->input, i++, &quote, &in_quotes);
		ret[++i2] = ft_substr(vars->input, i - j, j);
		if (!ret[i2])
			return (err_msg("substr error"), free_doubles(ret), NULL);
		if (vars->input[i] == '\0')
			break ;
	}
	return (ret[++i2] = NULL, pipe_checker(ret));
}

static int	create_newvars(t_vars *vars, char **argv, int i)
{
	t_vars	new_vars;
	char	*tmp;

	(void)i;
	if (!marche(&new_vars, vars->env, 0))
		return (free_doubles(argv), exit(EXIT_FAILURE), 0);
	tmp = strip(argv[i]);
	free_doubles(argv);
	if (!tmp)
		return (err_msg("Strip error"), killer(&new_vars),
			exit(EXIT_FAILURE), 0);
	new_vars.input = ft_strdup(tmp);
	if (null_free(&tmp) && !new_vars.input)
		return (killer(&new_vars), err_msg("Strdup error"),
			exit(EXIT_FAILURE), 0);
	handle_prompt(&new_vars, 0);
	killer(&new_vars);
	return (reset_fds(vars), exit(new_vars.exit_stat), 1);
}

static int	child_process(int **pipes, int pipe_count, t_vars *vars, int i)
{
	int		j;

	if (i != 0)
		if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
			return (perror("minishell: dup2"), exit(EXIT_FAILURE), 0);
	if (i != pipe_count)
		if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
			return (perror("minishell: dup2"), exit(EXIT_FAILURE), 0);
	j = -1;
	while (++j < pipe_count)
		if (close(pipes[j][0]) == -1 || close(pipes[j][1]) == -1)
			return (perror("close"), exit(EXIT_FAILURE), 0);
	return (create_newvars(vars, vars->input_parsed, i), exit(EXIT_FAILURE), 1);
}

static int	pipe_piping(int **pipes, int pipe_count, t_vars *vars, int i)
{
	pid_t	*pid;
	int		j;

	pid = malloc(sizeof(pid_t) * (pipe_count + 1));
	if (!pid)
		return (err_msg("allocation error"), null_free(&vars->input), 0);
	while (++i < pipe_count + 1)
	{
		pid[i] = fork();
		if (pid[i] == -1)
			return (perror("fork"), 0);
		if (pid[i] == 0)
			child_process(pipes, pipe_count, vars, i);
	}
	i = -1;
	while (++i < pipe_count)
		if (close(pipes[i][0]) == -1 || close(pipes[i][1]) == -1)
			return (perror("close"), vars->exit_stat = 1, 0);
	i = -1;
	j = 0;
	while (++i < pipe_count + 1)
		waitpid(pid[i], &j, 0);
	vars->exit_stat = wexitstatus(j);
	return (free(pid), null_free(&vars->input), 1);
}

int	pipe_parse(t_vars *vars, int i)
{
	int		**pipes;
	int		pipe_count;

	if (!vars->input)
		return (0);
	pipe_count = pipe_counter(vars);
	if (pipe_count == -1)
		return (1);
	if (pipe_count == 0)
		return (0);
	pipes = malloc(sizeof(int *) * pipe_count);
	if (!pipes)
		return (err_msg("allocation error"), null_free(&vars->input), 0);
	while (++i < pipe_count)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] && err_msg("allocation error") && null_free(&vars->input))
			return (free_doubles2((void **)pipes, pipe_count), 0);
		if (pipe(pipes[i]) == -1)
			return (perror("pipe"), 0);
	}
	if (!input_parse_fill(vars, pipe_count)
		|| !pipe_piping(pipes, pipe_count, vars, -1))
		return (free_doubles2((void **)pipes, pipe_count), 0);
	return (free_doubles2((void **)pipes, pipe_count), 1);
}
