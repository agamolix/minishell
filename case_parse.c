/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   case_parse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 19:35:18 by atrilles          #+#    #+#             */
/*   Updated: 2022/10/10 01:17:55gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>

char	*cas_chevron_in(char *input, t_command *command, t_env *env)
{
	int	i;

	input = forward_space(input + 1);
	i = 0;
	while (input[i] && maybe_char(input[i]))
		i++;
	command->file_in = str_n_dup(input, i);
	if (command->fd_file_in)
		close(command->fd_file_in);
	command->fd_file_in = open(command->file_in, O_RDONLY);
	if (command->fd_file_in == -1)
	{
		printf("%s: No file or directory\n", command->file_in);
		env->value = 1;
		env->stop = 1;
		return (0);
	}
	command->fd_in = command->fd_file_in;
	input = input + i;
	if (slen(input) == 0)
		return (0);
	return (input);
}

char	*cas_chevron_out(char *input, t_command *command, t_env *env)
{
	int	i;

	input = forward_space(input + 1);
	i = 0;
	while (input[i] && maybe_char(input[i]))
		i++;
	command->file_out = str_n_dup(input, i);
	command->fd_file_out = \
		open(command->file_out, O_CREAT | O_WRONLY | O_TRUNC, 0640);
	if (command->fd_file_out == -1)
	{
		printf("%s: Open file error\n", command->file_out);
		env->value = 1;
		env->stop = 1;
		return (0);
	}
	command->fd_out = command->fd_file_out;
	input = input + i;
	if (slen(input) == 0)
		return (0);
	return (input);
}

char	*cas_double_quote(char *input, t_command *command, t_env *env)
{
	int		i;
	char	*temp;

	i = 1;
	while (input[i] && input[i] != '"')
		i++;
	if (!input[i])
	{
		printf("Input error: no end double quote\n");
		env->value = 1;
		env->stop = 1;
		return (0);
	}
	temp = str_n_dup(input, i + 1);
	temp = replace_variable(temp, env);
	temp = str_join(command->options, temp);
	command->options = temp;
	if (input[i + 1] && input[i + 1] == ' ')
		command->options = str_join(command->options, ft_strdup(" "));
	input = input + i;
	if (slen(input) == 1)
		return (0);
	return (input + 1);
}

char	*cas_single_quote(char *input, t_command *command, t_env *env)
{
	int		i;
	char	*temp;

	i = 1;
	while (input[i] && input[i] != '\'')
		i++;
	if (!input[i])
	{
		printf("Input error: no end single quote\n");
		env->value = 1;
		env->stop = 1;
		return (0);
	}
	temp = str_n_dup(input, i + 1);
	temp = str_join(command->options, temp);
	command->options = temp;
	if (input[i + 1] && input[i + 1] == ' ')
		command->options = str_join(command->options, ft_strdup(" "));
	input = input + i;
	if (slen(input) == 1)
		return (0);
	return (input + 1);
}
