/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_output.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bakgun <bakgun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:42 by bakgun            #+#    #+#             */
/*   Updated: 2024/05/24 12:55:27 by bakgun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "../libft/libft.h"

static int	fd_open_operations(t_vars *vars, char *var)
{
	int		fd;
	char	*file;
	char	*tmp;

	if (vars->file_opened)
		if (dup2(vars->origin_stdin, STDIN_FILENO) == -1)
			return (perror("dup2"), -1);
	tmp = ft_substr(var, 1, ft_strlen(var) -1);
	if (!tmp)
		return (err_msg("Substr error"), -1);
	file = strip(tmp);
	if (null_free(&tmp) && !file)
		return (err_msg("Strip error"), -1);
	fd = open(file, O_RDONLY);
	if (null_free(&file) && fd < 0)
		return (perror(file), -1);
	vars->origin_stdin = dup(STDIN_FILENO);
	if (vars->origin_stdin == -1)
		return (perror("Error"), -1);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (perror("dup2"), -1);
	vars->file_opened = 1;
	if (!vars->input)
		return (close(fd), -1);
	return (close(fd), 1);
}

static char	*find_keyword(t_vars *vars, int i, int condition, int j)
{
	int		check;
	int		in_quotes;
	char	quote;
	char	*var;

	check = 1;
	in_quotes = 0;
	quote = 0;
	while (vars->input[++i] && (in_quotes
			|| (check || !is_space(vars->input[i]))))
	{
		quote_pass(vars->input, i, &quote, &in_quotes);
		j++;
		if (j != 1 && !is_space(vars->input[i]))
			check = 0;
		if (j != 1 && illegal_char(vars->input[i]))
			return (err_msg(SYNTAX_ERR), NULL);
	}
	if ((condition && j++ <= 1) || (!condition && check == 1))
		return (err_msg(SYNTAX_ERR), NULL);
	var = ft_substr(vars->input, i - j, j);
	if (!var)
		return (err_msg("Substr error"), NULL);
	return (var);
}

int	open_file(t_vars *vars, int i)
{
	int		ret;
	char	*var;
	char	**tmp;
	char	*tmp2;
	char	*stripped;

	var = find_keyword(vars, i, 0, 0);
	if (!var)
		return (-1);
	stripped = strip(vars->input);
	if (!stripped)
		return (err_msg("Strip error"), null_free(&var), -1);
	tmp = split_string(stripped, var);
	null_free(&stripped);
	if (!tmp)
		return (free(var), -1);
	null_free(&tmp[1]);
	if (!append_doubles(&vars->input, tmp, 1))
		return (null_free(&var), free_doubles(tmp), -1);
	free_doubles2((void **)tmp, 3);
	tmp2 = destroy_quotes(var);
	null_free(&var);
	ret = fd_open_operations(vars, tmp2);
	vars->exit_stat = (ret != -1);
	return (null_free(&tmp2), ret);
}

static int	fd_output_operations(t_vars *vars, char *var)
{
	int		fd;
	char	*file;
	char	*tmp;

	if (vars->file_created)
		if (dup2(vars->origin_stdout, STDOUT_FILENO) == -1)
			return (perror("dup2"), -1);
	tmp = ft_substr(var, 1, (ft_strlen(var) -1));
	if (null_free(&var) && !tmp)
		return (err_msg("Substr error"), -1);
	file = strip(tmp);
	if (null_free(&tmp) && !file)
		return (err_msg("Strip error"), null_free(&tmp), -1);
	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (null_free(&file) && fd < 0)
		return (perror("minishell"), -1);
	vars->origin_stdout = dup(STDOUT_FILENO);
	if (vars->origin_stdout == -1)
		return (perror("minishell: dup"), -1);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (perror("dup2"), -1);
	vars->file_created = 1;
	if (!vars->input)
		return (close(fd), -1);
	return (close(fd), 1);
}

int	output_file(t_vars *vars, int i)
{
	char	*var;
	char	**tmp;
	char	*tmp2;
	char	*stripped;

	var = find_keyword(vars, i, 1, 0);
	if (!var)
		return (-1);
	stripped = strip(vars->input);
	if (!stripped)
		return (err_msg("Strip error"), null_free(&var), -1);
	tmp = split_string(stripped, var);
	null_free(&stripped);
	if (!tmp)
		return (null_free(&var), -1);
	null_free(&tmp[1]);
	if (!append_doubles(&vars->input, tmp, 1))
		return (free_doubles(tmp), -1);
	free_doubles2((void **)tmp, 3);
	tmp2 = destroy_quotes(var);
	null_free(&var);
	return (fd_output_operations(vars, tmp2));
}
