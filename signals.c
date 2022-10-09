/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/09 03:00:49 by gmillon           #+#    #+#             */
/*   Updated: 2022/10/10 01:52:47 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_ctrl_c(int sig)
{
	(void) sig;

	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	if (!g_env.pid)
		rl_redisplay();
	if (g_env.pid)
	{
		kill(g_env.pid, SIGTERM);
		g_env.value = 130;
	}
	else
		g_env.value = 1;
	g_env.received_sig = 1;
}

void	handle_ctrl_d(int sig)
{
	(void) sig;

	if (g_env.pid)
	{
		kill(g_env.pid, SIGQUIT);
		g_env.value = 131;
		printf("Quit: 3");
	}
	return ;
}
