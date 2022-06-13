/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atrilles <atrilles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 17:40:41 by atrilles          #+#    #+#             */
/*   Updated: 2022/04/22 17:41:03 by atrilles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **copy_envp(char **envp)
{
	char **res;
	char *temp;
	int i = 0;

	while(envp[i])
		i++;
	res = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while(envp[i])
	{
		res[i] = str_dup(envp[i]);
		i++;
	}
	res[i] = 0;
	return res;
}

int boucle(int argc, char **argv, char **envp2)
{
	char *input;
	char **tab;

	input = readline("$> ");
	add_history(input);
	tab = my_split(input, ' ');
	int i = 0;
	if (str_n_cmp(tab[0], "echo", 5) == 0)
	{
		printf ("cmd echo\n");
		cmd_echo(tab);
	}
	else if (str_n_cmp(tab[0], "cd", 3) == 0)
	{
		printf ("cmd cd\n");
		cmd_cd(tab);
	}
	else if (str_n_cmp(tab[0], "pwd", 4) == 0)
	{
		printf ("cmd pwd\n");
		cmd_pwd();
	}
	else if (str_n_cmp(tab[0], "export", 7) == 0)
	{
		printf ("cmd export\n");
		envp2 = cmd_export(envp2, tab);
	}
	else if (str_n_cmp(tab[0], "unset", 6) == 0)
	{
		printf ("cmd unset\n");
		envp2 = cmd_unset(envp2, tab);
	}
	else if (str_n_cmp(tab[0], "env", 4) == 0)
	{
		printf ("cmd env\n");
		cmd_env(envp2, tab);
	}
	else if (str_n_cmp(tab[0], "exit", 5) == 0)
	{
		printf ("cmd exit\n");
		cmd_exit(tab);
	}
	else
{
			printf ("cmd \n");
		command(tab, envp2);
}
	boucle(argc, argv, envp2);
	return 0;
}

int main(int argc, char **argv, char **envp)
{
	char **envp2;

	value = 0;
	envp2 = copy_envp(envp);
	boucle(argc, argv, envp2);
	return 0;
}
