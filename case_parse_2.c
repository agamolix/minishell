/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   case_parse_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 20:54:09 by gmillon           #+#    #+#             */
/*   Updated: 2022/10/11 00:22:24 by gmillon          ###   ########.fr       */
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
	command->options = replace_wildcards(command->options);
	input = input + i;
	if (slen(input) == 0)
		return (0);
	return (input);
}

t_command	*parse_input(char *input, t_vars vars, char *free_ptr)
{
	while (input)
	{
		input = forward_space(input);
		input = check_special_chars(input, vars.command, vars.env);
		if (input && input[0] == '|')
		{
			input = cas_pipe(input, vars.command);
			if (vars.command->options \
					&& vars.command->options[0] != ' ' && vars.env->stop == 0)
				g_env.pid = execute(vars.env, vars.command);
			else
			{
				printf("Error pipe: stop pipe\n");
				init(vars.command, 0);
				free(free_ptr);
				parse_loop(vars.argc, vars.argv, vars.env, vars.command);
			}
			pipe_var_set(vars.command);
		}		
		else if (input)
			input = cas_char(input, vars.command, vars.env);
	}
	return (vars.command);
}

t_command	*make_command(t_vars vars)
{
	char	*input;
	char	*free_ptr;

	input = readline("$> ");
	free_ptr = input;
	if (!input)
		exit(0);
	add_history(input);
	vars.command = parse_input(input, vars, free_ptr);
	free(free_ptr);
	return (vars.command);
}

int	parse_loop(int argc, char **argv, t_env *env, t_command *command)
{
	pid_t	lastpid;

	lastpid = -100;
	command = make_command(make_var_struct(argc, argv, env, command));
	if (env->stop == 0)
		if (command->options && command->options[0] != ' ')
			lastpid = execute(env, command);
	await_child_loop(env, lastpid);
	init(command, 0);
	env->stop = 0;
	env->pid = 0;
	if (!env->stop)
		parse_loop(argc, argv, env, command);
	return (0);
}
