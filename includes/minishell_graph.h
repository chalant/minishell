#ifndef MINISHELL_GRAPH_H
# define MINISHELL_GRAPH_H

#include "libft.h"

typedef struct	s_graph 
{
	int num_vertices;
	t_darray **adjacency_list;
}				t_graph;

int	init_graph(t_graph *graph, int num_vertices, int type_size);
int	delete_graph(t_graph *graph);
int	add_edge(t_graph *graph, int start, void* data);

#endif
