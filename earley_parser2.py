from typing import List, Tuple, Union, Optional

def df_search(paths, edges, child, pred, root):
	def aux(pths, depth, root, index):
		pths[index] = (root, edge)
		index += 1
		if pred(depth, root):
			print(pths)
		else:
			for edge in edges(depth, root):
				aux(pths, depth + 1, child(depth, edge), index)
	aux(paths, 0, root, 0)

def df_search2(edges, child, pred, root):
	def aux(depth, root):
		if pred(depth, root):
			return [[]]
		else:
			paths = []
			for edge in edges(depth, root):
				sub_paths = aux(depth + 1, child(depth, edge))
				for path in sub_paths:
					paths.append([(root, edge)] + path)
			return paths

	result = aux(0, root)
	if not result:
		return None
	return result

# Define a simple directed graph as an adjacency list
graph = {
	'A': ['B', 'C', 'G', 'I'],
	'B': ['D', 'E', 'H', 'I'],
	'C': ['F'],
	'D': [],
	'E': ['F'],
	'F': [],
	'G': ['H'],
	'H': ['I'],
	'I': ['F']
}

# Define the edges function to return outgoing edges for a node
def edges(depth, node):
	if node in graph:
		return [(edge, None) for edge in graph[node]]
	else:
		return []

# Define the child function to return the child node for an edge
def child(depth, edge):
	return edge[0]

# Define the pred function to check if a node is a target node (e.g., 'F')
def pred(depth, node):
	return node == 'F'

# Define the root node
root_node = 'A'

# Call df_search with the defined functions and root node
result = []
df_search(result, edges, child, pred, root_node)

# Print the result
if result is None:
	print("No paths found")
else:
	print("Paths found:")
	for path in result:
		print(path)

result = df_search2(edges, child, pred, root_node)

if result is None:
	print("No paths found")
else:
	print("Paths found:")
	for path in result:
		print(path)