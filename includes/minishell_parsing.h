#ifndef MINISHELL_PARSING_H
# define MINISHELL_PARSING_H

#define MS_TERMINAL_SYMBOL 0
#define MS_NON_TERMINAL_SYMBOL 1
#define MS_NULL_SYMBOL 2

# include "libft.h"
# include "minishell_graph.h"

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

typedef struct	s_ms_symbol
{
	const char	*name;
	int			symbol_type;
	int			rule;
	int			(*match)(struct s_ms_symbol*, char *);
}				t_ms_symbol;

typedef struct	s_ms_rule
{
	int			rule_id;
	int			length;
	const char	*name;
	t_ms_symbol	**symbols;
}				t_ms_rule;

typedef struct	s_ms_grammar
{
	const char		*start_rule;
	const char		*name;
	t_ms_rule		**rules;
	int				length;
}				t_ms_grammar;

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
	t_ms_grammar	*grammar;
	t_graph			*chart;
	char			**input;
	int				input_length;
	int				chart_size;
}				t_parsing_data;

typedef struct	s_parser_state
{
	int				depth;
	int				node;
	int				end;
	t_ms_rule		*rule;
	t_darray		*substate;
}				t_parser_state;

typedef struct	s_parse_tree
{
	char		*rule_name;
	int			rule;
	int			terminal;
	int			start;
	int			end;
	t_darray	*children;
}				t_parse_tree;

int	build_earley_items(t_earley_set **sets, t_ms_grammar *grammar, int n_sets, char **input);
int	ms_build_parse_tree(t_parse_tree *parse_tree, t_parsing_data *data);

#endif
