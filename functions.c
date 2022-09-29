/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 19:35:18 by atrilles          #+#    #+#             */
/*   Updated: 2022/09/29 20:26:14 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"

int str_len(char *str)
{
    int len;

    len = 0;
    while (str[len])
        len++;
    return len;
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
		s = s + str_len(res[i]);
		s = forward(s, c);
		i++;
	}
	res[i] = 0;
	return (res);
}

int str_n_cmp(char *s1, char *s2, size_t n)
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

char    *emptystr()
{
    char    *res;

    res = malloc(sizeof(char));
    res[0] = 0;
    return res;
}

char	*sub_str(char *s, unsigned int start, size_t len)
{
    char    *res;
    int i;

    i = 0;
    if (s == 0)
        return (0);
    if ((int)start > str_len((char *)s))
        return (emptystr());
    if (len > str_len((char *)s) - start)
        res = malloc((str_len((char *)s) - start + 1) * sizeof(char));
    else
        res = malloc((len + 1) * sizeof(char));
    if (res == 0)
        return 0;
    while (s[start + i] && i < (int)len)
    {
        res[i] = s[start + i];
        i++;
    }
    res[i] = 0;
    return (res);
}

char	*str_join(char *s1, char *s2)
{
	char		*res;
	int			i;
	int			j;
	const char	*s = "";

	if (s1 == 0)
		s1 = (char *)s;
	res = malloc((str_len(s1) + str_len(s2) + 1) * sizeof(char));
	if (res == 0)
		return (0);
	i = -1;
	while (s1[++i])
		res[i] = s1[i];
	j = -1;
	while (s2[++j])
		res[i + j] = s2[j];
	res[i + j] = 0;
	if (s1 && *s1)
		free(s1);
	if (s2 && *s2)
		free(s2);
	return (res);
}

char	*str_dup(char *src)
{
	int		size;
	char	*s2;
	int		i;

	size = str_len(src) + 1;
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

char *str_chr(char *s, int c)
{
    if (s == 0)
        return (0);
    while (s[0] && s[0] != (char)c)
        s++;
    if (s[0] == (char)c)
        return ((char *)s);
    return (0);
}

void	put_nbr_fd(int n, int fd)
{
    char c;
    if (fd < 0)
        return;
    if (n == -2147483648)
    {
        put_nbr_fd(n / 10, fd);
        write(fd, "8", 1);
        return;
    }
    else if (n < 0)
    {
        n = n * -1;
        write(fd, "-", 1);
    }
    if (n / 10 > 0)
    {
        put_nbr_fd(n / 10, fd);
        c = n % 10 + '0';
    }
    else
        c = n + '0';
    write(fd, &c, 1);
}