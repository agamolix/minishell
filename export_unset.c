/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_unset.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atrilles <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 19:35:18 by atrilles          #+#    #+#             */
/*   Updated: 2022/02/08 16:14:27 by atrilles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *exist_var(char **envp2, char *var)
{
	int i = 0;
	char *var2;

	while (var[i] && var[i] != '=')
		i++;
	if (var[i] != '=')
		return 0;
	var2 = sub_str(var, 0, i);
	i = 0;
	while (envp2[i])
	{
		if (str_n_cmp(envp2[i], var2, str_len(var2)) == 0 && 
		envp2[i][str_len(var2)] == '=')
			return (var2);
		i++;
	}
	return (0);
}

char **remove_var(char **envp2, char *var)
{
	char **res;
	int i = 0;
	int tot = 0;

	while (envp2[i])
	{
		if (str_n_cmp(envp2[i], var, str_len(var)) != 0 || 
		envp2[i][str_len(var)] != '=')
			tot++;
		i++;
	}
	res = malloc(sizeof(char *) * (tot + 1));
	i = 0;
	tot = 0;
	while(envp2[i])
	{
		if (str_n_cmp(envp2[i], var, str_len(var)) != 0 || 
		envp2[i][str_len(var)] != '=')
		{
			res[tot] = str_dup(envp2[i]);
			tot++;
		}
		free(envp2[i]);
		i++;
	}
	res[tot] = 0;
	free(envp2);
	return (res);
}

char **cmd_unset(char **envp2, char **tab)
{
	char **res;
	int i = 0;

	if(str_chr(tab[1], '='))
	{
		printf("Identifier not valid\n");
		return (envp2);
	}
	res = remove_var(envp2, tab[1]);
	return (res);
}

char **add_var(char **envp2, char *var)
{
	char **res;
	int i = 0;

	while (envp2[i])
		i++;
	res = malloc(sizeof(char *) * (i + 2));
	i = 0;
	while(envp2[i])
	{
		res[i] = str_dup(envp2[i]);
		free(envp2[i]);
		i++;
	}
	res[i] = var;
	res[i + 1] = 0;
	free(envp2);
	return (res);
}

char **cmd_export(char **envp2, char **tab)
{
	char ** res;
	char *var;

	if(!str_chr(tab[1], '='))
		return (envp2);
	if (tab[2])
	{
		printf ("Too many arguments\n");
		return (envp2);
	}
	var = exist_var(envp2, tab[1]);
	if (var != 0)
	{
		res = remove_var(envp2, var);
		free(var);
		res = add_var(res, tab[1]);
	}
	else
		res = add_var(envp2, tab[1]);
	return (res);
}
