#ifndef MINISHELL_PARSING_H
# define MINISHELL_PARSING_H

#define MS_TERMINAL_SYMBOL 0
#define MS_NON_TERMINAL_SYMBOL 1
#define MS_NULL_SYMBOL 2

# include "libft.h"

typedef struct	s_earley_item
{
	int		rule_group;
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

typedef struct	s_ms_parser
{
	t_ms_grammar	*grammar;
	t_earley_set	**chart;
	int				chart_size;
}				t_ms_parser;

int	build_earley_items(t_earley_set **sets, t_ms_grammar *grammar, int n_sets, char **input);
int	ms_parse_tree();

#endif
