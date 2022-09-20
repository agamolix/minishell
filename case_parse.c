/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   case_parse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 19:35:18 by atrilles          #+#    #+#             */
/*   Updated: 2022/07/23 00:33:49 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>

char *forward_space(char *input)
{
	while(input[0] == ' ' || input[0] == '\t')
	{
		input++;
	}
	// printf("%s\n", input);
	return (input);
}

int maybe_char(char c)
{
	if (c != ' ' && c != '|' && c != '"' 
	&& c != '\'' && c != '<' && c != '>') 
		return (1);
	return (0);
}

char *cas_chevron_in(char *input, t_command *command, t_env *env)
{
	int i;

	// printf("inputL: %s\n", input);
	input++;
	input = forward_space(input);
	// printf("inputL: %c\n", input[0]);
	i = 0;
	while (input[i] && maybe_char(input[i]))
		i++;
	// while(input[i])
	// {
	// 	if (maybe_char(input[i]))
	// 		i++;
	// 	else 
	// 		break;
	// }
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
	ft_printf("inputL:_%s\n", input);
	input = input + i;
	printf("ass%d\n", i);
	if (str_len(input) == 0)
		return(0);
	// print
	ft_printf("inputL:_%s\n", input);
	return(input);
}

char *cas_chevron_out(char *input, t_command *command, t_env *env)
{
	int i;

	input++;
	input = forward_space(input);
	i = 0;
	while (maybe_char(input[i]))
		i++;
	command->file_out = str_n_dup(input, i);
	command->fd_file_out = open(command->file_out, O_CREAT | O_WRONLY | O_TRUNC, 0640);
	if (command->fd_file_out == -1)
	{
		printf("%s: Open file error\n", command->file_out);
		env->value = 1;
		env->stop = 1;
		return(0);
	}
	command->fd_out = command->fd_file_out;
	input = input + i;
	if (str_len(input) == 0)
		return(0);
	return(input);
}

char *cas_pipe(char *input)
{
	input++;
	input = forward_space(input);
	if (str_len(input) == 0)
		return(0);
	return(input);
}

char *insert_variable(char *str, int index1, int index2, char *val)
{
	char *part1;
	char *part2;
	char *temp;
	char *res;

	part1 = str_n_dup(str, index1 - 1);
	part2 = str_n_dup_start(str, str_len(str) - index2, index2);
	if (val == 0)
		res = str_join(part1, part2);
	else
	{
		temp = str_join(part1, val);
		res = str_join(temp, part2);
		free(temp);
	}
	free(part1);
	free(part2);
	free(str);
	return (res);
}

char *replace_variable(char *temp, t_env *env)
{
	int i;
	int j;
	char *var;

	i = 0;
	j = 0;
	while(temp[i])
	{
		if (temp[i] == '$')
		{
			i++;
			while(temp[i] && maybe_char(temp[i]))
			{	
				j++;
				i++;
			}
			var = str_n_dup_start(temp, j, i - j);
			var = find_free_var(env, var);
			temp = insert_variable(temp, i - j, i, var);
			i = -1;
			j = 0;
		}
		i++;
	}
	return (temp);
}

char *cas_double_quote(char *input, t_command *command, t_env *env)
{
	int i;
	char *temp;

	i = 0;
	input++;
	while(input[i])
	{
		if (input[i] != '"')
			i++;
		else 
			break;
	}
	if (!input[i])
	{
		printf("Input error: no end double quote\n");
		env->value = 1;
		env->stop = 1;
		return(0);
	}
	temp = str_n_dup(input, i);
	temp = replace_variable(temp, env);
	temp = str_join(command->options, temp);
	command->options = str_join(temp, " ");
	free(temp);
	input = input + i;
	if (str_len(input) == 1)
		return (0);
	return (input + 1);
}


char *cas_single_quote(char *input, t_command *command, t_env *env)
{
	int i;
	char *temp;

	i = 0;
	input++;
	while(input[i])
	{
		if (input[i] != '\'')
			i++;
		else 
			break;
	}
	if (!input[i])
	{
		printf("Input error: no end single quote\n");
		env->value = 1;
		env->stop = 1;
		return(0);
	}
	temp = str_n_dup(input, i);
	temp = str_join(command->options, temp);
	command->options = str_join(temp, " ");
	free(temp);
	input = input + i;
	if (str_len(input) == 1)
		return (0);
	return (input + 1);
}

char *cas_char(char *input, t_command *command, t_env *env)
{
	int i;
	char *temp;

	i = 0;
	while(input[i])
	{
		if (maybe_char(input[i]))
			i++;
		else 
			break;
	}
	temp = str_n_dup(input, i);
	temp = replace_variable(temp, env);
	temp = str_join(command->options, temp);
	command->options = str_join(temp, " ");

	free(temp);
	input = input + i;
	if (str_len(input) == 0)
		return(0);
		
	return(input);
}

