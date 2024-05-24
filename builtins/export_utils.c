/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bakgun <bakgun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 17:33:26 by bakgun            #+#    #+#             */
/*   Updated: 2024/05/24 12:55:27 by bakgun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include "../libft/libft.h"

static void	swap(char **a, char **b)
{
	char	*temp;	

	temp = *a;
	*a = *b;
	*b = temp;
}

static void	print_vars(char **arr)
{
	char	**exports;
	int		i;

	i = -1;
	while (arr[++i])
	{
		exports = ft_split(arr[i], '=');
		if (!exports)
			return ;
		if (ft_strchr(arr[i], '=') && !exports[1])
			printf("declare -x %s=\"\"\n", exports[0]);
		else if (!exports[1])
			printf("declare -x %s\n", exports[0]);
		else
			printf("declare -x %s=\"%s\"\n", exports[0],
				(ft_strchr(arr[i], '=') + 1));
		free_doubles(exports);
	}
}

int	bubblesort(t_vars *vars, int n)
{
	int		i;
	int		j;
	char	**arr;

	arr = dup_env(vars, vars->env);
	if (!arr)
		return (0);
	i = -1;
	while (++i < n - 1)
	{
		j = -1;
		while (++j < n - i - 1)
			if (ft_strncmp(arr[j], arr[j + 1], ft_strlen(arr[j])) > 0)
				swap(&arr[j], &arr[j + 1]);
	}
	print_vars(arr);
	return (free_doubles(arr), 1);
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
		if (!tmp2)
			return (err_msg("Strip error"), free_doubles(splited), 0);
		index = find_in_env_var(vars->env, tmp2, count);
		if (free_doubles(splited) && null_free(&tmp2) && index == -1)
			continue ;
		null_free(&vars->env[index]);
		vars->env[index] = NULL;
		del++;
	}
	return (re_init_env(vars, count, del));
}
