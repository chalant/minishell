/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_graph.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 13:49:29 by ychalant          #+#    #+#             */
/*   Updated: 2023/10/23 13:54:27 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_graph.h"

int	init_graph(t_graph *graph, int num_vertices, int type_size)
{
	int	i;

	graph->adjacency_list = (t_darray **)malloc(sizeof(t_darray *) * num_vertices);
	if (!graph->adjacency_list)
		return (-1);
	i = -1;
	while (++i < num_vertices)
		graph->adjacency_list[i] = NULL;
	i = -1;
	while (++i < num_vertices)
	{
		graph->adjacency_list[i] = (t_darray *)malloc(sizeof(t_darray));
		if (!graph->adjacency_list[i])
			return (-1);
		if (ft_darray_init(graph->adjacency_list[i], type_size, 10) < 0)
			return (-1);
	}
	graph->num_vertices = num_vertices;
	return (1);
}

int	add_edge(t_graph *graph, int start, void* data)
{
	return (ft_darray_append(graph->adjacency_list[start], data));
}

int	delete_graph(t_graph *graph)
{
	int	i;

	i = -1;
	while (++i < graph->num_vertices)
	{
		ft_darray_delete(graph->adjacency_list[i], NULL);
		free(graph->adjacency_list);
	}
	return (1);
}
