/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alialtintoprak <alialtintoprak@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 17:33:26 by aaltinto          #+#    #+#             */
/*   Updated: 2024/04/13 16:02:16 by alialtintop      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include "../libft/libft.h"

void	print_vars(t_vars *vars)
{
	char	**exports;
	char	*tmp;
	int		i;

	i = -1;
	while (vars->env[++i])
	{
		tmp = vars->env[i];
		exports = ft_split(vars->env[i], '=');
		if (!exports)
			return ;
		if (ft_strchr(vars->env[i], '=') && !exports[1])
			printf("declare -x %s=\"\"\n", exports[0]);
		else if (!exports[1])
			printf("declare -x %s\n", exports[0]);
		else
			printf("declare -x %s=\"%s\"\n", exports[0],
				(ft_strchr(vars->env[i], '=') + 1));
		vars->env[i] = tmp;
		free_doubles(exports);
	}
}

int	check_restore(t_vars *vars, int count)
{
	int		i;
	int		index;
	int		del;
	char	**splited;
	char	*tmp2;

	i = 0;
	del = 0;
	count = double_counter(vars->env);
	while (vars->input_parsed[++i])
	{
		splited = ft_split(vars->input_parsed[i], '=');
		if ((!splited || !splited[0]) && free_doubles(splited))
			continue ;
		tmp2 = strip(splited[0]);
		index = find_in_env(vars->env, tmp2, count);
		if (free_doubles(splited) && null_free(&tmp2) && index == -1)
			continue ;
		null_free(&vars->env[index]);
		vars->env[index] = NULL;
		del++;
	}
	return (re_init_env(vars, count, del));
}
