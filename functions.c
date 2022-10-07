/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 19:35:18 by atrilles          #+#    #+#             */
/*   Updated: 2022/10/07 22:02:46 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"

char	*str_chr(char *s, int c)
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
	char	c;

	if (fd < 0)
		return ;
	if (n == -2147483648)
	{
		put_nbr_fd(n / 10, fd);
		write(fd, "8", 1);
		return ;
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
