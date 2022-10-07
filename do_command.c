/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 17:40:41 by atrilles          #+#    #+#             */
/*   Updated: 2022/10/07 23:53:06 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_fd_dup(t_command *command, t_env *env)
{
	if (command->fd_in)
	{
		if (dup2(command->fd_in, 0) == -1)
		{
			printf("Dup2 fd_in (stdin): unexpected error\n");
			env->value = 1;
			env->stop = 1;
			return (0);
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
			return (0);
		}
		close(command->fd_out);
	}
	return (1);
}

void	check_if_builtin(char **tab, t_env *env, t_command *command)
{
	if (str_n_cmp(tab[0], "echo", 5) == 0)
	{
		cmd_echo(tab, env, command);
		if (env->value == 1)
			exit(EXIT_FAILURE);
		exit(EXIT_SUCCESS);
	}
	else if (str_n_cmp(tab[0], "pwd", 4) == 0)
	{
		cmd_pwd(env);
		if (env->value == 1)
			exit(EXIT_FAILURE);
		exit(EXIT_SUCCESS);
	}
	if (str_n_cmp(tab[0], "env", 4) == 0)
	{
		cmd_env(env, tab);
		if (env->value == 1)
			exit(EXIT_FAILURE);
		exit(EXIT_SUCCESS);
	}
}

void	close_fds(t_command *command)
{
	if (command->fd_in)
		close(command->fd_in);
	if (command->fd_out != 1)
		close(command->fd_out);
}

int	do_command(char **tab, t_env *env, t_command *command)
{
	pid_t	pid;
	int		status;
	char	*path;

	pid = fork();
	if (pid == -1)
	{
		perror("Fork error");
		env->value = 1;
		env->stop = 1;
		return (env->value);
	}
	else if (pid == 0)
	{
		if (!handle_fd_dup(command, env))
			return (env->value);
		if (command->fd_pipe_in_next)
			close(command->fd_pipe_in_next);
		check_if_builtin(tab, env, command);
		path = (find_path(env, tab[0]));
		if (path == 0)
			exit(127);
		execve(path, tab, NULL);
	}
	close_fds(command);
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
	return (pid);
}
