/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 19:35:18 by atrilles          #+#    #+#             */
/*   Updated: 2022/09/20 12:09:46 by gmillon          ###   ########.fr       */
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

char	*relative_path(char *cmd)
{
	char	buf[1000];
	int		i;
	char	**absolute_path;
	char	**folders_in_path;
	int		current_folder_index;

	i = 0;
	getcwd(buf, 1000);
	absolute_path = my_split(buf, '/');
	folders_in_path = my_split(cmd, '/');
	current_folder_index = arr_len(absolute_path) - 1;
	absolute_path = double_pointer_realloc(absolute_path, arr_len(absolute_path) + arr_len(folders_in_path));
	while (folders_in_path[i])
	{
		if (str_n_cmp(folders_in_path[i], "..", 3) == 0)
			current_folder_index--;
		else if (str_n_cmp(folders_in_path[i], ".", 2) != 0)
		{
			absolute_path[current_folder_index + 1] = folders_in_path[i];
			current_folder_index++;
		}
		i++;
	}
	absolute_path[current_folder_index + 1] = 0;
//	printf("\nAbsolute path: %s\n", ft_join_arr_by_str(absolute_path, "/"));
	if (access(ft_join_arr_by_str(absolute_path, "/"), F_OK || X_OK) == 0)
		return (ft_join_arr_by_str(absolute_path, "/"));
	return (0);
}

char *validate_path(char *relative_path, char *env_path, t_env *env)
{
	if (env_path)
		return (env_path);
	if (relative_path)
		return (relative_path);
	perror("Path error");
	env->value = 126;
	env->stop = 1;
	return (0);	
}

char *find_path(t_env *env, char *cmd)
{
	char	**paths;
	char	*env_path;
	char	*rel_path;
	
	paths = find_paths(env);
	// if (str_n_cmp(cmd, ".", 1) == 0)
	// 	rel_path = relative_path(cmd);
	// else
	// 	rel_path = 0;
	rel_path = relative_path(cmd);
	if (paths == 0)
	{
		perror("Path error");
		env->value = 127;
		env->stop = 1;
		return (0);
	}
	modify_paths(&paths[0]);
	env_path = find_right_path(&paths[0], cmd);
	// if (path == 0)
	// {
	// 	perror("Path error");
	// 	env->value = 126;
	// 	env->stop = 1;
	// 	return (0);
	// }
	return (validate_path(rel_path, env_path, env));
}
