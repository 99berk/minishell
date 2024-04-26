/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alialtintoprak <alialtintoprak@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:39 by aaltinto          #+#    #+#             */
/*   Updated: 2024/04/16 12:44:42 by alialtintop      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "minishell.h"
#include "libft/libft.h"

static int	fd_append_operations(t_vars *vars, char *var)
{
	int		fd;
	char	*file;
	char	*tmp;

	if (vars->file_created)
		if (dup2(vars->origin_stdout, STDOUT_FILENO) == -1)
			return (perror("dup2"), -1);
	tmp = strip(var + 2);
	file = ft_substr(tmp, 0, ft_strlen(var));
	null_free(&tmp);
	if (!file)
		return (err_msg("Error"), -1);
	fd = open(file, O_WRONLY | O_APPEND, O_CREAT, 0644);
	if (fd < 0)
		return (perror(file), null_free(&file), -1);
	null_free(&file);
	vars->origin_stdout = dup(STDOUT_FILENO);
	if (vars->origin_stdout == -1)
		return (perror("error: dup"), -1);
	vars->file_created = 1;
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (perror("dup2"), -1);
	if (!vars->input)
		return (close(fd), -1);
	return (close(fd), 1);
}

static char	*substract_var(t_vars *vars, int i)
{
	int		in_quotes;
	char	quote;
	int		j;
	int		check;
	char	*var;

	check = 1;
	i += 2;
	j = 3;
	in_quotes = 0;
	quote = 0;
	while (vars->input[++i] && (in_quotes || (check || !is_space(vars->input[i] \
	))))
	{
		quote_pass(vars->input, i, &quote, &in_quotes);
		if (j++ && !is_space(vars->input[i]))
			check = 0;
	}
	if (check)
		return (err_msg("minishell: syntax error near unexpected token"), NULL);
	var = ft_substr(vars->input, i - j, j);
	if (!var)
		return (err_msg("ft_substr error"), NULL);
	return (var);
}

int	append_output(t_vars *vars, int i)
{
	int		j;
	char	*var;
	char	**tmp;
	char	*tmp2;

	var = substract_var(vars, i);
	if (!var)
		return (err_msg("Error"), -1);
	tmp2 = strip(vars->input);
	if (!tmp2)
		return (null_free(&var), 0);
	tmp = split_string(tmp2, var);
	if (!tmp)
		return (null_free(&var), null_free(&tmp2), 0);
	if (null_free(&tmp2) && !tmp)
		return (null_free(&var), -1);
	if (null_free(&tmp[1]), !append_doubles(&vars->input, tmp, 1))
		return (err_msg("Error!"), null_free(&var), free_doubles(tmp), -1);
	free_doubles2((void **)tmp, 3);
	tmp2 = destroy_quotes(var, 1);
	j = fd_append_operations(vars, tmp2);
	return (null_free(&var), null_free(&tmp2), j);
}
