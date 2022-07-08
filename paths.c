/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 19:35:18 by atrilles          #+#    #+#             */
/*   Updated: 2022/07/08 22:34:45 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>


char **find_paths(t_env *env)
{
	char **paths;
	int i;

	i = 0;
	while(env->env[i])
	{
		if (str_n_cmp(env->env[i], "PATH", 4) == 0)
		{
			paths = my_split(env->env[i], ':');
			return (paths);
		}
		i++;
	}
	return (0);
}

char **modify_paths(char **paths)
{
	int i;
	char *temp;

	i = 0;
	while (paths[i])
	{
		temp = paths[i];
		if (i == 0)
		{
			paths[i] = sub_str(paths[i], 5, str_len(paths[i]) - 5);
			free(temp);
			temp = paths[i];
		}
		paths[i] = str_join(paths[i], "/");
		free (temp);
		i++;
	}
	return (paths);
}

char *find_right_path(char **paths, char *cmd)
{
	int i;
	char *path;
	char *temp;

	i = 0;
	while (paths[i])
	{
		temp = paths[i];
		if (cmd[0] == '/')
			path = cmd;
		else
			path = str_join(paths[i], cmd);
		if (access(path, F_OK || X_OK) == 0)
			return (path);
		free(temp);
		i++;
	}
	return (0);
}

int arr_len(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
}
int relative_path(char *cmd)
{
	char	buf[1000];
	int		i;
	char	**absolute_path;
	char	**folders_in_path;
	int		current_folder_index;


	i = 0;
	getcwd(buf, 1000);
	printf("i%s\n;", buf);
	absolute_path = my_split(buf, "/");
	folders_in_path = my_split(cmd, "/");
	current_folder_index = arr_len(absolute_path) - 1;
	while (folders_in_path[i])
	{
		if (str_n_cmp(folders_in_path[i], "..", 3) == 0)
		{
			current_folder_index--;
		}
		else if (str_n_cmp(folders_in_path[i], ".", 2) != 0)
		{
			
		}

		i++;
	}
	
}
char *find_path(t_env *env, char *cmd)
{
	char **paths;
	char *path;
	relative_path(cmd);
	paths = find_paths(env);
	if (paths == 0)
	{
		perror("Path error");
		env->value = 127;
		env->stop = 1;
		return (0);
	}
	modify_paths(&paths[0]);
	path = find_right_path(&paths[0], cmd);
	if (path == 0)
	{
		perror("Path error");
		env->value = 126;
		env->stop = 1;
		return (0);
	}
	return (path);
}
