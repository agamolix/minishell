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

char *exist_var(t_env *env, char *str)
{
	int i = 0;
	char *var;

	while (str[i] && str[i] != '=')
		i++;
	if (str[i] != '=')
		return 0;
	var = sub_str(str, 0, i);
	i = 0;
	while (env->env[i])
	{
		if (str_n_cmp(env->env[i], var, str_len(var)) == 0 && 
		env->env[i][str_len(var)] == '=')
			return (var);
		i++;
	}
	return (0);
}

char **remove_var(t_env *env, char *var)
{
	char **res;
	int i = 0;
	int tot = 0;

	while (env->env[i])
	{
		if (str_n_cmp(env->env[i], var, str_len(var)) != 0 || 
		env->env[i][str_len(var)] != '=')
			tot++;
		i++;
	}
	res = malloc(sizeof(char *) * (tot + 1));
	i = 0;
	tot = 0;
	while(env->env[i])
	{
		if (str_n_cmp(env->env[i], var, str_len(var)) != 0 || 
		env->env[i][str_len(var)] != '=')
		{
			res[tot] = str_dup(env->env[i]);
			tot++;
		}
		free(env->env[i]);
		i++;
	}
	res[tot] = 0;
	free(env->env);
	return (res);
}

int cmd_unset(t_env *env, char **tab)
{
	if(str_chr(tab[1], '='))
	{
		printf("Error unset: identifier not valid\n");
		env->value = 1;
		env->stop = 1;
		return (env->value);
	}
	env->env = remove_var(env, tab[1]);
	return (0);
}

char **add_var(t_env *env, char *var)
{
	char **res;
	int i = 0;

	while (env->env[i])
		i++;
	res = malloc(sizeof(char *) * (i + 2));
	i = 0;
	while(env->env[i])
	{
		res[i] = str_dup(env->env[i]);
		free(env->env[i]);
		i++;
	}
	res[i] = var;
	res[i + 1] = 0;
	free(env->env);
	return (res);
}

int cmd_export(t_env *env, char **tab)
{
	char *var;

	if(!str_chr(tab[1], '='))
	{
		printf ("Error export : '=' nothing happens on bash\n");
		env->value = 0;
		env->stop = 1;
		return (env->value);
	}
	if (tab[2])
	{
		printf ("Error export: invalid identifier\n");
		env->value = 1;
		env->stop = 1;
		return (env->value);
	}
	var = exist_var(env, tab[1]);
	if (var != 0)
	{
		env->env = remove_var(env, var);
		free(var);
		env->env = add_var(env, tab[1]);
	}
	else
		env->env = add_var(env, tab[1]);
	return (0);
}
