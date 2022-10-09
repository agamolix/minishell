/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 19:35:18 by atrilles          #+#    #+#             */
/*   Updated: 2022/10/09 04:04:36 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>

char	**find_paths(t_env *env)
{
	char	**paths;
	int		i;

	i = 0;
	while (env->env[i])
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

char	**modify_paths(char **paths)
{
	int		i;
	char	*temp;

	i = 0;
	while (paths[i])
	{
		temp = paths[i];
		if (i == 0)
		{
			paths[i] = sub_str(paths[i], 5, slen(paths[i]) - 5);
			free(temp);
			temp = paths[i];
		}
		paths[i] = str_join(paths[i], ft_strdup("/"));
		i++;
	}
	return (paths);
}

char	*find_right_path(char **paths, char *cmd)
{
	int		i;
	char	*path;
	char	*temp;

	i = 0;
	while (paths[i])
	{
		temp = paths[i];
		if (cmd[0] == '/')
			path = cmd;
		else
			path = str_join(ft_strdup(paths[i]), ft_strdup(cmd));
		if (access(path, F_OK || X_OK) == 0)
			return (path);
		i++;
	}
	return (0);
}

char	*validate_path(char *relative_path, char *env_path, t_env *env)
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

char	*find_path(t_env *env, char *cmd)
{
	char	**paths;
	char	*env_path;
	char	*rel_path;

	paths = find_paths(env);
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
	return (validate_path(rel_path, env_path, env));
}
