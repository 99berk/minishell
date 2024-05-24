/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bakgun <bakgun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:53:35 by bakgun            #+#    #+#             */
/*   Updated: 2024/05/24 12:55:27 by bakgun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"
#include <stdlib.h>

char	**re_init_double(char **env, int count, int del)
{
	char	**new_env;
	int		i;
	int		j;

	i = -1;
	j = -1;
	if (del == 0)
		return (env);
	new_env = malloc(sizeof(char *) * (count - del + 2));
	if (!new_env)
		return (err_msg("Malloc error"), NULL);
	while (count >= ++i)
	{
		if (!env[i])
			continue ;
		new_env[++j] = ft_strdup(env[i]);
		if (!new_env[j])
			return (free_doubles(new_env),
				err_msg("Strdup error"), NULL);
	}
	return (new_env[++j] = NULL, new_env);
}
