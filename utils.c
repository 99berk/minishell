/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:13:14 by aaltinto          #+#    #+#             */
/*   Updated: 2024/03/22 16:34:35 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

int	is_space(char c)
{
	return (c == ' ' || c == '\t');
}

int	double_counter(char **str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

char	*destroy_quotes(char *str, int check)
{
	char	*ret;
	int		i;
	int		j;
	int		in_quotes;
	char	quote;

	(void)check;
	ret = malloc(ft_strlen(str) + 1);
	if (!ret)
		return (err_msg("Allocation error"), NULL);
	i = -1;
	j = -1;
	in_quotes = 0;
	quote = 0;
	while (str[++i] != '\0')
	{
		quote_pass(str, i, &quote, &in_quotes);
		if (in_quotes && quote != str[i])
			ret[++j] = str[i];
		else if (!is_quote(str[i]))
			ret[++j] = str[i];
	}
	return (ret[++j] = '\0', ret);
}

void	reset_vars(t_vars *vars)
{
	reset_fds(vars);
	vars->file_created = 0;
	vars->file_opened = 0;
	null_free(&vars->input);
	null_free(&vars->output);
	free_doubles(vars->input_parsed);
	reset_fds(vars);
	vars->input_parsed = NULL;
	vars->hist = 0;
}
