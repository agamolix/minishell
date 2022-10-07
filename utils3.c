/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 22:02:07 by gmillon           #+#    #+#             */
/*   Updated: 2022/10/07 22:10:40 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*str_dup(char *src)
{
	int		size;
	char	*s2;
	int		i;

	size = slen(src) + 1;
	s2 = malloc (size * sizeof(char));
	if (s2 == 0)
		return (0);
	i = 0;
	while (src[i])
	{
		s2[i] = src[i];
		i++;
	}
	s2[i] = 0;
	return (s2);
}

char	*str_n_dup(char *src, int nb)
{
	int		size;
	char	*s2;
	int		i;

	size = nb + 1;
	s2 = malloc (size * sizeof(char));
	if (s2 == 0)
		return (0);
	i = 0;
	while (src[i] && i < nb)
	{
		s2[i] = src[i];
		i++;
	}
	s2[i] = 0;
	return (s2);
}

char	*str_n_dup_start(char *src, int nb, int start)
{
	int		size;
	char	*s2;
	int		i;

	size = nb + 1;
	s2 = malloc (size * sizeof(char));
	if (s2 == 0)
		return (0);
	i = 0;
	while (src[i + start] && i < nb)
	{
		s2[i] = src[i + start];
		i++;
	}
	s2[i] = 0;
	return (s2);
}
