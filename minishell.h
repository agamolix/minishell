/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atrilles <atrilles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 17:40:41 by atrilles          #+#    #+#             */
/*   Updated: 2022/04/22 17:41:03 by atrilles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>


#ifndef MINISHELL_H
# define MINISHELL_H


typedef struct s_command
{
	char *file_in;
	int fd_in;
	char *file_out;
	int fd_out;
	char *options;
}	t_command;




int value;


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

//-----paths-----
int do_command(char **tab, char **envp);
char **find_paths(char **envp);
char **modify_paths(char **paths);
char *find_right_path(char **paths, char *cmd);
char *find_path(char **envp, char *cmd);

//-----var-----
char *find_var(char **envp, char *var);
char *find_free_var(char **envp, char *var);

//-----cmd-----
void cmd_exit();
int cmd_env(char **envp2, char **tab);
int cmd_cd(char **tab);
int cmd_pwd();
int cmd_echo(char **envp2, char **tab);

//-----export_unset-----
char *exist_var(char **envp2, char *var);
char **remove_var(char **envp2, char *var);
char **cmd_unset(char **envp2, char **tab);
char **add_var(char **envp2, char *var);
char **cmd_export(char **envp2, char **tab);

//-----atoi-----
int maybe_unsigned_long_long(char *str);
int is_unsigned_long_long(char *str);
long long int convert_long_long(char *str);
char *itoa(int nb);




#endif