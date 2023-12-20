/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_startup.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 18:04:43 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/20 18:26:32 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ms_startup_fail(t_ms_context *data)
{
	ms_perror("start-up", NULL, NULL, errno);
	ft_clear_ds(data->env);
	free_parse_data(&(data->parse_data), NULL);
	ft_darray_delete(&(data->commands), NULL);
	return (1);
}

static int	ms_init_parse(t_ms_context *data)
{
	init_parse_data(&(data->parse_data));
	data->commands.contents = NULL;
	if (alloc_parse_data(&(data->parse_data), 20) < 0)
		return (1);
	if (ft_darray_init(&(data->commands), sizeof(t_command), 10) < 0)
		return (1);
	data->tree.children = NULL;
	return (0);
}

int	ms_startup(t_ms_context *data)
{
	if (ms_envcpy(data))
		return (ms_startup_fail(data));
	if (ms_set_signals(data))
		return (ms_startup_fail(data));
	if (ms_init_parse(data))
		return (ms_startup_fail(data));
	g_global_state.status = 0;
	g_global_state.prompt = 0;
	data->status = 0;
	return (0);
}
