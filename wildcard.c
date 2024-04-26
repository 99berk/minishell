/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alialtintoprak <alialtintoprak@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 15:36:44 by alialtintop       #+#    #+#             */
/*   Updated: 2024/04/16 11:13:03 by alialtintop      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include <stdio.h>
#include <unistd.h>

int	find_in_list_rev(t_vars *vars, char *var, char **split, char **list)
{
	char	*tmp;
	size_t	len;
	int		i;
	int		j;

	i = -1;
	j = -1;
	len = ft_strlen(var);
	var[--len] = '\0';
	while (split[++i])
	{
		tmp = split[i];
		if (ft_strncmp(var, tmp, len) == 0)
		{
			list[++j] = ft_strjoin(tmp, " ");
			if (!list[j])
				return (err_msg("strdup error"), 0);
		}
		tmp = NULL;
	}
	if (j != -1 && !append_doubles(&vars->output, list, 0))
		return (0);
	return (1);
}

int	find_in_list(t_vars *vars, char *var, char **split, char **list)
{
	char	*tmp;
	int		i;
	int		j;

	i = -1;
	j = -1;
	var = var + 1;
	while (split[++i])
	{
		tmp = ft_strrchr(split[i], var[0]);
		if (!tmp)
			continue ;
		if (ft_strncmp(var, tmp, ft_strlen(tmp)) == 0)
		{
			if (ft_strlen(tmp) != ft_strlen(var))
				continue ;
			tmp = ft_strdup(split[i]);
			list[++j] = ft_strjoin(tmp, " ");
			if (null_free(&tmp) && !list[j])
				return (err_msg("strdup error"), 0);
		}
		tmp = NULL;
	}
	return (j == -1 || append_doubles(&vars->output, list, 0));
}

char	*extract_var(t_vars *vars, int i, int rev)
{
	char	*ret;
	int		j;

	j = 0;
	if (rev)
	{
		while (i >= 0 && !is_space(vars->input[i]) && i--)
			j++;
		ret = ft_substr(vars->input, i + 1, j + 1);
		if (!ret)
			return (err_msg("Error"), NULL);
	}
	else
	{
		while (vars->input[i] && !is_space(vars->input[i]) && i++)
			j++;
		ret = ft_substr(vars->input, i - j, j);
		if (!ret)
			return (err_msg("Error"), NULL);
	}
	return (ret);
}

int	find_list(t_vars *vars, char *var, int rev)
{
	char	**split;
	char	**list;

	if (ft_strncmp(var, "*", 2) == 0)
		return (transform_output(vars));
	split = ft_split(vars->output, '\n');
	null_free(&vars->output);
	if (!split)
		return (err_msg("Malloc error"), 0);
	list = ft_calloc(double_counter(split), sizeof(char *));
	if (!list)
		return (err_msg("Malloc error"), free_doubles(split), 0);
	if (rev)
	{
		if (!find_in_list_rev(vars, var, split, list))
			return (free_doubles(split), free_doubles(list), 0);
	}
	else
		if (!find_in_list(vars, var, split, list))
			return (free_doubles(split), free_doubles(list), 0);
	return (free_doubles(split), free_doubles(list), 1);
}

int	wildcard(t_vars *vars, int i, int rev)
{
	char	*var;
	char	*var_tmp;
	char	**tmp;

	if (!exec_ls(vars))
		return (1);
	var = extract_var(vars, i, rev);
	if (!var)
		return (0);
	var_tmp = ft_strdup(var);
	if (!var_tmp)
		return (null_free(&var), err_msg("Error"), 0);
	if (!find_list(vars, var, rev))
		return (null_free(&var_tmp), null_free(&var), 0);
	if (null_free(&var) && !vars->output)
		return (null_free(&var_tmp), 1);
	tmp = split_string(vars->input, var_tmp);
	if (null_free(&var_tmp) && !tmp)
		return (err_msg("Error"), 0);
	null_free(&tmp[1]);
	tmp[1] = ft_strdup(vars->output);
	if (!tmp[1])
		return (free_doubles(tmp), 0);
	i = append_doubles(&vars->input, tmp, 1);
	return (free_doubles(tmp), i);
}
