/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alialtintoprak <alialtintoprak@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 11:09:49 by alialtintop       #+#    #+#             */
/*   Updated: 2024/04/16 11:09:54 by alialtintop      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_logic(int chr, int chr2)
{
	if (chr == '\0')
		return (0);
	if (chr2 == '\0')
		return (0);
	return ((chr == '&' && chr2 == '&')
		|| (chr == '|' && chr2 == '|'));
}

int	split_coms(t_vars *vars, int *p, int i)
{
	int	j;

	j = *p;
	while (vars->input[i + j] != '\0' && !is_logic(vars->input[i + j],
			vars->input[i + j + 1]))
		j++;
	*p = j;
	return (1);
}
