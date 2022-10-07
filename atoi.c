/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atoi.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 17:40:41 by atrilles          #+#    #+#             */
/*   Updated: 2022/10/07 22:04:09 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int maybe_unsigned_long_long(char *str)
{
	int len;

	if (str[0] == '-')
		str++;
	len = 0;
	while (str[0])
	{
		if (str[0] < '0' || str[0] > '9')
			return (0);
		len++;
		str++;
	}
	if (len > 19)
		return (0);
	return (1);
}

int is_unsigned_long_long(char *str)
{
	unsigned long long int	res;
    int     len_nb;

    if (str[0] == '-')
        str++;
    len_nb = slen(str);
    res = 0;
    while (len_nb > 0)
    {
        res = res * 10;
        res = res + str[0] - '0';
        if (len_nb != 1)
            str++;
        len_nb--;
    }
    if (res > 9223372036854775807)
        return (0);
    return (1);
}

long long int convert_long_long(char *str)
{
    int     sign;
    int     len_nb;
	long long int	res;

    sign = 1;
    if (str[0] == '-')
    {
        sign = -1;
        str++;
    }
    len_nb = slen(str);
    res = 0;
    while (len_nb > 0)
    {
        res = res * 10;
        res = res + str[0] - '0';
        if (len_nb != 1)
            str++;
        len_nb--;
    }
    return (res * sign);
}

char *itoa(int nb)
{
	int nb2;
	int i;
	char *res;

	if (nb == 0)
		return (str_dup("0"));
	nb2 = nb;
	i = 1;
	while (nb2 / 10 > 0)
	{
		i++;
		nb2 = nb2 / 10;
	}
	res = malloc(sizeof(char) * (i + 1));
	res[i] = 0;
	i--;
	while (nb > 0)
	{
		res[i] = (nb % 10) + '0';
		nb = nb / 10; 
		i--;
	}	
	return (res);
}