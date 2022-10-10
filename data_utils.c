/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/20 16:54:13 by gmillon           #+#    #+#             */
/*   Updated: 2022/10/10 23:45:41 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_split(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free (arr);
}

void	await_child_loop(t_env *env, pid_t lastpid)
{
	pid_t	pid;
	int		status;

	while (1)
	{
		pid = wait(&status);
		if (env->received_sig)
		{
			env->received_sig = 0;
			break ;
		}
		if (pid == lastpid)
			env->value = WEXITSTATUS(status);
		if (pid < 0)
			break ;
	}
}

t_vars	make_var_struct(int argc, char **argv, t_env *env, t_command *command)
{
	t_vars	vars;

	vars.argc = argc;
	vars.argv = argv;
	vars.env = env;
	vars.command = command;
	return (vars);
}

void	pipe_var_set(t_command *command)
{
	command->pipe_flag_out = 0;
	command->pipe_flag_in = 1;
	command->fd_pipe_in = command->fd_pipe_in_next;
	command->fd_out = 1;
}
