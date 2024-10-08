/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_grammar.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 16:40:32 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/27 16:42:12 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_GRAMMAR_H
# define MINISHELL_GRAMMAR_H

# include "minishell_tokens.h"

# define MS_NEW_LINE '0'
# define MS_EQUAL '1'
# define MS_INTEGER '2'
# define MS_STRING '3'
# define MS_PROMPTING '4'

typedef struct s_ms_symbol
{
	char	*name;
	int		symbol_type;
	int		rule;
	int		skip;
	void	*context;
	int		(*match)(struct s_ms_symbol*, t_token *);
}			t_ms_symbol;

typedef struct s_ms_rule
{
	int			rule_id;
	int			length;
	int			parent;
	char		*name;
	t_ms_symbol	**symbols;
}				t_ms_rule;

typedef struct s_ms_grammar
{
	char		*start_rule;
	t_ms_rule	**rules;
	int			length;
}				t_ms_grammar;

int		ms_match_equal(t_ms_symbol *symbol, t_token *token);
int		ms_match_string(t_ms_symbol *symbol, t_token *token);

int		add_rule(t_ms_grammar *grammar, char **rules, int i);
int		handle_terminal_symbol(t_ms_symbol *symbol, char *input);

int		set_grammar(t_ms_grammar *grammar, char **definition);
void	delete_grammar(t_ms_grammar *grammar);
int		print_grammar(t_ms_grammar *grammar);
int		set_minishell_grammar(t_ms_grammar *grammar);

#endif
