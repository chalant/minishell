/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_grammar.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 13:49:44 by ychalant          #+#    #+#             */
/*   Updated: 2023/10/23 13:50:05 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

static const char	g_minishell_grammar[163] = "sum:sum <'+-' product\n"\
	"sum:product\n"\
	"product:product <'*/' factor\n"\
	"product:factor\n"\
	"factor:='(' sum =')'\n"\
	"factor:number\n"\
	"number:<'0123456789' number\n"\
	"number:<'0123456789'\n";

int	ms_match_subset(t_ms_symbol *symbol, char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (!ft_strchr(symbol->name, input[i]))
			return (0);
		i++;
	}
	return (1);
}

int ms_match_equal(t_ms_symbol *symbol, char *input)
{
	size_t	len;

	len = ft_strlen(input);
	if (ft_strlen(symbol->name) != len)
		return (0);
	if (ft_strnstr(symbol->name, input, len))
		return (1);
	return (0);
}

int	set_grammar(t_ms_grammar *grammar)
{
	int			i;

	i = -1;
	grammar->rules = malloc(sizeof(t_ms_rule *) * 8);
	while (++i < 8)
		grammar->rules[i] = malloc(sizeof(t_ms_rule));
	grammar->start_rule = "sum";

	grammar->rules[0]->name = "sum";
	grammar->rules[0]->symbols = malloc(sizeof(t_ms_symbol *) * 3);
	grammar->rules[0]->symbols[0] = malloc(sizeof(t_ms_symbol));
	grammar->rules[0]->symbols[0]->name = "sum";
	grammar->rules[0]->symbols[0]->symbol_type = MS_NON_TERMINAL_SYMBOL;
	grammar->rules[0]->symbols[1] = malloc(sizeof(t_ms_symbol));
	grammar->rules[0]->symbols[1]->name = "+-";
	grammar->rules[0]->symbols[1]->symbol_type = MS_TERMINAL_SYMBOL;
	grammar->rules[0]->symbols[1]->match = ms_match_subset;
	grammar->rules[0]->symbols[2] = malloc(sizeof(t_ms_symbol));
	grammar->rules[0]->symbols[2]->name = "product";
	grammar->rules[0]->symbols[2]->symbol_type = MS_NON_TERMINAL_SYMBOL;

	grammar->rules[1]->name = "sum";
	grammar->rules[1]->symbols = malloc(sizeof(t_ms_symbol *));
	grammar->rules[1]->symbols[0] = malloc(sizeof(t_ms_symbol));
	grammar->rules[1]->symbols[0]->name = "product";
	grammar->rules[1]->symbols[0]->symbol_type = MS_NON_TERMINAL_SYMBOL;

	grammar->rules[2]->name = "product";
	grammar->rules[2]->symbols = malloc(sizeof(t_ms_symbol *) * 3);
	grammar->rules[2]->symbols[0] = malloc(sizeof(t_ms_symbol));
	grammar->rules[2]->symbols[0]->name = "product";
	grammar->rules[2]->symbols[0]->symbol_type = MS_NON_TERMINAL_SYMBOL;
	grammar->rules[2]->symbols[1] = malloc(sizeof(t_ms_symbol));
	grammar->rules[2]->symbols[1]->name = "*/";
	grammar->rules[2]->symbols[1]->symbol_type = MS_TERMINAL_SYMBOL;
	grammar->rules[2]->symbols[1]->match = ms_match_subset;
	grammar->rules[2]->symbols[2] = malloc(sizeof(t_ms_symbol));
	grammar->rules[2]->symbols[2]->name = "factor";
	grammar->rules[2]->symbols[2]->symbol_type = MS_NON_TERMINAL_SYMBOL;

	grammar->rules[3]->name = "product";
	grammar->rules[3]->symbols = malloc(sizeof(t_ms_symbol *));
	grammar->rules[3]->symbols[0] = malloc(sizeof(t_ms_symbol));
	grammar->rules[3]->symbols[0]->name = "factor";
	grammar->rules[3]->symbols[0]->symbol_type = MS_NON_TERMINAL_SYMBOL;

	grammar->rules[4]->name = "factor";
	grammar->rules[4]->symbols = malloc(sizeof(t_ms_symbol *) * 3);
	grammar->rules[4]->symbols[0] = malloc(sizeof(t_ms_symbol));
	grammar->rules[4]->symbols[0]->name = "(";
	grammar->rules[4]->symbols[0]->symbol_type = MS_TERMINAL_SYMBOL;
	grammar->rules[4]->symbols[0]->match = ms_match_subset;
	grammar->rules[4]->symbols[1] = malloc(sizeof(t_ms_symbol));
	grammar->rules[4]->symbols[1]->name = "sum";
	grammar->rules[4]->symbols[1]->symbol_type = MS_NON_TERMINAL_SYMBOL;
	grammar->rules[4]->symbols[2] = malloc(sizeof(t_ms_symbol));
	grammar->rules[4]->symbols[2]->name = ")";
	grammar->rules[4]->symbols[2]->match = ms_match_subset;
	grammar->rules[4]->symbols[2]->symbol_type = MS_TERMINAL_SYMBOL;

	grammar->rules[5]->name = "factor";
	grammar->rules[5]->symbols = malloc(sizeof(t_ms_symbol *));
	grammar->rules[5]->symbols[0] = malloc(sizeof(t_ms_symbol));
	grammar->rules[5]->symbols[0]->name = "number";
	grammar->rules[5]->symbols[0]->symbol_type = MS_NON_TERMINAL_SYMBOL;

	grammar->rules[6]->name = "number";
	grammar->rules[6]->symbols = malloc(sizeof(t_ms_symbol *) * 2);
	grammar->rules[6]->symbols[0] = malloc(sizeof(t_ms_symbol));
	grammar->rules[6]->symbols[0]->name = "[0123456789]";
	grammar->rules[6]->symbols[0]->symbol_type = MS_TERMINAL_SYMBOL;
	grammar->rules[6]->symbols[0]->match = ms_match_subset;
	grammar->rules[6]->symbols[1] = malloc(sizeof(t_ms_symbol));
	grammar->rules[6]->symbols[1]->name = "number";
	grammar->rules[6]->symbols[1]->symbol_type = MS_NON_TERMINAL_SYMBOL;

	grammar->rules[7]->name = "number";
	grammar->rules[7]->symbols = malloc(sizeof(t_ms_symbol *));
	grammar->rules[7]->symbols[0] = malloc(sizeof(t_ms_symbol));
	grammar->rules[7]->symbols[0]->name = "[0123456789]";
	grammar->rules[7]->symbols[0]->symbol_type = MS_TERMINAL_SYMBOL;
	grammar->rules[7]->symbols[0]->match = ms_match_subset;

	return (0);
}
