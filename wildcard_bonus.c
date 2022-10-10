/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 04:28:50 by gmillon           #+#    #+#             */
/*   Updated: 2022/10/10 22:59:04 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*check_match(char *substr_ptr, t_wildcard vars, int i)
{
	if (!substr_ptr)
		return (ft_strdup(""));
	else
		return (ft_strdup(substr_ptr + (slen(vars.split_str[i]) - 1)));
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
			substr_ptr = ft_strnstr(filtered_files[j], \
								vars.split_str[i], slen(filtered_files[j]));
			substr_ptr = edge_cases(filtered_files, vars, i, j);
			free(filtered_files[j]);
			filtered_files[j] = check_match(substr_ptr, vars, i);
			j++;
		}
		i++;
	}
	free_split(vars.split_str);
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

static char	*join_and_cleanup(char **args, char **files, char *str)
{
	char	*result;
	char	*final;

	result = ft_join_arr_by_str(args, " ");
	final = ft_strdup(result + 1);
	free(result);
	free(str);
	free_split(args);
	return (final);
}

char	*replace_wildcards(char *str)
{
	char	**args;
	char	**files;
	char	*substitution;
	int		i;

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
		substitution = match_files(files, make_split_struct(args[i]));
		free(args[i]);
		args[i] = substitution;
		i++;
	}
	return (join_and_cleanup(args, files, str));
}
