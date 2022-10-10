/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   case_parse3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 23:51:36 by gmillon           #+#    #+#             */
/*   Updated: 2022/10/11 00:17:22 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*check_special_chars(char *input, t_command *command, t_env *env)
{
	if (!str_n_cmp(input, ">>", 2))
		return (cas_append(input, command, env));
	else if (!str_n_cmp(input, "<<", 2))
		return (cas_heredoc(input, command));
	else if (input[0] == '<')
		return (cas_chevron_in(input, command, env));
	else if (input[0] == '>')
		return (cas_chevron_out(input, command, env));
	else if (input[0] == '"')
		return (cas_double_quote(input, command, env));
	else if (input[0] == '\'')
		return (cas_single_quote(input, command, env));
	return (input);
}

void	handle_pipe_flag_in(t_command *command, t_env *env)
{
	(void) env;
	command->fd_in = command->fd_pipe_in;
	if (command->fd_file_in)
	{
		close(command->fd_in);
		command->fd_in = command->fd_file_in;
	}
}

int	handle_pipes(t_command *command, t_env *env)
{
	int		my_pipe[2];

	if (command->pipe_flag_in)
		handle_pipe_flag_in(command, env);
	if (command->pipe_flag_out)
	{
		if (pipe(my_pipe) == -1)
		{
			printf("Pipe error\n");
			env->value = 1;
			env->stop = 1;
			return (0);
		}
		command->fd_pipe_in_next = my_pipe[0];
		command->fd_pipe_out = my_pipe[1];
		command->fd_out = command->fd_pipe_out;
		if (command->fd_file_out)
		{
			close(command->fd_out);
			command->fd_out = command->fd_file_out;
		}
	}
	return (1);
}
