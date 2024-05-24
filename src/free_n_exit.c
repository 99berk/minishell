/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_n_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bakgun <bakgun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:37 by bakgun            #+#    #+#             */
/*   Updated: 2024/05/24 12:55:27 by bakgun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"
#include <stdlib.h>

int	killer(t_vars *vars)
{
	free_doubles(vars->input_parsed);
	vars->input_parsed = NULL;
	free_doubles(vars->env);
	vars->env = NULL;
	null_free(&vars->input);
	null_free(&vars->output);
	null_free(&vars->user_pwd);
	return (1);
}

int	err_msg(char *msg)
{
	ft_putendl_fd(msg, 2);
	return (1);
}

int	free_doubles(char **str)
{
	int	i;

	if (!str)
		return (1);
	i = -1;
	while (str[++i])
		null_free(&str[i]);
	free(str);
	str = NULL;
	return (1);
}

int	free_doubles2(void **str, int j)
{
	int		i;

	if (!str)
		return (1);
	i = -1;
	while (j > ++i)
	{
		if (str[i] != NULL)
			null_free((char **)&str[i]);
	}
	free(str);
	str = NULL;
	return (1);
}

int	null_free(char **var)
{
	if (!*var)
		return (1);
	free(*var);
	*var = NULL;
	return (1);
}
