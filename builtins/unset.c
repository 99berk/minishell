/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bakgun <bakgun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:13:07 by bakgun            #+#    #+#             */
/*   Updated: 2024/05/24 12:55:27 by bakgun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"

int	unset(t_vars *vars, int del, int count)
{
	int		i;
	int		j;
	char	**exports;

	if (vars->input_parsed[1] == NULL)
		return (1);
	i = -1;
	while (vars->env[++i])
	{
		j = 0;
		exports = ft_split(vars->env[i], '=');
		if (!exports)
			return (err_msg("allocation error"), 0);
		while (vars->input_parsed[++j])
		{
			if (ft_strncmp(exports[0], vars->input_parsed[j],
					ft_strlen(exports[0])) == 0)
				if (null_free(&vars->env[i]), del++)
					break ;
		}
		free_doubles(exports);
	}
	return (re_init_env(vars, count, del));
}
