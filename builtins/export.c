/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bakgun <bakgun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:21 by bakgun            #+#    #+#             */
/*   Updated: 2024/05/24 12:55:27 by bakgun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"
#include <stdio.h>
#include <unistd.h>

static int	illegal_char_check(char *str)
{
	int		i;
	char	*err;

	i = -1;
	while (str[++i])
	{
		if ((i == 0 && (ft_isalpha(str[i]) || str[i] == '_')) || \
	(i != 0 && (str[i] == '=' || str[i] == '_' || ft_isalnum(str[i]))))
			continue ;
		err = ft_strjoin("export: invalid identifier found: ", str);
		if (!err)
			return (err_msg("strjoin error"), 1);
		err_msg(err);
		null_free(&err);
		return (1);
	}
	return (0);
}

static char	*find_before_eq(char *var)
{
	char	**split;
	char	*tmp;

	split = ft_split(var, '=');
	if (!split)
		return (err_msg("Split error"), NULL);
	if (ft_strncmp(split[0], "", 1) == 0 || !split[0]
		|| var[0] == '=')
		return (free_doubles(split), err_msg("not a valid identifier!"), NULL);
	tmp = strip(split[0]);
	if (free_doubles(split), !tmp)
		return (err_msg("Strip error"), NULL);
	return (tmp);
}

static int	check_val(t_vars *vars, char ***new_env, char *export)
{
	char	**tmp2;
	char	*tmp;
	int		i;
	int		count;

	count = double_counter(*new_env);
	tmp = find_before_eq(export);
	if (!tmp)
		return (0);
	if (illegal_char_check(tmp))
		return (null_free(&tmp), 1);
	i = find_in_env_var(*new_env, tmp, double_counter(*new_env));
	if (null_free(&tmp) && i == -1)
		return (null_free(&tmp), 0);
	null_free(&(*new_env)[i]);
	tmp2 = re_init_double(*new_env, count, 1);
	if (!tmp2)
		return (err_msg("env couldn't re-init"), 1);
	free_doubles2((void **)(*new_env), count);
	*new_env = dup_env(vars, tmp2);
	if (!new_env)
		return (err_msg("env couldn't re-init"), 1);
	return (free_doubles(tmp2), 0);
}

static int	check_export(t_vars *vars, char ***new_env, int i, int *i2)
{
	int		j2;
	char	*tmp;

	j2 = *i2;
	(*new_env)[j2 + 1] = NULL;
	tmp = find_before_eq(vars->input_parsed[i]);
	if (!tmp)
		return (0);
	if (find_in_env_var(*new_env, tmp, j2) != -1)
		j2--;
	null_free(&tmp);
	*i2 = j2;
	return (1);
}

int	new_export(t_vars *vars, int ret, int i)
{
	int		i2;
	char	**new_env;

	if (!check_restore(vars, 0))
		return (0);
	new_env = dup_env(vars, vars->env);
	if (!new_env)
		return (0);
	i2 = double_counter(new_env) - 1;
	while ((vars->argc + 1) > ++i)
	{
		if (!vars->input_parsed[i] || !check_export(vars, &new_env, i, &i2))
			continue ;
		if (check_val(vars, &new_env, vars->input_parsed[i]))
		{
			ret = 0;
			continue ;
		}
		new_env[++i2] = ft_strdup(vars->input_parsed[i]);
		if (!new_env[i2])
			return (free_doubles(new_env), err_msg("Something wrong"), 2);
	}
	return (new_env[++i2] = NULL, free_doubles(vars->env), env_init(vars, \
	new_env), free_doubles2((void **)new_env, i2), ret);
}
