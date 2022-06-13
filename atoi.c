/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atoi.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atrilles <atrilles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 17:40:41 by atrilles          #+#    #+#             */
/*   Updated: 2022/04/22 17:41:03 by atrilles         ###   ########.fr       */
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
    len_nb = str_len(str);
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
    len_nb = str_len(str);
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
