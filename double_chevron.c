/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_chevron.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 20:41:26 by gmillon           #+#    #+#             */
/*   Updated: 2022/10/11 00:17:12 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./libftextended/headers/libft.h"
#include "minishell.h"
#include <fcntl.h>

char	*update_heredoc(int heredoc, char *heredoc_input)
{
	write(heredoc, heredoc_input, slen(heredoc_input));
	free(heredoc_input);
	write(heredoc, "\n", 1);
	heredoc_input = readline("heredoc> ");
	return (heredoc_input);
}

char	*cas_heredoc(char *input, t_command *command)
{
	const int	heredoc = open(".heredoc", O_CREAT | O_WRONLY | O_TRUNC, 0640);
	char		*heredoc_input;
	int			i;
	char		*terminator;	

	i = 0;
	input = forward_space(input + 2);
	command->file_in = ".heredoc";
	if (command->fd_file_in)
		close(command->fd_file_in);
	while (input[i] && maybe_char(input[i]))
		i++;
	terminator = str_n_dup(input, i);
	heredoc_input = readline("heredoc> ");
	while (heredoc_input && \
			str_n_cmp(heredoc_input, terminator, slen(terminator) + 1))
		heredoc_input = update_heredoc(heredoc, heredoc_input);
	free(heredoc_input);
	close(heredoc);
	command->fd_file_in = open(".heredoc", O_RDONLY);
	command->fd_in = command->fd_file_in;
	free(terminator);
	if (slen(input + i) == 0)
		return (0);
	return (input + i);
}

char	*cas_append(char *input, t_command *command, t_env *env)
{
	int	i;

	input += 2;
	input = forward_space(input);
	i = 0;
	while (input[i] && maybe_char(input[i]))
		i++;
	command->file_out = str_n_dup(input, i);
	command->fd_file_out = \
		open(command->file_out, O_CREAT | O_WRONLY | O_APPEND);
	if (command->fd_file_out == -1)
	{
		printf("%s: Open file error\n", command->file_out);
		env->value = 1;
		env->stop = 1;
		free(command->file_out);
		return (0);
	}
	command->fd_out = command->fd_file_out;
	input = input + i;
	if (slen(input) == 0)
		return (0);
	return (input);
}
