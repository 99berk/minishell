/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bakgun <bakgun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:12:52 by bakgun            #+#    #+#             */
/*   Updated: 2024/05/24 12:55:27 by bakgun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define SYNTAX_ERR "minishell: syntax error near unexpected token"

int	g_l;

typedef struct s_vars
{
	int		hist;
	int		i;
	char	*user_pwd;
	char	*input;
	char	*output;
	char	**input_parsed;
	char	**env;
	int		argc;
	int		bonus;
	int		exit_stat;
	int		origin_stdin;
	int		origin_stdout;
	int		file_opened;
	int		file_created;
}	t_vars;

//free and exit
int		err_msg(char *msg);
int		free_doubles(char **str);
int		null_free(char **var);
int		killer(t_vars *vars);
int		free_doubles2(void **str, int j);

//execute
int		marche(t_vars *vars, char **env, int condition);
int		handle_prompt(t_vars *vars, int condition);
int		path_finder(t_vars *vars, char *cmd, char **argv);
int		pipe_parse(t_vars *vars, int i);

//env
int		find_in_env(char **env, char *to_find, int count);
int		env_init(t_vars *vars, char **env);
char	**dup_env(t_vars *vars, char **to_dup);
int		find_in_env_var(char **env, char *to_find, int count);
char	**re_init_double(char **env, int count, int del);
int		re_init_env(t_vars *vars, int count, int del);

//builtins
int		exit_setter(t_vars *vars);
int		new_cd(t_vars *vars);
int		new_pwd(t_vars *vars);
void	echo(t_vars *vars);
int		new_export(t_vars *vars, int ret, int i);
int		check_restore(t_vars *vars, int count);
int		unset(t_vars *vars, int del, int count);
void	new_env(t_vars *vars);
int		bubblesort(t_vars *vars, int n);

//pipe utils
int		pipe_counter(t_vars *vars);
char	**pipe_checker(char **ret);
int		input_parse_fill(t_vars *vars, int pipe_count);
char	**split_pipes(t_vars *vars, int pipe_count, int i);

//quote
int		quote(t_vars *vars);
int		quote_pass(char *str, int i, char *quote_type, int *in_quotes);

//utils
int		reset_fds(t_vars *vars);
int		reset_vars(t_vars *vars);
int		is_quote(char c);
int		is_space(char c);
int		is_empty(char *str);
int		double_counter(char **str);
char	**split_string(char *src, char *key);
char	*strip(char *tmp);
char	*destroy_quotes(char *str);
int		append_doubles(char **dest, char **src, int condition);
int		illegal_char(char c);

//parsing
int		parse(t_vars *vars, int i, int j);
int		dolar_parse(t_vars *vars, int i);
int		open_fds_parse(t_vars *vars, int in_quotes, char quote_type);

//input and output
int		open_file(t_vars *vars, int i);
int		output_file(t_vars *vars, int i);
int		append_output(t_vars *vars, int *i);
int		heredoc(t_vars *vars, int *i);

//signals
int		wexitstatus(int status);
void	sig_c(int sig);
void	init_signals(void);
void	init_signals2(void);

//bonus
int		wildcard(t_vars *vars, int i, int rev);
int		get_list(t_vars *vars, int *pipe_fd);
int		exec_ls(t_vars *vars);
int		transform_output(t_vars *vars);
int		wildcard_parse(t_vars *vars);
int		seek_operator(t_vars *vars);
int		split_cmds(t_vars *vars, int *p, int i);
int		is_logic(int chr, int chr2);
int		check_commands(char **commands, t_vars **child);
int		in_para(t_vars *vars, int i, int *para);
int		destroy_para(t_vars *vars, int para);

#endif