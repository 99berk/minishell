/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:16 by aaltinto          #+#    #+#             */
/*   Updated: 2024/03/15 16:12:17 by aaltinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"
#include <stdio.h>

void	new_env(t_vars *vars)
{
	int	i;

	i = -1;
	while (vars->env[++i])
		if (ft_strchr(vars->env[i], '=') != 0)
			printf("%s\n", vars->env[i]);
}
