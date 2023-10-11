from functools import partial

def df_search(edges_fn, child_fn, pred_fn, root):
    def aux(depth, node):
        if pred_fn(depth, node):
            return [[]]
        else:
            result = []
            for edge in edges_fn(depth, node):
                subpaths = aux(depth + 1, child_fn(depth, edge))
                for subpath in subpaths:
                    result.append([(node, edge)] + subpath)
            return result

    paths = aux(0, root)
    if paths:
        return paths[0]
    else:
        return None

def top_list(grammar, input_data, chart, start, finish_rule):
    def pred(depth, node):
        return depth == bottom and node == finish_rule['finish']

    def child(depth, edge):
        return edge['finish']

    def edges(depth, node):
        if depth >= len(symbols):
            return []
        else:
            symbol = symbols[depth]
            if isinstance(symbol, tuple):  # Terminal symbol
                t, _ = symbol
                if t(input_data[node]):
                    return [{'finish': node + 1, 'rule': -1}]
                else:
                    return []
            elif isinstance(symbol, str):  # Non-terminal symbol
                result = []
                for edge in chart[node]:
                    if grammar['rule_name'](grammar, edge['rule']) == symbol:
                        result.append(edge)
                return result
            else:
                raise ValueError("Invalid symbol type")

    symbols = grammar['rule_body'](grammar, finish_rule['rule'])
    bottom = len(symbols)

    path = df_search(partial(edges, start=start), partial(child, start=start), pred, start)
    if path is None:
        raise ValueError("There's always a solution")
    return path

# Example usage:
# Replace these functions and data with your actual implementations
def rule_body(grammar, rule):
    pass

def rule_name(grammar, rule):
    pass

class ParseTree:
    def __init__(self, node_type, children=None, token=None):
        self.node_type = node_type
        self.children = children or []
        self.token = token

def parse_tree(grammar, input_data, chart):
    start = 0
    finish = len(chart) - 1
    name = grammar['start_symbol']

    def rule_name(edge):
        return grammar['rule_name'](grammar, edge['rule'])

    def aux(start, edge):
        if edge['rule'] == -1:
            return ParseTree('Token', token=input_data[start])
        else:
            children = [
                aux(child_edge['start'], child_edge) 
                for child_edge in top_list(grammar, input_data, chart, start, edge)]
            return ParseTree(rule_name(edge), children=children)

    target_edge = next((edge for edge in chart[start] if edge['finish'] == finish and rule_name(edge) == name), None)

    if target_edge is None:
        raise ValueError("Are you sure this parse succeeded?")

    return aux(start, target_edge)

# Example usage:
# Replace these functions and data with your actual implementations and data
def rule_name(grammar, rule):
    return

grammar = {'start_symbol': 'your_start_symbol', 'rule_name': rule_name}
input_data = "your_input_data_here"
chart = []  # Your chart data

result = parse_tree(grammar, input_data, chart)
print(result)