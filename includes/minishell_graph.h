/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_graph.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 16:41:34 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/17 16:41:35 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_GRAPH_H
# define MINISHELL_GRAPH_H

#include "libft.h"

typedef struct	s_graph 
{
	int			num_vertices;
	int			current_size;
	t_darray	*adjacency_list;
}				t_graph;

int			init_graph(t_graph *graph, int num_vertices);
int			clear_graph(t_graph *graph, int (*del_method)(t_darray *, void(*)(void *)));
int			add_edge(t_graph *graph, int start, void* data);
int			add_adjacency_list(t_graph *graph, int type_size, int size);

t_darray	*get_edges(t_graph *graph, int index);

#endif
