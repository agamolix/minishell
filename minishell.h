/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 17:40:41 by atrilles          #+#    #+#             */
/*   Updated: 2022/10/10 23:56:50 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <dirent.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <signal.h>
# include <errno.h>
# include "libft.h"

typedef struct s_command
{
	char	*file_in;
	int		fd_file_in;
	char	*file_out;
	int		fd_file_out;
	char	*options;
	int		fd_pipe_in;
	int		fd_pipe_out;
	int		fd_pipe_in_next;
	int		pipe_flag_in;
	int		pipe_flag_out;
	int		fd_out;
	int		fd_in;

}		t_command;

/*
typedef struct s_redir
{
	char	*filename;
	int		type; // type = O_CREATE | O_TRUNC;
	int		sens;

}	t_redior;

typedef struct s_cmmmd 
{
	char	**args; //args[0] = "ls";
	int			pipe_in;
	int			pipe_out;
	t_redir *redir;
	s_cmmmd *next;
}	cmd_t;
*/
typedef struct s_env
{
	char	**env;
	int		value;
	int		stop;
	pid_t	pid;
	int		received_sig;
}			t_env;

typedef struct parse_vars
{
	int			argc;
	char		**argv;
	t_env		*myenv;
	t_command	*mycommand;
}				t_parse_vars;

typedef struct s_vars
{
	int			argc;
	char		**argv;
	t_env		*env;
	t_command	*command;
}				t_vars;

typedef struct s_wildcard
{
	int			leading;
	int			trailing;
	char		**split_str;
}				t_wildcard;

extern t_env	g_env;
//-----functions-----
extern void		rl_replace_line(const char *s, int x);
int				slen(char *str);
char			*line(char	*str, char c);
char			*forward(char	*str, char c);
int				count(char	*str, char c);
char			**my_split(char	*s, char c);
int				str_n_cmp(char	*s1, char	*s2, size_t n);
char			*emptystr(void);
char			*sub_str(char	*s, unsigned int start, size_t len);
char			*str_join(char	*s1, char	*s2);
char			*str_n_dup_start(char	*src, int nb, int start);
char			*str_n_dup(char	*src, int nb);
char			*str_dup(char	*src);
char			*str_chr(char	*s, int c);
void			put_nbr_fd(int n, int fd);

//--			--do_command-----
int				do_command(char	**tab, t_env *env, t_command *command);

// SIGNALS
void			handle_ctrl_d(int sig);
void			handle_ctrl_c(int sig);

//--			--paths-----

char			*relative_path(char *cmd);
char			**build_relative_path(char **absolute_path, \
										char **folders_in_path);
char			**find_paths(t_env *env);
char			**modify_paths(char	**paths);
char			*find_right_path(char	**paths, char	*cmd);
char			*find_path(t_env *env, char	*cmd);

//--			--var-----
char			*find_free_var(t_env *env, char	*var);

//--			--cmd-----

int				execute(t_env *env, t_command *command);
void			cmd_exit(char	**tab, t_env *env);
int				cmd_cd(char	**tab, t_env *env);
void			cmd_env(t_env *env, char	**tab);
void			cmd_pwd(t_env *env);
void			cmd_echo(char	**tab, t_env *env, t_command *command);

//--			--export_unset-----
char			*exist_var(t_env *env, char	*var);
char			**remove_var(t_env *env, char	*var);
int				cmd_unset(t_env *env, char	**tab);
char			**add_var(t_env *env, char	*var);
int				cmd_export(t_env *env, char	**tab);

//--			--atoi-----
int				maybe_unsigned_long_long(char	*str);
int				is_unsigned_long_long(char	*str);
long long int	convert_long_long(char	*str);
char			*itoa(int nb);

//-----init-----
int				init(t_command *command, int first);
int				init_cmd(t_command *command);

// Data Utils
void			free_split(char	**arr);
void			await_child_loop(t_env *env, pid_t lastpid);
t_vars			make_var_struct(int argc, char **argv, \
								t_env *env, t_command *command);

//-----case_parse-----
void			handle_pipe_flag_in(t_command *command, t_env *env);
int				handle_pipes(t_command *command, t_env *env);
t_command		*parse_input(char *input, t_vars vars, char *free_ptr);

char			*check_special_chars(char *input, t_command *command, \
									t_env *env);
int				parse_loop(int argc, char **argv, t_env *env, \
							t_command *command);
char			*forward_space(char	*input);
int				maybe_char(char c);
char			*cas_heredoc(char	*input, t_command *command, t_env *env);
char			*cas_append(char	*input, t_command *command, t_env *env);
char			*cas_chevron_in(char	*input, t_command *command, t_env *env);
char			*cas_chevron_out(char	*input, t_command *command, t_env *env);
void			pipe_var_set(t_command *command);
char			*cas_pipe(char	*input, t_command *command);
char			*insert_variable(char *str, int index1, int index2, char *val);
char			*replace_variable(char	*temp, t_env *env);
char			*cas_double_quote(char	*input, t_command *command, t_env *env);
char			*cas_single_quote(char	*input, t_command *command, t_env *env);
char			*cas_char(char	*input, t_command *command, t_env *env);

//WILDCARD
int				num_pwd_files(void);
char			*match_files(char **files, t_wildcard vars);
char			*filenames_by_index(char **filtered_files, char **files);
char			*replace_wildcards(char *str);
char			*edge_cases(char **filtered_files, \
							t_wildcard vars, int i, int j);
char			**get_pwd_files(void);

#endif