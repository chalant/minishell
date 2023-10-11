#include "minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_VERTICES 100

struct Node {
    int data;
    struct Node* next;
};

struct Graph {
    int numVertices;
    struct Node* adjacencyList[MAX_VERTICES];
};

struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

struct Graph* createGraph(int numVertices) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->numVertices = numVertices;
    for (int i = 0; i < numVertices; i++) {
        graph->adjacencyList[i] = NULL;
    }
    return graph;
}

void addEdge(struct Graph* graph, int src, int dest) {
    struct Node* newNode = createNode(dest);
    newNode->next = graph->adjacencyList[src];
    graph->adjacencyList[src] = newNode;
}

void DFSWithPaths(struct Graph* graph, int vertex, int dest, int visited[], int path[], int pathIndex) {
    visited[vertex] = 1;
    path[pathIndex] = vertex;
    pathIndex++;

    if (vertex == dest) {
        printf("Path: ");
        for (int i = 0; i < pathIndex; i++) {
            printf("%d ", path[i]);
        }
        printf("\n");
    } else {
        struct Node* temp = graph->adjacencyList[vertex];
        while (temp != NULL) {
            int adjVertex = temp->data;
            if (!visited[adjVertex]) {
                DFSWithPaths(graph, adjVertex, dest, visited, path, pathIndex);
            }
            temp = temp->next;
        }
    }

    visited[vertex] = 0;
}

void findPaths(struct Graph* graph, int start, int dest) {
    int visited[MAX_VERTICES] = {0};
    int path[MAX_VERTICES];
    int pathIndex = 0;

    printf("Paths from vertex %d to vertex %d:\n", start, dest);
    DFSWithPaths(graph, start, dest, visited, path, pathIndex);
}

int main() {
    int 			numVertices = 6;
    struct Graph*	graph = createGraph(numVertices);

    addEdge(graph, 0, 1);
    addEdge(graph, 0, 2);
    addEdge(graph, 1, 3);
    addEdge(graph, 1, 4);
    addEdge(graph, 2, 4);
    addEdge(graph, 3, 5);
    addEdge(graph, 4, 5);

    findPaths(graph, 0, 4);

    return 0;
}

int	ms_edges(t_darray *edges, t_ms_rule *rule, int depth, int node)
{
	int	i;

	if (depth >= rule->length)
		return (0);
	rule->symbols[depth];
	return (1);
}

int	ms_search_core(t_darray *path, t_ms_rule *rule, int end, int depth, int node)
{
	int			i;
	int			j;
	t_darray	edges;
	t_ms_edge	*c_edge;

	if (ft_darray_init(&edges, sizeof(t_ms_edge), 10) < 0)
		return (-1);
	if (depth == rule->length && node == end)
		return (0);
	ms_edges(&edges, rule, depth, node);
	i = -1;
	//todo: check if the path is well formatted, might get errors here.
	while (++i < edges.size)
	{
		c_edge = (t_ms_edge *)(edges.contents + i);
		ft_darray_append(path, c_edge);
		ms_search_core(&subpaths, rule, end, depth + 1, c_edge->finish);
		j = -1;
		while (++j < subpaths.size)
			subpaths
	}
	ft_darray_delete(&edges);
	return (1);
}

int	ms_search(t_darray *paths, t_ms_rule *rule, int root)
{
	ft_darray_init(paths, sizeof(t_darray), 10);
	return (1);
}