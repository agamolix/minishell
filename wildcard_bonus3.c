/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_bonus3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 04:28:50 by gmillon           #+#    #+#             */
/*   Updated: 2022/10/10 23:49:41 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*filenames_by_index(char **filtered_files, char **files)
{
	int		i;
	char	*result;

	result = NULL;
	i = 0;
	while (filtered_files[i])
	{
		if (!ft_strncmp(filtered_files[i], "", 1))
		{
			free(files[i]);
			i++;
			continue ;
		}
		result = str_join(result, files[i]);
		result = str_join(result, ft_strdup(" "));
		i++;
	}
	free(files);
	free_split(filtered_files);
	return (result);
}

char	*edge_cases(char **filtered_files, t_wildcard vars, int i, int j)
{
	int	file_len;
	int	search_len;
	int	array_len;

	array_len = arr_len(vars.split_str);
	search_len = slen(vars.split_str[i]);
	file_len = slen(filtered_files[j]);
	if (i == 0 && !vars.leading)
		return (ft_strnstr(filtered_files[j], vars.split_str[i], search_len));
	if (i == array_len - 1 && !vars.trailing)
	{
		if (file_len - search_len >= 0)
			return (ft_strnstr(&filtered_files[j][file_len - search_len], \
								vars.split_str[i], search_len));
		else
			return (NULL);
	}
	return (ft_strnstr(filtered_files[j], \
						vars.split_str[i], slen(filtered_files[j])));
}
