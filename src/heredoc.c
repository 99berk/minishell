/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bakgun <bakgun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 14:36:14 by bakgun            #+#    #+#             */
/*   Updated: 2024/05/24 12:55:27 by bakgun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"
#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>

int	illegal_char(char c)
{
	if (c == '<' || c == '>'
		|| c == '(' || c == ')')
		return (1);
	return (0);
}

static char	*substract_var(t_vars *vars, int *i2)
{
	int		j;
	int		i;
	int		check;
	char	*var;

	j = 3;
	check = 1;
	i = *i2;
	while (vars->input[i++] && (check || !is_space(vars->input[i])))
	{
		if (++j && !is_space(vars->input[i]))
		{
			if (illegal_char(vars->input[i]))
				return (err_msg(SYNTAX_ERR), NULL);
			check = 0;
		}
	}
	if (j <= 3 || check)
		return (err_msg(SYNTAX_ERR), NULL);
	var = ft_substr(vars->input, i - j, j);
	if (!var)
		return (err_msg("Substr error"), NULL);
	i -= (ft_strlen(var) + 2);
	*i2 = i;
	return (var);
}

static int	handle_eof(char *delimeter, int *fd)
{
	char	*new_input;

	while (1)
	{
		new_input = readline("> ");
		if (!new_input || ft_strncmp(new_input, delimeter,
				ft_strlen(delimeter)) == 0)
			break ;
		ft_putendl_fd(new_input, fd[1]);
		null_free(&new_input);
	}
	if (!new_input)
		ft_putchar_fd('\n', 1);
	return (null_free(&new_input), 1);
}

static int	heredoc_loop(t_vars *vars, char *delimeter, int status)
{
	int	fd[2];
	int	pid;

	if (pipe(fd) == -1)
		return (perror("pipe"), null_free(&vars->input), -1);
	vars->origin_stdin = dup(STDIN_FILENO);
	if (vars->origin_stdin == -1)
		return (perror("Error: dup"), -1);
	vars->file_opened = 1;
	pid = fork();
	if (pid < 0)
		return (perror("minishell: fork"), 1);
	if (pid == 0)
	{
		g_l = 42;
		signal(SIGINT, sig_c);
		handle_eof(delimeter, fd);
		killer(vars);
		exit(EXIT_SUCCESS);
	}
	if (dup2(fd[0], 0) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1)
		return (perror("dup2/close"), null_free(&vars->input), -1);
	if (waitpid(pid, &status, 0), WEXITSTATUS(status) == 1)
		null_free(&vars->input);
	return (1);
}

int	heredoc(t_vars *vars, int *i)
{
	char	*var;
	char	*deli;
	char	**tmps;
	char	*tmp;

	var = substract_var(vars, i);
	if (!var)
		return (0);
	tmp = strip(vars->input);
	if (!tmp)
		return (err_msg("Strip error"), null_free(&var), 0);
	tmps = split_string(tmp, var);
	if (!tmps)
		return (null_free(&var), null_free(&tmp), 0);
	if (null_free(&tmp) && !tmps)
		return (null_free(&var), 0);
	if (null_free(&tmps[1]) && !append_doubles(&vars->input, tmps, 1))
		return (null_free(&var), free_doubles2((void **)tmps, 3), 0);
	tmp = strip(var + 2);
	if (free_doubles2((void **)tmps, 3) && !tmp)
		return (err_msg("Strip error"), null_free(&tmp), null_free(&var), 0);
	deli = ft_substr(tmp, 0, ft_strlen(var));
	if (null_free(&var) && null_free(&tmp) && !deli)
		return (err_msg("Substr error"), 0);
	return (heredoc_loop(vars, deli, 0), null_free(&deli), vars->input != NULL);
}
