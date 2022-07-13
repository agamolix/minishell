/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 19:35:18 by atrilles          #+#    #+#             */
/*   Updated: 2022/07/13 01:17:30 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// do we have to add env value for the first two exit cases? 
void cmd_exit(char **tab, t_env *env)
{
	if (!tab[1]) 
		exit(0);
	if (maybe_unsigned_long_long(tab[1]) == 0 || is_unsigned_long_long(tab[1]) == 0)
	{
		printf("Exit error: %s: numeric argument necessary\n", tab[1]);
		exit(2);
	}
	if (tab[2])
	{
		printf("Exit error: too many arguments\n");
		exit(1);
	}
	env->value = convert_long_long(tab[1]) % 256;
	printf("exit\n");
	exit(env->value);
}

int cmd_cd(char **tab, t_env *env)
{
	if (tab[2])
	{
		printf ("Error cd: too many arguments\n");
		env->value = 1;
		env->stop = 1;
		return (env->value);
	}
	if (tab[1] == 0)
	{
		env->value = 0;
		env->stop = 1;
		return (env->value);
	}
	if (chdir(tab[1]))
	{
		perror("Error cd");
		env->value = 1;
		env->stop = 1;
		return (env->value);
	}
	env->value = 0;
	return (env->value);
}
void cmd_env(t_env *env, char **tab)
{
	int i = 0;

	if (tab[1])
	{
		printf("Error env: too many arguments\n");
		env->value = 1;
		env->stop = 1;
		return;
	}
	while (env->env[i])
	{
	    printf("%s\n", env->env[i]);
		i++;
	}
	env->value = 0;
}

void cmd_pwd(t_env *env)
{
	char *buf;

	buf = malloc(sizeof(char) * 999);
	if (buf == 0)
	{
		perror("Erreur pwd");
		env->value = 1;
		env->stop = 1;
		return;
	}
	write(1, getcwd(buf, 1000), str_len(getcwd(buf, 1000)));
	write(1, "\n", 1);
	free(buf);
	env->value = 0;
}

void cmd_echo(char **tab, t_env *env)
{
	int i = 1;

	if (tab[1] && str_n_cmp(tab[1], "$?", 3) == 0)
	{
		put_nbr_fd(env->value, 1);
		write(1, "\n", 1);
	}
	if (tab[1] && str_n_cmp(tab[1], "-n", 3) == 0)
		i = 2;

	while (tab[i])
	{
		write(1, tab[i], str_len(tab[i]));
		i++;
	}
	if (tab[1] && str_n_cmp(tab[1], "-n", 3))
		write(1, "\n", 1);
	env->value = 0;
}
