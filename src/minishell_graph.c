/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_graph.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 13:49:29 by ychalant          #+#    #+#             */
/*   Updated: 2023/11/29 13:41:29 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_graph.h"

int	init_graph(t_graph *graph, int num_vertices)
{
	graph->adjacency_list = malloc(sizeof(t_darray));
	if (!graph->adjacency_list)
		return (-1);
	if (ft_darray_init(graph->adjacency_list, sizeof(t_darray), num_vertices))
		return (-1);
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

void	delete_darray(void *data)
{
	t_darray	*array;

	array = (t_darray *)data;
	ft_darray_delete(array, NULL);
}

int	clear_graph(t_graph *graph, int (*del_method)(t_darray *, void(*)(void *)))
{
	// int			i;
	// t_darray	*list;

	// i = -1;
	// while (++i < graph->adjacency_list->size)
	// {
	// 	list = ft_darray_get(graph->adjacency_list, i);
	// 	del_method(list, );
	// 	free(list);
	// }
	del_method(graph->adjacency_list, delete_darray);
	//free(graph->adjacency_list);
	//graph->adjacency_list = NULL;
	graph->num_vertices = 0;
	return (1);
}
