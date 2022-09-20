/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 17:40:41 by atrilles          #+#    #+#             */
/*   Updated: 2022/09/20 12:30:20 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
// #include <editline/readline.h>

#include <sys/wait.h>
#include <signal.h>
# include "libft.h"

typedef struct s_command
{
	char *file_in;
	int fd_file_in;
	char *file_out;
	int fd_file_out;
	char *options;
	int fd_pipe_in;
	int fd_pipe_out;
	int fd_pipe_in_next;
	int pipe_flag_in;
	int pipe_flag_out;
	int fd_out;
	int fd_in;

}	t_command;
/*
typedef struct s_redir
{
	char *filename;
	int type; // type = O_CREATE | O_TRUNC;
	int sens;

}	t_redior;

typedef struct s_cmmmd 
{
	char	**args; //args[0] = "ls";
	int		pipe_in;
	int		pipe_out;
	t_redir *redir;
	s_cmmmd *next;
}	cmd_t;
*/
typedef struct s_env
{
	char **env;
	int value;
	int stop;
} t_env;

typedef struct parse_vars
{
	int 		argc; 
	char		**argv;
	t_env		*myenv; 
	t_command	*mycommand;
} t_parse_vars;


//-----functions-----
int str_len(char *str);
char    *line(char *str, char c);
char    *forward(char *str, char c);
int     count(char *str, char c);
char	**my_split(char *s, char c);
int str_n_cmp(char *s1, char *s2, size_t n);
char    *emptystr();
char	*sub_str(char *s, unsigned int start, size_t len);
char	*str_join(char *s1, char *s2);
char	*str_n_dup_start(char *src, int nb, int start);
char	*str_n_dup(char *src, int nb);
char	*str_dup(char *src);
char *str_chr(char *s, int c);
void	put_nbr_fd(int n, int fd);

//-----do_command-----
int do_command(char **tab, t_env *env, t_command *command);

//-----paths-----
char **find_paths(t_env *env);
char **modify_paths(char **paths);
char *find_right_path(char **paths, char *cmd);
char *find_path(t_env *env, char *cmd);

//-----var-----
char *find_free_var(t_env *env, char *var);

//-----cmd-----
void cmd_exit(char **tab, t_env *env);
int cmd_cd(char **tab, t_env *env);
void cmd_env(t_env *env, char **tab);
void cmd_pwd(t_env *env);
void cmd_echo(char **tab, t_env *env);

//-----export_unset-----
char *exist_var(t_env *env, char *var);
char **remove_var(t_env *env, char *var);
int cmd_unset(t_env *env, char **tab);
char **add_var(t_env *env, char *var);
int cmd_export(t_env *env, char **tab);

//-----atoi-----
int maybe_unsigned_long_long(char *str);
int is_unsigned_long_long(char *str);
long long int convert_long_long(char *str);
char *itoa(int nb);

//-----init-----
int init(t_command *command);
int init_cmd(t_command *command);

//-----case_parse-----
char *forward_space(char *input);
int maybe_char(char c);
char *cas_chevron_in(char *input, t_command *command, t_env *env);
char *cas_chevron_out(char *input, t_command *command, t_env *env);
char *cas_pipe(char *input);
char *insert_variable(char *str, int index1, int index2, char *val);
char *replace_variable(char *temp, t_env *env);
char *cas_double_quote(char *input, t_command *command, t_env *env);
char *cas_single_quote(char *input, t_command *command, t_env *env);
char *cas_char(char *input, t_command *command, t_env *env);



#endif