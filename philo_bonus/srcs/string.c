/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 18:13:53 by plouvel           #+#    #+#             */
/*   Updated: 2022/05/12 16:21:14 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>

char	*ft_strcat(char *dest, const char *src)
{
	size_t	i;
	char	*org;

	i = 0;
	org = dest;
	while (*dest)
		dest++;
	while (src[i])
	{
		dest[i] = src[i];
		++i;
	}
	dest[i] = '\0';
	return (org);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}
