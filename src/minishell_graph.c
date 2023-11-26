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

#include "minishell.h"
#include "minishell_graph.h"

int	init_graph(t_graph *graph, int num_vertices, int type_size)
{
	int	i;

	graph->adjacency_list = malloc(sizeof(t_darray));
	if (!graph->adjacency_list)
		return (-1);
	if (ft_darray_init(graph->adjacency_list, sizeof(t_darray), num_vertices))
		return (-1);
	graph->num_vertices = 0;
	i = -1;
	while (++i < num_vertices)
	{
		if (add_adjacency_list(graph, type_size, 10) < 0)
			return (-1);
	}
	return (1);
}

int	add_adjacency_list(t_graph *graph, int type_size, int size)
{
	t_darray	list;

	if (ft_darray_init(&list, type_size, size))
		return (-1);
	if (ft_darray_append(graph->adjacency_list, &list) < 0)
		return (-1);
	graph->num_vertices += 1;
	return (1);
}

t_darray	*get_edges(t_graph *graph, int index)
{
	return (ft_darray_get(graph->adjacency_list, index));
}

int	add_edge(t_graph *graph, int start, void *data)
{
	t_darray	*edges;

	edges = get_edges(graph, start);
	if (!edges)
		return (-1);
	return (ft_darray_append(edges, data));
}

int	delete_graph(t_graph *graph)
{
	int			i;
	t_darray	*list;

	i = -1;
	while (++i < graph->num_vertices)
	{
		list = ft_darray_get(graph->adjacency_list, i);
		ft_darray_delete(list, NULL);
	}
	ft_darray_delete(graph->adjacency_list, NULL);
	return (1);
}
