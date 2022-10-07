/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   case_parse_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 20:54:09 by gmillon           #+#    #+#             */
/*   Updated: 2022/10/07 22:43:59 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*cas_pipe(char *input, t_command *command)
{
	input++;
	input = forward_space(input);
	if (slen(input) == 0)
		return (0);
	command->pipe_flag_out = 1;
	return (input);
}

char	*cas_char(char *input, t_command *command, t_env *env)
{
	int		i;
	char	*temp;
	char	*dup;

	i = 0;
	while (input[i] && maybe_char(input[i]))
		i++;
	dup = str_n_dup(input, i);
	dup = replace_variable(dup, env);
	temp = str_join(command->options, dup);
	command->options = temp;
	if (input[i] != '\'' && input[i] != '\"')
		command->options = str_join(command->options, ft_strdup(" "));
	input = input + i;
	if (slen(input) == 0)
		return (0);
	return (input);
}
