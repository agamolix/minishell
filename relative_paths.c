/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   relative_paths.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 23:41:16 by gmillon           #+#    #+#             */
/*   Updated: 2022/10/10 00:45:32 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**build_relative_path(char **absolute_path,	char **folders_in_path)
{
	int	i;
	int	current_folder_index;

	current_folder_index = arr_len(absolute_path) - 1;
	i = 0;
	while (folders_in_path[i])
	{
		if (str_n_cmp(folders_in_path[i], "..", 3) == 0)
			current_folder_index--;
		else if (str_n_cmp(folders_in_path[i], ".", 2) != 0)
		{
			absolute_path[current_folder_index + 1] = \
				ft_strdup(folders_in_path[i]);
			current_folder_index++;
		}
		i++;
	}
	absolute_path[current_folder_index + 1] = 0;
	return (absolute_path);
}

char	*relative_path(char *cmd)
{
	char	buf[1000];
	char	**folders_in_cmd;
	char	**target_path_arr;
	char	*target_path_str;

	getcwd(buf, 1000);
	target_path_arr = my_split(buf, '/');
	folders_in_cmd = my_split(cmd, '/');
	target_path_arr = double_pointer_realloc(target_path_arr, \
		arr_len(target_path_arr) + arr_len(folders_in_cmd));
	target_path_arr = build_relative_path(target_path_arr, folders_in_cmd);
	target_path_str = ft_join_arr_by_str(target_path_arr, "/");
	free_split(folders_in_cmd);
	free_split(target_path_arr);
	if (access(target_path_str, F_OK || X_OK) == 0)
		return (target_path_str);
	return (0);
}
