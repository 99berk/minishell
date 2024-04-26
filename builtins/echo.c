/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:18 by aaltinto          #+#    #+#             */
/*   Updated: 2024/03/15 16:12:19 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"
#include <stdio.h>

void	echo(t_vars *vars)
{
	int		i;
	char	*tmp;

	if (!vars->input_parsed[1])
	{
		printf("\n");
		return ;
	}
	i = 0;
	if (ft_strncmp(vars->input_parsed[1], "-n ", 3) == 0
		|| ft_strncmp(vars->input_parsed[1], "-n", 3) == 0)
		i++;
	while (vars->input_parsed[++i])
	{
		ft_putstr_fd(vars->input_parsed[i], 1);
		if (vars->input_parsed[i + 1] != 0)
			ft_putstr_fd(" ", 1);
	}
	tmp = strip(vars->input_parsed[1]);
	if (ft_strncmp(tmp, "-n", 3) != 0)
		ft_putstr_fd("\n", 1);
	free(tmp);
}
