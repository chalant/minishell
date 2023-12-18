/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_history_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 14:27:45 by yves              #+#    #+#             */
/*   Updated: 2023/12/18 16:10:57 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_add_separator(t_ms_context *context, char *line, const char *delim)
{
	char	*tmp;

	tmp = ft_strjoin(context->line, delim);
	if (!tmp)
	{
		free(line);
		return (-1);
	}
	free(context->line);
	context->line = tmp;
	return (1);
}

int	ms_join_line(t_ms_context *context, char *line, const char *delim)
{
	char	*tmp;

	if (!line)
		return (1);
	if (ms_add_separator(context, line, delim) < 0)
		return (-1);
	tmp = ft_strjoin(context->line, line);
	if (!tmp)
	{
		free(line);
		return (-1);
	}
	free(context->line);
	context->line = tmp;
	return (1);
}
