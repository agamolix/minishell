/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   relative_paths.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 23:41:16 by gmillon           #+#    #+#             */
/*   Updated: 2022/10/07 23:43:04 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**build_relative_path(char **absolute_path, \
	char **folders_in_path, int current_folder_index)
{
	int	i;

	i = 0;
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
	return (absolute_path);
}

char	*relative_path(char *cmd)
{
	char	buf[1000];
	char	**absolute_path;
	char	**folders_in_path;
	char	**target_path;
	int		current_folder_index;

	getcwd(buf, 1000);
	absolute_path = my_split(buf, '/');
	folders_in_path = my_split(cmd, '/');
	current_folder_index = arr_len(absolute_path) - 1;
	absolute_path = double_pointer_realloc(absolute_path, \
		arr_len(absolute_path) + arr_len(folders_in_path));
	target_path = build_relative_path(absolute_path, folders_in_path, \
									current_folder_index);
	free_split(folders_in_path);
	if (access(ft_join_arr_by_str(target_path, "/"), F_OK || X_OK) == 0)
		return (ft_join_arr_by_str(target_path, "/"));
	return (0);
}
