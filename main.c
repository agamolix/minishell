/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 17:40:41 by atrilles          #+#    #+#             */
/*   Updated: 2022/10/10 23:54:26 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	g_env = {0, 0, 0, 0, 0};

char	**copy_env(char **envp)
{
	char	**res;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	res = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (envp[i])
	{
		res[i] = str_dup(envp[i]);
		i++;
	}
	res[i] = 0;
	return (res);
}

int	execute(t_env *env, t_command *command)
{
	char	**tab;
	int		pid;

	pid = -1;
	if (!handle_pipes(command, env))
		return (env->value);
	tab = my_split(command->options, ' ');
	if (str_n_cmp(tab[0], "cd", 3) == 0)
		cmd_cd(tab, env);
	else if (str_n_cmp(tab[0], "export", 7) == 0)
		cmd_export(env, tab);
	else if (str_n_cmp(tab[0], "unset", 6) == 0)
		cmd_unset(env, tab);
	else if (str_n_cmp(tab[0], "exit", 5) == 0)
	{
		free(command->options);
		cmd_exit(tab, env);
	}
	else
		pid = do_command(tab, env, command);
	init_cmd(command);
	free_split(tab);
	return (pid);
}

int	main(int argc, char **argv, char **envp)
{
	t_env				myenv;
	t_command			mycommand;

	signal(SIGINT, handle_ctrl_c);
	signal(SIGQUIT, handle_ctrl_d);
	init(&mycommand, 1);
	g_env.env = copy_env(envp);
	parse_loop(argc, argv, &g_env, &mycommand);
	return (0);
}
