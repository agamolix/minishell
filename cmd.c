/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atrilles <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 19:35:18 by atrilles          #+#    #+#             */
/*   Updated: 2022/02/08 16:14:27 by atrilles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void cmd_exit(char **tab)
{
	if (maybe_unsigned_long_long(tab[1]) == 0 || is_unsigned_long_long(tab[1]) == 0)
	{
		printf("bash: exit: %s: numeric argument necessary\n", tab[1]);
		exit(2);
	}
	if (tab[2])
	{
		printf("bash: exit: too many arguments\n");
		exit(1);
	}
	value = convert_long_long(tab[1]) % 256;
	printf("exit\n");
	exit(value);
}

int cmd_env(char **envp2, char **tab)
{
	int i = 0;

	if (tab[1])
	{
		printf ("bash: too many arguments\n");
		value = 1;
		return (value);
	}
	while (envp2[i])
	{
		printf("%s\n", envp2[i]);
		i++;
	}
	value = 0;
	return (value);
}

int cmd_cd(char **tab)
{
	if (tab[2])
	{
		printf ("bash: too many arguments\n");
		value = 1;
		return (value);
	}
	if (chdir(tab[1]))
	{
		perror("Error cd");
		value = 1;
		return (value);
	}
	value = 0;
	return (value);
}

int cmd_pwd()
{
	char *buf;

	buf = malloc(sizeof(char) * 999);
	if (buf == 0)
	{
		perror("Erreur pwd");
		value = 1;
		return (value);
	}
	printf("%s\n", getcwd(buf, 1000));
	free(buf);
	value = 0;
	return (value);
}

int cmd_echo(char **envp2, char **tab)
{
	int i = 1;
	char *res;

	if (tab[1] && str_n_cmp(tab[1], "$?", 3) == 0)
	{
		printf("%d\n", value);
		value = 0;
		return (value);
	}
	if (tab[1] && str_n_cmp(tab[1], "-n", 3) == 0)
		i = 2;

	/* en cas de variables $
	res = find_var(envp2, "TERM");
	printf("res = %s\n", res);*/

	while (tab[i])
	{
		write(1, tab[i], str_len(tab[i]));
		if (tab[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (tab[1] && str_n_cmp(tab[1], "-n", 3))
		write(1, "\n", 1);
	value = 0;
	return (value);
}
