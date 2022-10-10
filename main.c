/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 17:40:41 by atrilles          #+#    #+#             */
/*   Updated: 2022/10/10 02:17:24 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env g_env = {0, 0, 0, 0, 0};

char	**copy_env(char **envp)
{
	char	**res;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	res = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (envp[i])
	{
		res[i] = str_dup(envp[i]);
		i++;
	}
	res[i] = 0;
	return (res);
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

int	execute(t_env *env, t_command *command)
{
	char	**tab;
	int		pid;

	pid = -1;
	if (!handle_pipes(command, env))
		return (env->value);
	tab = my_split(command->options, ' ');
	if (str_n_cmp(tab[0], "cd", 3) == 0)
		cmd_cd(tab, env);
	else if (str_n_cmp(tab[0], "export", 7) == 0)
		cmd_export(env, tab);
	else if (str_n_cmp(tab[0], "unset", 6) == 0)
		cmd_unset(env, tab);
	else if (str_n_cmp(tab[0], "exit", 5) == 0)
	{
		free(command->options);
		cmd_exit(tab, env);
	}
	else
		pid = do_command(tab, env, command);
	init_cmd(command);
	free_split(tab);
	return (pid);
}

// ^D displays on ctrl-D -> needs to be fixed

char	*check_special_chars(char *input, t_command *command, t_env *env)
{
	if (!str_n_cmp(input, ">>", 2))
		return (cas_append(input, command, env));
	else if (!str_n_cmp(input, "<<", 2))
		return (cas_heredoc(input, command, env));
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

void	pipe_var_set(t_command *command)
{
	command->pipe_flag_out = 0;
	command->pipe_flag_in = 1;
	command->fd_pipe_in = command->fd_pipe_in_next;
	command->fd_out = 1;
}

int	parse_input(char *input, t_vars vars, int *pid, char *free_ptr)
{
	while (input)
	{
		input = forward_space(input);
		input = check_special_chars(input, vars.command, vars.env);
		if (input && input[0] == '|')
		{
			input = cas_pipe(input, vars.command);
			if (vars.command->options && vars.command->options[0]!= ' ' && vars.env->stop == 0)
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
	return (1);
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


int parse_loop(int argc, char **argv, t_env *env, t_command *command)
{
	char	*input;
	pid_t	lastpid;
	pid_t	pid;
	int		status;
	char	*free_ptr;

	input = readline("$> ");
	free_ptr = input;
	lastpid = -1;
	if (!input)
		exit(0);
	add_history(input);
	parse_input(input, make_var_struct(argc, argv, env, command), &pid, free_ptr);
	free(free_ptr);
	if (env->stop == 0)
	{
		if (command->options && command->options[0]!= ' ')
			lastpid = execute(env, command);
	}
	while (1)
	{
		pid = wait(&status);
		if (env->received_sig)
		{
			env->received_sig = 0;
			break;
		}
		if (pid == lastpid)
			env->value = WEXITSTATUS(status);
		if (pid < 0)
			break ;
	}
	init(command, 0);
	env->stop = 0;
	env->pid = 0;
	parse_loop(argc, argv, env, command);	
	return (0);
}

int main(int argc, char **argv, char **envp)
{
	t_env				myenv;
	t_command			mycommand;

	signal(SIGINT, handle_ctrl_c);
	signal(SIGQUIT, handle_ctrl_d);
	init(&mycommand, 1);
	// myenv.value = 0;
	// myenv.stop = 0;
	g_env.env = copy_env(envp);

	parse_loop(argc, argv, &g_env, &mycommand);
	return (0);
}
