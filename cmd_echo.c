/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 21:01:02 by gmillon           #+#    #+#             */
/*   Updated: 2022/10/10 00:18:10 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**double_quotes_newtab(char *subset)
{
	char	**split;
	char	**result;

	split = ft_split(subset, ' ');
	if (subset && *subset != ' ')
		return (split);
	split[0] = str_join(ft_strdup(" "), split[0]);
	return (split);
}

char	**echo_double_quotes(char *arg, char **tab, char target_char)
{
	static char	*subset = NULL;
	char		*substr;
	int			start;
	int			len;

	if (subset == NULL)
		subset = arg;
	if (arg == NULL)
	{
		subset = NULL;
		return (NULL);
	}
	start = ft_strchr_index(subset, target_char) + 1;
	len = ft_strchr_index(subset + start, target_char);
	substr = ft_substr(subset, start, len);
	// ft_printf("slen(substr): %d\n", slen(substr));
	if (slen(substr))
		write(1, substr, slen(substr));
	free(substr);
	if (tab)
		free(tab);
	subset = subset + start + len + 1;
	if (!subset || !*subset)
		return (ft_split("", 0));
	return (double_quotes_newtab(subset));
}

void	echo_string(char **tab, t_command *command, int i)
{
	int	j;

	while (tab[i])
	{
		j = 0;
		while (tab[i][j])
		{
			if (tab[i][j] == '\"' || tab[i][j] == '\'')
			{
				tab = echo_double_quotes(command->options, tab, tab[i][j]);
				// ft_printf("slen(tab[0]): %d\n", slen(tab[0]));
				// ft_printf("tab[0]: _%s_\n", tab[0]);
				i = -1;
				j = 0;
				break ;
			}
			write(1, &tab[i][j], 1);
			j++;
		}
		if (tab[i + 1] && i != -1)
			write(1, " ", 1);
		i++;
	}
}

void	cmd_echo(char **tab, t_env *env, t_command *command)
{
	int	i;
	int	j;

	i = 1;
	if (tab[1] && str_n_cmp(tab[1], "$?", 3) == 0)
	{
		put_nbr_fd(env->value, 1);
		write(1, "\n", 1);
	}
	if (tab[1] && str_n_cmp(tab[1], "-n", 3) == 0)
		i = 2;
	echo_string(tab, command, i);
	if (i != 2)
		write(1, "\n", 1);
	env->value = 0;
}
