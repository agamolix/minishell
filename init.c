/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 19:35:18 by atrilles          #+#    #+#             */
/*   Updated: 2022/10/07 23:38:33 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init(t_command *command, int first)
{
	ft_printf("\n");
	if (command->options && (!first))
		free(command->options);
	if (command->file_out && (!first))
		free(command->file_out);
	// if (command->file_in && (!first))
	// 	free(command->file_in);
	command->options = 0;
	command->fd_file_in = 0;
	command->file_in = 0;
	command->fd_file_out = 0;
	command->file_out = 0;
	command->pipe_flag_in = 0;
	command->pipe_flag_out = 0;
	command->fd_pipe_in = 0;
	command->fd_pipe_in_next = 0;
	command->fd_pipe_out = 0;
	command->fd_in = 0;
	command->fd_out = 1;
	return (0);
}

int	init_cmd(t_command *command)
{
	if (command->options)
		free(command->options);
	if (command->file_out)
		free(command->file_out);
	// if (command->file_in)
	// 	free(command->file_in);
	command->options = 0;
	command->fd_file_in = 0;
	command->file_in = 0;
	command->fd_file_out = 0;
	command->file_out = 0;
	return (0);
}
