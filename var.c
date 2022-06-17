/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atrilles <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 19:35:18 by atrilles          #+#    #+#             */
/*   Updated: 2022/02/08 16:14:27 by atrilles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *find_var(char **envp, char *var)
{
	char *res;
	int i;

	i = 0;
	while(envp[i])
	{
		if (str_n_cmp(envp[i], var, str_len(var)) == 0 && envp[i][str_len(var)] == '=')
		{
			res = envp[i];
			while(res[0] != '=')
				res++;
			res++;
			return (res);
		}
		i++;
	}
	return (0);
}

char *find_free_var(char **envp, char *var)
{
	char *res;
	int i;

	i = 0;
	while(envp[i])
	{
		if (str_n_cmp(envp[i], var, str_len(var)) == 0 && envp[i][str_len(var)] == '=')
		{
			res = envp[i];
			while(res[0] != '=')
				res++;
			res++;
			return (res);
		}
		i++;
	}
	if (var[0] == '?')
		return(itoa(value));
	free(var);
	return (0);
}


