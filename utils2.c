/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 22:00:53 by gmillon           #+#    #+#             */
/*   Updated: 2022/10/07 23:55:14 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*emptystr(void)
{
	char	*res;

	res = malloc(sizeof(char));
	res[0] = 0;
	return (res);
}

char	*sub_str(char *s, unsigned int start, size_t len)
{
	char	*res;
	int		i;

	i = 0;
	if (s == 0)
		return (0);
	if ((int)start > slen((char *)s))
		return (emptystr());
	if (len > slen((char *)s) - start)
		res = malloc((slen((char *)s) - start + 1) * sizeof(char));
	else
		res = malloc((len + 1) * sizeof(char));
	if (res == 0)
		return (0);
	while (s[start + i] && i < (int)len)
	{
		res[i] = s[start + i];
		i++;
	}
	res[i] = 0;
	return (res);
}

void	*free_and_null(void *ptr)
{
	if (ptr)
		free (ptr);
	ptr = NULL;
	return (NULL);
}

char	*str_join(char *s1, char *s2)
{
	char		*res;
	int			i;
	int			j;
	const char	*s = "";
	int			null;

	null = 0;
	if (s1 == 0)
	{
		s1 = (char *)s;
		null = 1;
	}
	res = malloc((slen(s1) + slen(s2) + 1) * sizeof(char));
	i = -1;
	while (s1[++i])
		res[i] = s1[i];
	j = -1;
	while (s2[++j])
		res[i + j] = s2[j];
	res[i + j] = 0;
	if (s1 && !null)
		s1 = free_and_null(s1);
	if (s2)
		s2 = free_and_null(s2);
	return (res);
}

int	str_n_cmp(char *s1, char *s2, size_t n)
{
	size_t	i;
	int		res;

	i = 0;
	res = 0;
	while (i < n)
	{
		res = s1[i] - s2[i];
		if (res != 0)
			return (res);
		if (s1[i] == 0 || s2[i] == 0)
			return (0);
		i++;
	}
	return (res);
}
