/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_bonus2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 04:28:50 by gmillon           #+#    #+#             */
/*   Updated: 2022/10/11 00:21:36 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	num_pwd_files(void)
{
	DIR				*current_dir;
	char			buf[1000];
	struct dirent	*file;
	int				count;

	count = 0;
	getcwd(buf, 1000);
	current_dir = opendir(buf);
	if (current_dir)
	{
		file = readdir(current_dir);
		while (file)
		{
			count++;
			file = readdir(current_dir);
		}
		closedir(current_dir);
	}
	return (count);
}

char	**get_pwd_files(void)
{
	DIR				*current_dir;
	char			buf[1000];
	struct dirent	*file;
	char			**files;
	int				i;

	getcwd(buf, 1000);
	i = 0;
	files = malloc(sizeof(char *) * (num_pwd_files() + 1));
	current_dir = opendir(buf);
	if (current_dir)
	{
		file = readdir(current_dir);
		while (file)
		{
			files[i] = ft_strdup(file->d_name);
			i++;
			file = readdir(current_dir);
		}
		files[i] = NULL;
		closedir(current_dir);
	}
	return (files);
}
