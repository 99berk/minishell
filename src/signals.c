/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bakgun <bakgun@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 16:13:11 by bakgun            #+#    #+#             */
/*   Updated: 2024/05/24 12:55:27 by bakgun           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"
#include <signal.h>
#include <stdio.h>
#include <readline/readline.h>

void	sig_c(int sig)
{
	(void)sig;
	if (!g_l)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (g_l == 42)
	{
		printf("\n");
		exit(EXIT_FAILURE);
	}
}

void	sig_backslash(int sig)
{
	ft_putstr_fd("Quit: ", 2);
	ft_putnbr_fd(sig, 2);
	ft_putstr_fd("\n", 2);
}

void	init_signals(void)
{
	g_l = 0;
	signal(SIGINT, sig_c);
	signal(SIGQUIT, SIG_IGN);
}

void	init_signals2(void)
{
	g_l = 1;
	signal(SIGINT, sig_c);
	signal(SIGQUIT, sig_backslash);
}
