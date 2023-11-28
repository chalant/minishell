#ifndef MINISHELL_PARSING_H
# define MINISHELL_PARSING_H

#define MS_TERMINAL_SYMBOL 0
#define MS_NON_TERMINAL_SYMBOL 1
#define MS_NULL_SYMBOL 2

#include "libft.h"
#include "minishell_graph.h"
#include "minishell_grammar.h"

typedef struct	s_earley_item
{
	int		rule;
	int		start;
	int		next;
	int		completed;
}				t_earley_item;

typedef	struct	s_earley_set
{
	int			size;
	t_darray	*items;
}				t_earley_set;

typedef struct	s_ms_edge
{
	int	start;
	int	finish;
	int	rule;
}				t_ms_edge;

typedef	struct	s_ms_chart
{
	int			size;
	t_darray	*edges;
}				t_ms_chart;

typedef struct	s_parsing_data
{
	int				input_length;
	int				chart_size;
	t_ms_grammar	*grammar;
	t_graph			*chart;
	t_darray		*tokens;
}				t_parsing_data;

typedef struct	s_parser_state
{
	int				depth;
	int				node;
	t_ms_rule		*rule;;
}				t_parser_state;

typedef struct	s_parse_tree
{
	int			start_rule;
	int			rule;
	int			terminal;
	int			start;
	int			end;
	char		*rule_name;
	t_darray	*children;
}				t_parse_tree;

int		ms_prompt(t_ms_symbol *symbol, t_token *token);
int		build_earley_items(t_darray *sets, t_ms_grammar *grammar, t_darray *tokens);
int		build_parse_tree(t_parse_tree *parse_tree, t_parsing_data *data);
int 	add_earley_set(t_darray *sets, int size);
void    clear_earley_set(t_earley_set *set);
int		ms_search_core(t_parse_tree *tree, t_parsing_data *data, t_parser_state state);

#endif
