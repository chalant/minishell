/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_parsing.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 16:41:39 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/20 19:02:26 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_PARSING_H
# define MINISHELL_PARSING_H

# define MS_TERMINAL_SYMBOL 0
# define MS_NON_TERMINAL_SYMBOL 1
# define MS_NULL_SYMBOL 2

# include "libft.h"
# include "minishell_graph.h"
# include "minishell_grammar.h"

typedef struct s_earley_item
{
	int	rule;
	int	start;
	int	next;
	int	completed;
}		t_earley_item;

typedef struct s_earley_set
{
	t_darray	*items;
	int			size;
}				t_earley_set;

typedef struct s_ms_edge
{
	int	start;
	int	finish;
	int	rule;
}		t_ms_edge;

typedef struct s_ms_chart
{
	int			size;
	t_darray	*edges;
}				t_ms_chart;

typedef struct s_parsing_data
{
	int				input_length;
	int				chart_size;
	t_ms_grammar	*grammar;
	t_graph			*chart;
	t_darray		*tokens;
	t_darray		*earley_sets;
}					t_parsing_data;

typedef struct s_parser_state
{
	int			depth;
	int			node;
	t_ms_rule	*rule;
}				t_parser_state;

typedef struct s_parse_tree
{
	char		*rule_name;
	t_darray	*children;
	t_token		*token;
	int			start_rule;
	int			rule;
	int			terminal;
	int			start;
	int			end;
}				t_parse_tree;

int				init_parse_data(t_parsing_data *data);
int				alloc_parse_data(t_parsing_data *data, int size);
int				reset_parse_data(t_parsing_data *data, t_parse_tree *tree);
int				free_parse_data(t_parsing_data *data, t_parse_tree *tree);

int				recognize_input(t_parsing_data *data, void *context);
int				parse_input(t_parsing_data *data, t_parse_tree *tree);
int				ms_prompt_command(t_ms_symbol *symbol, t_token *token);
int				ms_syntax_error(void *input);

int				init_tree(t_parse_tree *tree);
int				init_children(t_parse_tree *parse_tree);
int				ms_start_rule(t_parse_tree *tree, t_parsing_data *data);
int				ms_search_core(t_parse_tree *tree,
					t_parsing_data *data, t_parser_state state);
int				build_parse_tree(t_parse_tree *parse_tree,
					t_parsing_data *data);
int				clear_parse_tree(t_parse_tree *tree,
					int (*del_method)(t_darray *, void(*)(void *)), int del);
int				fill_parse_tree(t_parse_tree *parse_tree, t_parsing_data *data);
void			set_subtree(t_parse_tree *subtree, char *rule_name,
					int start, int end);
t_parse_tree	*get_subtree(t_parse_tree *tree, int index);
t_parser_state	next_state(int depth, int node, t_ms_rule *rule);

int				build_earley_items(t_parsing_data *data, void *context);
int				init_earley_items(t_parsing_data *data, void *context);
int				earley_predict(t_parsing_data *data, t_ms_symbol *symbol,
					int state_id);
int				earley_complete(t_parsing_data *data, int j, int state_id);
int				earley_scan(t_parsing_data *data, t_ms_symbol *symbol,
					int state_id, int item_idx);
int				next_earley_items(t_parsing_data *data, t_earley_set *set,
					int state_id, int item_idx);
int				earley_safe_append(t_earley_set *set, t_earley_item *item);
int				add_earley_set(t_darray *sets, int size);
int				clear_earley_sets(t_darray *sets,
					int (*del_method)(t_darray *, void(*)(void *)));
int				set_parsing_info(t_parsing_data *data, void *context);
void			delete_earley_set(void *set);
void			reset_earley_set(void *set);

int				init_symbol(t_ms_symbol *symbol);
int				add_symbol(t_ms_symbol **dest, char *definition, int j);

t_ms_symbol		*next_symbol(t_ms_grammar *grammar, t_earley_item *item);

#endif
