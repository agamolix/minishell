/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 19:35:18 by atrilles          #+#    #+#             */
/*   Updated: 2022/10/10 01:19:56 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**add_var(t_env *env, char *var)
{
	char	**res;
	int		i;

	i = 0;
	while (env->env[i])
		i++;
	res = malloc(sizeof(char *) * (i + 2));
	i = 0;
	while (env->env[i])
	{
		res[i] = str_dup(env->env[i]);
		free(env->env[i]);
		i++;
	}
	res[i] = ft_strdup(var);
	res[i + 1] = 0;
	free(env->env);
	return (res);
}

int	handle_export_err(t_env *env, char **tab)
{
	if (!str_chr(tab[1], '='))
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
	return (-1);
}

int	cmd_export(t_env *env, char **tab)
{
	char	*var;
	int		error_check;

	error_check = handle_export_err(env, tab);
	if (error_check != -1)
		return (error_check);
	var = exist_var(env, tab[1]);
	if (var != 0)
	{
		env->env = remove_var(env, var);
		env->env = add_var(env, tab[1]);
	}
	else
		env->env = add_var(env, tab[1]);
	free(var);
	return (0);
}
