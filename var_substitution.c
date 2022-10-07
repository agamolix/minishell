/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_substitution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 20:52:19 by gmillon           #+#    #+#             */
/*   Updated: 2022/10/07 20:52:44 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*insert_variable(char *str, int index1, int index2, char *val)
{
	char	*part1;
	char	*part2;
	char	*temp;
	char	*res;

	part1 = str_n_dup(str, index1 - 1);
	part2 = str_n_dup_start(str, slen(str) - index2, index2);
	if (val == 0)
		res = str_join(part1, part2);
	else
	{
		temp = str_join(part1, val);
		res = str_join(temp, part2);
	}
	free(str);
	return (res);
}

char	*replace_variable(char *temp, t_env *env)
{
	int		i;
	int		j;
	char	*var;

	i = 0;
	j = 0;
	while (temp[i])
	{
		if (temp[i] == '$')
		{
			i++;
			while (temp[i] && maybe_char(temp[i]))
			{	
				j++;
				i++;
			}
			var = str_n_dup_start(temp, j, i - j);
			var = find_free_var(env, var);
			temp = insert_variable(temp, i - j, i, var);
			i = -1;
			j = 0;
		}
		i++;
	}
	return (temp);
}
