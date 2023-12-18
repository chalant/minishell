/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_grammar_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 16:13:02 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/18 16:15:23 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_symbol(t_ms_symbol *symbol)
{
	symbol->match = NULL;
	symbol->name = NULL;
	symbol->rule = 0;
	symbol->symbol_type = 0;
	symbol->context = NULL;
	return (1);
}

int	add_symbol(t_ms_symbol **dest, char *definition, int j)
{
	t_ms_symbol	*symbol;

	symbol = malloc(sizeof(t_ms_symbol));
	if (!symbol)
		return (-1);
	init_symbol(symbol);
	dest[j] = symbol;
	if (ft_strchr(definition, '\''))
		return (handle_terminal_symbol(symbol, definition));
	symbol->name = definition;
	symbol->symbol_type = MS_NON_TERMINAL_SYMBOL;
	return (1);
}
