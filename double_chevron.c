#include "./libftextended/headers/libft.h"
#include "minishell.h"
#include <fcntl.h>
char	*append_char(char *str, char c)
{

}

char	*cas_heredoc(char *input, t_command *command, t_env *env)
{
	const int	heredoc = open(".heredoc", O_CREAT | O_WRONLY | O_TRUNC, 0640);
	char		*heredoc_input;
	int			i;
	char		*terminator;	

	i = 0;
	input += 2;
	input = forward_space(input);
	command->file_in = ".heredoc";
	// if (command->fd_file_in) 
		// close(command->fd_file_in);
	while (input[i] && maybe_char(input[i]))
		i++;
	printf("input_.beginning: %s\n", input);
	terminator = str_n_dup(input, i);
	heredoc_input = readline("heredoc> ");
	while (str_n_cmp(heredoc_input, terminator, str_len(terminator) + 1))
	{
		write(heredoc, heredoc_input, str_len(terminator));
		free(heredoc_input);
		write(heredoc, "\n", 1);
		heredoc_input = readline("heredoc> ");
	}
	free(heredoc_input);
	close(heredoc);
	command->fd_file_in = open(".heredoc", O_RDONLY);
	command->fd_in = command->fd_file_in;
	input = input + i;
	free(terminator);
	if (str_len(input) == 0)
		return (0);
	return (input);
}

char	*cas_append(char *input, t_command *command, t_env *env)
{
	int i;

	input += 2;
	input = forward_space(input);
	i = 0;
	while (input[i] && maybe_char(input[i]))
		i++;
	command->file_out = str_n_dup(input, i);
	command->fd_file_out = open(command->file_out, O_CREAT | O_WRONLY | O_APPEND);
	if (command->fd_file_out == -1)
	{
		printf("%s: Open file error\n", command->file_out);
		env->value = 1;
		env->stop = 1;
		free(command->file_out);
		return(0);
	}
	command->fd_out = command->fd_file_out;
	input = input + i;
	if (str_len(input) == 0)
		return(0);
	return(input);
}




// int main(void)
// {
// 	t_command *t;
// 	t_env		*e;
// 	init(t, 1);
// 	cas_heredoc("<< x", t, e);
// 	return(0);
// }