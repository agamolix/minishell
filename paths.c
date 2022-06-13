/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atrilles <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 19:35:18 by atrilles          #+#    #+#             */
/*   Updated: 2022/02/08 16:14:27 by atrilles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int command(char **tab, char **envp)
{
	pid_t pid;

	pid = fork();
	if (pid == -1) //echec fork
	{
		perror("Erreur fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0) //processus fils
	{
		if (execve(find_path(envp, tab[0]), tab, NULL) == -1)
			perror("Execve");
	}
	else //processus père
	{
		wait(NULL);
		kill(pid, SIGTERM);
	}
	return 0;
}

char **find_paths(char **envp)
{
	char **paths;
	int i;

	i = 0;
	while(envp[i])
	{
		if (str_n_cmp(envp[i], "PATH", 4) == 0)
			paths = my_split(envp[i], ':');
		i++;
	}
	return (paths);
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

char *find_path(char **envp, char *cmd)
{
	char **paths;
	char *path;

	paths = find_paths(&envp[0]);
	modify_paths(&paths[0]);
	path = find_right_path(&paths[0], cmd);
	return (path);
}
