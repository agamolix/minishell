/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 19:35:18 by atrilles          #+#    #+#             */
/*   Updated: 2022/10/07 01:06:30 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *find_free_var(t_env *env, char *var)
{
	char *res;
	int i;

	i = 0;
	while(env->env[i])
	{
		if (str_n_cmp(env->env[i], var, slen(var)) == 0 && env->env[i][slen(var)] == '=')
		{
			res = env->env[i];
			while (res[0] != '=')
				res++;
			res++;
			free(var);
			return (ft_strdup(res));
		}
		i++;
	}
	if (var[0] == '?')
		return (itoa(env->value));
	free(var);
	return (0);
}


