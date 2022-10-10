/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 04:28:50 by gmillon           #+#    #+#             */
/*   Updated: 2022/10/10 05:08:05 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*filenames_by_index(char **matched_files, char **files)
{
	int		i;
	char	*result;

	result = NULL;
	i = 0;
	while (matched_files[i])
	{
		if (!ft_strncmp(matched_files[i], "", 1))
		{
			i++;
			continue ;
		}
		result = ft_strjoin(result, files[i]);
		result = ft_strjoin(result, " ");
		i++;
	}
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
	return (ft_strnstr(filtered_files[j], vars.split_str[i], slen(filtered_files[j])));
}

char	*match_files(char **files, t_wildcard vars)
{
	char	**filtered_files;
	int		i;
	int		j;
	char	*substr_ptr;

	j = 0;
	i = 0;
	filtered_files = ft_copy_split(files);
	while (vars.split_str[i])
	{
		j = 0;
		while (filtered_files[j])
		{
			substr_ptr = ft_strnstr(filtered_files[j], vars.split_str[i], slen(filtered_files[j]));
			substr_ptr = edge_cases(filtered_files, vars, i, j);
			if (!substr_ptr)
			{
				free(filtered_files[j]);
				filtered_files[j] = ft_strdup("");
			}
			else
			{
				free(filtered_files[j]);
				filtered_files[j] = ft_strdup(substr_ptr);
			}
			j++;
		}
		i++;
	}
	return (filenames_by_index(filtered_files, files));
}

t_wildcard	make_split_struct(char *str)
{
	t_wildcard	vars;

	vars.leading = 0;
	vars.trailing = 0;
	if (str && str[0] == '*')
		vars.leading = 1;
	if (str && str[slen(str) - 1] == '*')
		vars.trailing = 1;
	vars.split_str = ft_split(str, '*');
	return (vars);
}

char	*replace_wildcards(char *str)
{
	char	**args;
	char	**files;
	int		i;
	char	*final;
	char	*result;

	if (!ft_strchr(str, '*'))
		return (str); 
	args = ft_split(str, ' ');
	files = get_pwd_files();
	i = 0;
	while (args[i])
	{
		if (!ft_strchr(args[i], '*'))
		{
			i++;
			continue ;
		}
		args[i] = match_files(files, make_split_struct(args[i]));
		i++;
	}
	free(files);
	result = ft_join_arr_by_str(args, " ");
	final = ft_strdup(result + 1);
	free(result);
	return (final);
}
