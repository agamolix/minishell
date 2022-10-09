/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 19:35:18 by atrilles          #+#    #+#             */
/*   Updated: 2022/10/10 01:23:36 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*exist_var(t_env *env, char *str)
{
	int		i;
	char	*var;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	if (str[i] != '=')
		return (0);
	var = sub_str(str, 0, i);
	i = 0;
	while (env->env[i])
	{
		if (str_n_cmp(env->env[i], var, slen(var)) == 0 && \
			env->env[i][slen(var)] == '=')
			return (var);
		i++;
	}
	free(var);
	return (0);
}

int	get_remove_tot(t_env *env, char *var)
{
	int	i;
	int	tot;

	tot = 0;
	i = 0;
	while (env->env[i])
	{
		if (str_n_cmp(env->env[i], var, slen(var)) != 0 || \
			env->env[i][slen(var)] != '=')
			tot++;
		i++;
	}
	return (tot);
}

char	**remove_var(t_env *env, char *var)
{
	char	**res;
	int		i;
	int		tot;

	tot = get_remove_tot(env, var);
	res = malloc(sizeof(char *) * (tot + 1));
	i = 0;
	tot = 0;
	while (env->env[i])
	{
		if (str_n_cmp(env->env[i], var, slen(var)) != 0 || \
			env->env[i][slen(var)] != '=')
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

int	cmd_unset(t_env *env, char **tab)
{
	int	i;

	i = 1;
	if (str_chr(tab[1], '='))
	{
		printf("Error unset: identifier not valid\n");
		env->value = 1;
		env->stop = 1;
		return (env->value);
	}
	while (tab[i])	
	{
		env->env = remove_var(env, tab[i]);
		i++;
	}
	return (0);
}
