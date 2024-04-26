/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bakgun <bakgun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 15:37:30 by alialtintop       #+#    #+#             */
/*   Updated: 2024/04/24 17:00:33 by bakgun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"
#include <stdio.h>

int	check_digits(t_vars *vars, char **split)
{
	int		i;
	int		j;

	i = 0;
	while (split[++i])
	{
		j = -1;
		while (split[i][++j])
		{
			if (j == 0 && (split[i][j] == '+' || split[i][j] == '-'))
				continue ;
			if (!ft_isdigit(split[i][j]))
				return (vars->exit_stat = 255, 
					err_msg("numeric argument required"), 0);
		}
	}
	return (1);
}

int	exit_setter(t_vars *vars)
{
	int		num;
	int		count;

	if (!vars->input_parsed)
		return (ft_putendl_fd("exit", 1), 1);
	count = double_counter(vars->input_parsed);
	if (count == 1)
		return (0);
	if (count > 2)
		return (err_msg("too many arguments"), 0);
	if (!check_digits(vars, vars->input_parsed))
		return (0);
	printf("exit\n");
	num = ft_atoi(vars->input_parsed[1]);
	if (num > 255)
		num = num % 256;
	vars->exit_stat = num;
	return (num);
}
