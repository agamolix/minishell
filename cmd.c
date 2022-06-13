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
	
	//verifier les erreurs et les codes de retour
//	exit(tab[1]);

}

int cmd_env(char **envp2, char **tab)
{
	int i = 0;

	if (tab[1])
	{
		printf ("Too many arguments\n");
		return (1);
	}
	while (envp2[i])
	{
		printf("%s\n", envp2[i]);
		i++;
	}
	return 0;
}

int cmd_cd(char **tab)
{
	if (tab[2])
	{
		printf ("Too many arguments\n");
		return (1);
	}
	if (chdir(tab[1]))
	{
		perror("Erreur cd");
	}
	return 0;
}

int cmd_pwd()
{
	char *buf;

	buf = malloc(sizeof(char) * 999);
	if (buf == 0)
	{
		perror("Erreur pwd");
	}
	printf("%s\n", getcwd(buf, 1000));
	free(buf);

	return 0;
}

int cmd_echo(char **tab)
{
	int i = 1;

	if (tab[1] && str_n_cmp(tab[1], "-n", 3) == 0)
		i = 2;
	while (tab[i])
	{
		write(1, tab[i], str_len(tab[i]));
		if (tab[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (tab[1] && str_n_cmp(tab[1], "-n", 3))
		write(1, "\n", 1);
	return 0;
}
