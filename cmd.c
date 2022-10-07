/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 19:35:18 by atrilles          #+#    #+#             */
/*   Updated: 2022/10/07 02:56:02 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// do we have to add env value for the first two exit cases? 
void cmd_exit(char **tab, t_env *env)
{
	if (!tab[1]) 
	{
		free_split(tab);
		exit(0);
	}
	if (maybe_unsigned_long_long(tab[1]) == 0 || is_unsigned_long_long(tab[1]) == 0)
	{
		free_split(tab);
		printf("Exit error: %s: numeric argument necessary\n", tab[1]);
		exit(2);
	}
	if (tab[2])
	{
		free_split(tab);
		printf("Exit error: too many arguments\n");
		exit(1);
	}
	env->value = convert_long_long(tab[1]) % 256;
	printf("exit\n");
	free_split(tab);
	exit(env->value);
}

int cmd_cd(char **tab, t_env *env)
{
	if (tab[2])
	{
		printf ("Error cd: too many arguments\n");
		env->value = 1;
		env->stop = 1;
		return (env->value);
	}
	if (tab[1] == 0)
	{
		env->value = 0;
		env->stop = 1;
		return (env->value);
	}
	if (chdir(tab[1]))
	{
		perror("Error cd");
		env->value = 1;
		env->stop = 1;
		return (env->value);
	}
	env->value = 0;
	return (env->value);
}
void cmd_env(t_env *env, char **tab)
{
	int i = 0;

	if (tab[1])
	{
		printf("Error env: too many arguments\n");
		env->value = 1;
		env->stop = 1;
		return;
	}
	while (env->env[i])
	{
	    printf("%s\n", env->env[i]);
		i++;
	}
	env->value = 0;
}

void cmd_pwd(t_env *env)
{
	char *buf;

	buf = malloc(sizeof(char) * 999);
	if (buf == 0)
	{
		perror("Erreur pwd");
		env->value = 1;
		env->stop = 1;
		return;
	}
	write(1, getcwd(buf, 1000), slen(getcwd(buf, 1000)));
	write(1, "\n", 1);
	free(buf);
	env->value = 0;
}

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
	// ft_printf("start: %d\n", start);
	// # ft_printf("substr + start + 1: %s\n", &substr[start + 1]);
	len = ft_strchr_index(subset + start, target_char);
	// ft_printf("len: %d\n", len);
	substr = ft_substr(subset, start, len);
	// ft_printf("substr: %s\n", substr);
	write(1, substr, slen(substr));
	free(substr);
	if (tab)
		free(tab);
	subset = subset + start + len + 1;
	if (!subset || !*subset)
		return (ft_split("", 0));
	// ft_printf("subset: %s\n", subset);
	return (double_quotes_newtab(subset));
}
void	cmd_echo(char **tab, t_env *env, t_command *command)
{
	int i = 1;
	int	j;

	ft_printf("command->options: %s\n", command->options);
	if (tab[1] && str_n_cmp(tab[1], "$?", 3) == 0)
	{
		put_nbr_fd(env->value, 1);
		write(1, "\n", 1);
	}
	if (tab[1] && str_n_cmp(tab[1], "-n", 3) == 0)
		i = 2;
	// if quotes, print w substr ftsrtchr, then remake tab with split on incrementted input. 
	while (tab[i])
	{
		// write(1, tab[i], slen(tab[i]));
		j = 0;
		while (tab[i][j])
		{
			if (tab[i][j] == '\"' || tab[i][j] == '\'')
			{
				tab = echo_double_quotes(command->options, tab, tab[i][j]);
				// ft_printf("\n New arr: ");
				// ft_print_str_arr(tab);
				i =  -1;
				j = 0;
				// exit(0);
				break;
			}
			write(1, &tab[i][j], 1);
			j++;
		}
		if (tab[i + 1] && i != -1)
			write(1, " ", 1);
		i++;
	}
	if (tab[1] && str_n_cmp(tab[1], "-n", 3))
		write(1, "\n", 1);
	env->value = 0;
}
