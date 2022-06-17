/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atrilles <atrilles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 17:40:41 by atrilles          #+#    #+#             */
/*   Updated: 2022/04/22 17:41:03 by atrilles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>


char **copy_envp(char **envp)
{
	char **res;
	char *temp;
	int i = 0;

	while(envp[i])
		i++;
	res = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while(envp[i])
	{
		res[i] = str_dup(envp[i]);
		i++;
	}
	res[i] = 0;
	return res;
}

char *verify_chev(char *input)
{
	char *res;
	int i;
	int j;

	res = malloc(sizeof(char) * str_len(input) * 2);
	i = 0;
	j = 0;
	while(input[i])
	{
		if (input[i] == '>' || input[i] == '<')
		{
			res[j] = ' ';
			res[j + 1] = input[i];
			res[j + 2] = ' ';
			j = j + 2;
		}
		else 
			res[j] = input[i];
		i++;
		j++;
	}
	res[j] = 0;
	free(input);
	return(res);
}

char *forward_space(char *input)
{
	while(input[0] == ' ')
		input++;
	return (input);
}

int maybe_char(char c)
{
	if (c != ' ' && c != '|' && c != '"' 
	&& c != '\'' && c != '<' && c != '>') 
		return (1);
	return (0);
}

char *cas_chevron_in(char *input, t_command *command)
{
	int i;

	input = forward_space(input);
	i = 0;
	while(input[i])
	{
		if (maybe_char(input[i]))
			i++;
		else 
			break;
	}


	command->file_in = str_n_dup(input, i);
//	printf("file_in = <%s>\n", command->file_in);
	command->fd_in = open(command->file_in, O_RDONLY);
	if (command->fd_in == -1) 
	{
		printf("bash: %s:No file or directory\n", command->file_in);
		value = 1;
		exit(value);
	}
	if (dup2(command->fd_in, 0) == -1)
	{
		printf("Dup2 file_in (stdin): unexpected error\n");
		value = 1;
		exit(value);
	}
	input = input + i;
	if (str_len(input) == 0)
		return(0);
	return(input);
}

char *cas_chevron_out(char *input, t_command *command)
{
	int i;

	input = forward_space(input);
	i = 0;
	while(input[i])
	{
		if (maybe_char(input[i]))
			i++;
		else 
			break;
	}
	command->file_out = str_n_dup(input, i);
	printf("file_out = <%s>\n", command->file_out);
	input = input + i;
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

char *replace_variable(char *temp, t_command *command, char **envp2)
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
			var = find_free_var(envp2, var);
			temp = insert_variable(temp, i - j, i, var);
			i = -1;
			j = 0;
		}
		i++;
	}
	return (temp);
}

char *cas_double_quote(char *input, t_command *command, char **envp2)
{
	int i;
	char *temp;

	i = 0;
	while(input[i])
	{
		if (input[i] != '"')
			i++;
		else 
			break;
	}
	if (!input[i])
	{
		printf("Error: no end double quote\n");
		return(0);
	}
	temp = str_n_dup(input, i);
	temp = replace_variable(temp, command, envp2);
	temp = str_join(command->options, temp);
	command->options = str_join(temp, " ");
	free(temp);
	printf("options = <%s>\n", command->options);

//	command->double_quote = str_n_dup(input, i);
//	printf("double_quote = <%s>\n", command->double_quote);
	input = input + i;
	if (str_len(input) == 1)
		return (0);
	return (input + 1);
}


char *cas_single_quote(char *input, t_command *command)
{
	int i;
	char *temp;

	i = 0;
	while(input[i])
	{
		if (input[i] != '\'')
			i++;
		else 
			break;
	}
	if (!input[i])
	{
		printf("Error: no end single quote\n");
		return(0);
	}
	temp = str_n_dup(input, i);
	temp = str_join(command->options, temp);
	command->options = str_join(temp, " ");
	free(temp);
	printf("options = <%s>\n", command->options);

//	command->single_quote = str_n_dup(input, i);
//	printf("single_quote = <%s>\n", command->single_quote);
	input = input + i;
	if (str_len(input) == 1)
		return (0);
	return (input + 1);
}

char *cas_char(char *input, t_command *command, char **envp2)
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
	temp = replace_variable(temp, command, envp2);
	temp = str_join(command->options, temp);
	command->options = str_join(temp, " ");
	free(temp);
	printf("options = <%s>\n", command->options);
	input = input + i;
	if (str_len(input) == 0)
		return(0);
	return(input);
}

int init(t_command *command)
{
	command->options = 0;
	command->file_in = 0;
	command->file_out = 0;
	return 0;
}

int execute(int argc, char **argv, char **envp2, t_command *command)
{
	char *input;
	char **tab;
	int i;

	tab = my_split(command->options, ' ');
	i = 0;	
	if (str_n_cmp(tab[0], "echo", 5) == 0)
	{
		printf ("cmd echo\n");
		cmd_echo(envp2, tab);
	}
	else if (str_n_cmp(tab[0], "cd", 3) == 0)
	{
		printf ("cmd cd\n");
		cmd_cd(tab);
	}
	else if (str_n_cmp(tab[0], "pwd", 4) == 0)
	{
		printf ("cmd pwd\n");
		cmd_pwd();
	}
	else if (str_n_cmp(tab[0], "export", 7) == 0)
	{
		printf ("cmd export\n");
		envp2 = cmd_export(envp2, tab);
	}
	else if (str_n_cmp(tab[0], "unset", 6) == 0)
	{
		printf ("cmd unset\n");
		envp2 = cmd_unset(envp2, tab);
	}
	else if (str_n_cmp(tab[0], "env", 4) == 0)
	{
		printf ("cmd env\n");
		cmd_env(envp2, tab);
	}
	else if (str_n_cmp(tab[0], "exit", 5) == 0)
	{
		printf ("cmd exit\n");
		cmd_exit(tab);
	}
	else
	{
		printf ("cmd \n");
		do_command(tab, envp2);
	}
	printf("fin execute\n");
	init(command);
	return 0;
}

int parse(int argc, char **argv, char **envp2, t_command *command)
{
	char *input;
	char **tab;
	int i;

	printf("debut parse\n");

	input = readline("$> ");
	add_history(input);
	while (input)
	{
		input = forward_space(input);
		if (input[0] == '<')
		{
//			printf("cas chevron_in <\n");
			input++;
			input = cas_chevron_in(input, command);
//			printf("input = %s\n", input);
		}
		else if (input[0] == '>')
		{
//			printf("cas chevron_out <\n");
			input++;
			input = cas_chevron_out(input, command);
//			printf("input = %s\n", input);
		}
		else if (input[0] == '"')
		{
//			printf("cas double quote\n");
			input++;
			input = cas_double_quote(input, command, envp2);
//			printf("input = %s\n", input);
		}
		else if (input[0] == '\'')
		{
//			printf("cas single quote\n");
			input++;
			input = cas_single_quote(input, command);
//			printf("input = %s\n", input);
		}
		else if (input[0] == '|')
		{
//			printf("cas pipe\n");
			execute(argc, argv, envp2, command);	
			parse(argc, argv, envp2, command);	
		}		
		else
		{
//			printf("cas char\n");
			input = cas_char(input, command, envp2);
//			printf("input = %s\n", input);
		}
	}
	printf("final command = %s\n", command->options);
	printf("final file_in = %s\n", command->file_in);
	printf("final file_out = %s\n", command->file_out);
	
	execute(argc, argv, envp2, command);
	printf("fin execute dans parse\n");
	parse(argc, argv, envp2, command);	

	return 0;
}


int main(int argc, char **argv, char **envp)
{
	char **envp2;
	t_command command;

	init(&command);
	value = 0;
	envp2 = copy_envp(envp);

	parse(argc, argv, envp2, &command);
	return 0;
}
