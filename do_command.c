/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 17:40:41 by atrilles          #+#    #+#             */
/*   Updated: 2022/10/06 23:30:53 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	do_command(char **tab, t_env *env, t_command *command)
{
	pid_t	pid;
	int		status;
	char	*path;

	pid = fork();
	if (pid == -1) //echec fork
	{
		perror("Fork error");
		env->value = 1;
		env->stop = 1;
		return(env->value);
	}
	else if (pid == 0) //processus fils
	{
		if (command->fd_in)
		{
			if (dup2(command->fd_in, 0) == -1)
			{
				printf("Dup2 fd_in (stdin): unexpected error\n");
				env->value = 1;
				env->stop = 1;
				return(env->value);
			}
			close(command->fd_in);
		}
		if (command->fd_out != 1)
		{
			if (dup2(command->fd_out, 1) == -1)
			{
				printf("Dup2 fd_out (stdout): unexpected error\n");
				env->value = 1;
				env->stop = 1;
				return(env->value);
			}
			close(command->fd_out);
		}
		if (command->fd_pipe_in_next) close(command->fd_pipe_in_next);

		if (str_n_cmp(tab[0], "echo", 5) == 0)
		{
			cmd_echo(tab, env, command);
			if (env->value == 1) exit(EXIT_FAILURE);
			exit(EXIT_SUCCESS);
		}
		else if (str_n_cmp(tab[0], "pwd", 4) == 0)
		{
			cmd_pwd(env);
			if (env->value == 1) exit(EXIT_FAILURE);
			exit(EXIT_SUCCESS);
		}
		if (str_n_cmp(tab[0], "env", 4) == 0)
		{
			cmd_env(env, tab);
			if (env->value == 1) exit(EXIT_FAILURE);
			exit(EXIT_SUCCESS);
		}
		else
		{
			path = (find_path(env, tab[0]));
//			printf("options = %s\n", command->options);
			if (path == 0)
				exit(127);
//			printf("path: %s\n", path);
			execve(path, tab, NULL);
		}
	}
	else
	{

	}

	if (command->fd_in) close(command->fd_in);
	if (command->fd_out != 1) close(command->fd_out);
/*
	else //processus père
	{
		if (command->fd_in) close(command->fd_in);
		if (command->fd_out != 1) close(command->fd_out);
		wait(&status);
		env->value = WEXITSTATUS(status);
		if (env->value)
			env->stop = 1;
		kill(pid, SIGTERM);
	}
*/
	return pid;
}
