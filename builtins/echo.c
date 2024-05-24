/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bakgun <bakgun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:18 by bakgun            #+#    #+#             */
/*   Updated: 2024/05/24 12:55:27 by bakgun           ###   ########.fr       */
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
	tmp = strip(vars->input_parsed[1]);
	if (!tmp && err_msg("Strip error"))
		return ;
	i = 0;
	if (ft_strncmp(tmp, "-n ", ft_strlen(tmp)) == 0
		|| ft_strncmp(tmp, "-n", ft_strlen(tmp)) == 0)
		i++;
	while (vars->input_parsed[++i])
	{
		ft_putstr_fd(vars->input_parsed[i], 1);
		if (vars->input_parsed[i + 1] != 0)
			ft_putstr_fd(" ", 1);
	}
	if (ft_strncmp(tmp, "-n", ft_strlen(tmp)) != 0)
		ft_putstr_fd("\n", 1);
	free(tmp);
}
