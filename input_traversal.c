/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_traversal.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmillon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 20:47:53 by gmillon           #+#    #+#             */
/*   Updated: 2022/10/07 20:51:17 by gmillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*forward_space(char *input)
{
	while (input[0] == ' ' || input[0] == '\t')
		input++;
	return (input);
}

int	maybe_char(char c)
{
	if (c != ' ' && c != '|' && c != '"'
		&& c != '\'' && c != '<' && c != '>')
		return (1);
	return (0);
}
