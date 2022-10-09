/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 22:00:07 by gmillon           #+#    #+#             */
/*   Updated: 2022/10/09 02:13:40 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	slen(char *str)
{
	int	len;

	len = 0;
	while (str && str[len])
		len++;
	return (len);
}

char	*line(char *str, char c)
{
	char	*temp;
	int		len;
	int		i;

	len = 0;
	while (str[len] && str[len] != c)
		len++;
	temp = malloc((len + 1) * sizeof(char));
	i = 0;
	while (i < len)
	{
		temp[i] = str[i];
		i++;
	}
	temp[i] = 0;
	return (temp);
}

char	*forward(char *str, char c)
{
	while (str[0] && str[0] == c)
		str++;
	return (str);
}

int	count(char *str, char c)
{
	int		i;

	i = 0;
	str = forward(str, c);
	while (str[0])
	{
		i++;
		while (str[0] && str[0] != c)
			str++;
		str = forward(str, c);
	}
	return (i);
}

char	**my_split(char *s, char c)
{
	char	**res;
	int		i;

	res = malloc((count(s, c) + 1) * sizeof(char *));
	i = 0;
	s = forward(s, c);
	while (s[0])
	{
		res[i] = line(s, c);
		s = s + slen(res[i]);
		s = forward(s, c);
		i++;
	}
	res[i] = 0;
	return (res);
}
