/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alialtintoprak <alialtintoprak@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:46 by aaltinto          #+#    #+#             */
/*   Updated: 2024/04/13 17:25:47 by alialtintop      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "minishell.h"
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"

char	*find_work_dir(char **env)
{
	char	**split;
	char	*pwd;
	char	*ret;
	int		i;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (perror("getcwd"), ft_strdup(".. "));
	i = find_in_env(env, "HOME=");
	if (i != -1 && ft_strncmp(pwd, env[i] + 5, ft_strlen(env[i])) == 0)
		ret = ft_strdup(" ~");
	else if (ft_strncmp(pwd, "/", 2) == 0)
		ret = ft_strdup(" /");
	else
	{
		split = ft_split(pwd, '/');
		if (!split)
			return (null_free(&pwd), NULL);
		i = double_counter(split);
		ret = ft_strjoin(" ", split[i -1]);
		free_doubles(split);
	}
	return (null_free(&pwd), ret);
}

int	prompter(char **env, t_vars *ret)
{
	int		i;
	char	**vars;

	vars = malloc(sizeof(char *) * 6);
	if (!vars)
		return (0);
	i = find_in_env(env, "USER=");
	if (i == -1)
		vars[0] = ft_strdup("user");
	else
		vars[0] = ft_strdup(env[i] + 5);
	vars[1] = ft_strdup("@");
	vars[2] = find_work_dir(env);
	vars[3] = ft_strdup(" $ ");
	if (!vars[0] || !vars[1] || !vars[2] || !vars[3])
		return (err_msg("Strdup error"), free_doubles(vars), 1);
	vars[4] = NULL;
	return (append_doubles(&ret->user_pwd, vars, 0), free_doubles(vars), 1);
}

int	opening_ceremony(t_vars *vars, char **env)
{
	int		i;
	char	**tmp;

	tmp = NULL;
	i = find_in_env(env, "HOME=");
	if (i != -1)
	{
		tmp = ft_split(vars->env[i], '=');
		if (!tmp)
			return (err_msg("ft_split error!"), 0);
		if (chdir(tmp[1]) != 0)
			return (perror("chdir"), 0);
		set_env(vars, "PWD=", tmp[1]);
		free_doubles(tmp);
	}
	else
		if (chdir("/") != 0 || !set_env(vars, "PWD=", "/"))
			return (perror("chdir"), 0);
	i = find_in_env(vars->env, "OLDPWD=");
	printf("\e[1;33mMornin' Sunshine 🌞\n\e[0m");
	if (i != -1)
		return (null_free(&vars->env[i]),
			re_init_env(vars, double_counter(vars->env), 1), 1);
	return (1);
}

int	marche(t_vars *vars, char **env, int condition)
{
	if (!env_init(vars, env))
		return (err_msg("Env couldn't initialized"), 0);
	vars->input = NULL;
	vars->input_parsed = NULL;
	vars->output = NULL;
	vars->user_pwd = NULL;
	vars->file_created = 0;
	vars->file_opened = 0;
	vars->exit_stat = 0;
	vars->hist = 0;
	vars->bonus = 1;
	if (condition)
		return (opening_ceremony(vars, env));
	return (1);
}

int	main(int argc, char **argv, char **env)
{
	t_vars	vars;

	(void)argv;
	if (argc != 1)
		return (err_msg("Error\nRun without arguments"), 1);
	if (!marche(&vars, env, 1))
		exit (EXIT_FAILURE);
	while (1)
	{
		reset_vars(&vars);
		if (!prompter(vars.env, &vars) || !vars.user_pwd)
			return (err_msg("Prompter error!"), 1);
		init_signals();
		vars.input = readline(vars.user_pwd);
		init_signals2();
		null_free(&vars.user_pwd);
		if (handle_prompt(&vars, 1) == 2)
			break ;
	}
	exit_setter(&vars);
	killer(&vars);
	return (vars.exit_stat);
}
