#ifndef MINISHELL_GRAPH_H
# define MINISHELL_GRAPH_H

#include "libft.h"

typedef struct	s_graph 
{
	int			num_vertices;
	t_darray	*adjacency_list;
}				t_graph;

int			init_graph(t_graph *graph, int num_vertices, int type_size);
int			clear_graph(t_graph *graph, int (*del_method)(t_darray *, void(*)(void *)));
int			add_edge(t_graph *graph, int start, void* data);
int			add_adjacency_list(t_graph *graph, int type_size, int size);

t_darray	*get_edges(t_graph *graph, int index);

#endif
