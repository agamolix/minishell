/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 17:40:41 by atrilles          #+#    #+#             */
/*   Updated: 2022/07/13 02:38:54 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **copy_env(char **envp)
{
	char **res;
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

int execute(t_env *env, t_command *command)
{
	char **tab;
	int my_pipe[2];

	if (command->pipe_flag_in)
		command->fd_in = command->fd_pipe_in;
	if (command->pipe_flag_out)
	{
		if (pipe(my_pipe) == -1)
		{
			printf("Pipe error\n");
			env->value = 1;
			env->stop = 1;
			return (env->value);
		}
		command->fd_pipe_in_next = my_pipe[0]; //8
		command->fd_pipe_out = my_pipe[1];  //7
		command->fd_out = command->fd_pipe_out;
	}

	tab = my_split(command->options, ' ');
	if (str_n_cmp(tab[0], "cd", 3) == 0)
		cmd_cd(tab, env);
	else if (str_n_cmp(tab[0], "export", 7) == 0)
		cmd_export(env, tab);
	else if (str_n_cmp(tab[0], "unset", 6) == 0)
		cmd_unset(env, tab);
	else if (str_n_cmp(tab[0], "exit", 5) == 0)
		cmd_exit(tab, env);
	else
		do_command(tab, env, command);
	init_cmd(command);
	return 0;
}

// ^D displays on ctrl-D -> needs to be fixed
int parse(int argc, char **argv, t_env *env, t_command *command)
{
	char *input;

	input = readline("$> ");
	if (!input)
		exit(0);
	add_history(input);
	while (input)
	{
		input = forward_space(input);
		if (input[0] == '<')
			input = cas_chevron_in(input, command, env);
		else if (input[0] == '>')
			input = cas_chevron_out(input, command, env);
		else if (input[0] == '"')
			input = cas_double_quote(input, command, env);
		else if (input[0] == '\'')
			input = cas_single_quote(input, command, env);
		else if (input[0] == '|')
		{
			input = cas_pipe(input);
			command->pipe_flag_out = 1;
			if (command->options && command->options[0]!= ' ' && env->stop == 0)
				execute(env, command);
			else
			{
				printf("Error pipe: stop pipe\n");
				init(command);
				parse(argc, argv, env, command);	
			}
			command->pipe_flag_out = 0;
			command->pipe_flag_in = 1;
			command->fd_pipe_in = command->fd_pipe_in_next;
			command->fd_out = 1;
		}		
		else
			input = cas_char(input, command, env);
	}
//	printf("final command = <%s>\n", command->options);
//	printf("value = %d\n", env->value);
//	printf("stop = %d\n", env->stop);

	if(env->stop == 0)
	{
		if (command->options && command->options[0]!= ' ')
			execute(env, command);
		else
			printf("Error: empty command\n");
	}
	init(command);
	env->stop = 0;
	parse(argc, argv, env, command);	
	return 0;
}

// Probably need to kill processes if signals are used to interrupt ongoing command
void	signal_handler(int sig_num, siginfo_t *info, void *parser_vars)
{
	(void)sig_num;
	t_parse_vars	*cast_vars;

	cast_vars = (t_parse_vars *)parser_vars;

	init(cast_vars->mycommand);
	cast_vars->myenv->stop = 0;
	parse(cast_vars->argc, cast_vars->argv, cast_vars->myenv, cast_vars->mycommand);
}
//Signal handling approach doesn't work: ignoe sigint and instead parse it in the parserfunction
int main(int argc, char **argv, char **envp)
{
	t_env				myenv;
	t_command			mycommand;
	struct sigaction	newaction;
	const t_parse_vars	parser_vars = {.argc = argc, .argv = argv,
						.myenv = &myenv, .mycommand = &mycommand};
	
	// newaction.sa_sigaction = &signal_handler;
	// sigaction(SIGINT, &newaction, &parser_vars);

	init(&mycommand);
	myenv.value = 0;
	myenv.stop = 0;
	myenv.env = copy_env(envp);

	parse(argc, argv, &myenv, &mycommand);
	return 0;
}
