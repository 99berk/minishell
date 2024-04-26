/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaltinto <aaltinto@student.42kocaeli.com.  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 18:24:10 by aaltinto          #+#    #+#             */
/*   Updated: 2023/07/06 20:03:15 by aaltinto         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	if (src == dst)
		return (dst);
	if (dst < src)
		ft_memcpy(dst, src, len);
	else
		while ((len --))
			*(unsigned char *)(dst + len) = *(unsigned char *)(src + len);
	return ((char *)dst);
}
