/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alialtintoprak <alialtintoprak@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 14:10:28 by alialtintop       #+#    #+#             */
/*   Updated: 2024/04/16 14:10:32 by alialtintop      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

int	is_empty(char *str)
{
	int	i;

	if (!str || ft_strncmp(str, "", 1) == 0)
		return (1);
	i = -1;
	while (str[++i])
		if (!is_space(str[i]))
			return (0);
	return (1);
}
