/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_create.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bakgun <bakgun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 11:59:44 by bakgun            #+#    #+#             */
/*   Updated: 2024/05/24 12:00:38 by bakgun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <unistd.h>
#include "../libft/libft.h"

static int	syntax_error(t_vars *vars, int i, int x)
{
	if (!vars->input || ft_strlen(vars->input) <= (size_t)(i + x))
		return (0);
	return (0);
}

static int	check_triple_redirection(char *input, int i)
{
	return ((input[i] == '<' && input[i + 1] == '<' && input[i + 2] == '<')
		|| (input[i] == '>' && input[i + 1] == '>' && input[i + 2] == '>'));
}

static int	handle_input_redirection(t_vars *vars, int i)
{
	if ((vars->input[i] == '<' && vars->input[i + 1] == '<' && ++i
			&& i++ && heredoc(vars, &i) == 0) || (syntax_error(vars, i, 0)
			|| (vars->input[i] == '<' && open_file(vars, i - 1) == -1)))
		return (null_free(&vars->input), 0);
	return (1);
}

static int	handle_output_redirection(t_vars *vars, int i)
{
	if (((vars->input[i] == '>' && vars->input[i + 1] == '>'
				&& append_output(vars, &i) == -1) || (syntax_error(vars, i, 0)
				|| (vars->input[i] == '>' && output_file(vars, i) == -1))))
		return (null_free(&vars->input), 0);
	return (1);
}

int	open_fds_parse(t_vars *vars, int in_quotes, char quote_type)
{
	int		i;

	i = -1;
	while (vars->input && vars->input[++i])
	{
		if (quote_pass(vars->input, i, &quote_type, &in_quotes) || in_quotes)
			continue ;
		else if (check_triple_redirection(vars->input, i))
			return (err_msg(SYNTAX_ERR), null_free(&vars->input), 0);
		else if (vars->input[i] == '<' && !handle_input_redirection(vars, i))
			return (0);
		else if (vars->input[i] == '>' && !handle_output_redirection(vars, i))
			return (0);
		if (ft_strlen(vars->input) - 1 <= (size_t)i + 1)
			break ;
	}
	return (1);
}
