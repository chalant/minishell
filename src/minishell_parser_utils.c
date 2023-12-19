/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_parser_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 12:05:37 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/19 12:15:34 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_start_rule(t_parse_tree *tree, t_parsing_data *data)
{
	int				end;
	t_ms_edge		*edge;
	t_ms_edge		*longest;
	t_darray		*edges;

	end = 0;
	tree->start_rule = 1;
	edges = get_edges(data->chart, tree->start);
	edge = (t_ms_edge *)edges->contents + end;
	longest = edge;
	while (++end < edges->size)
	{
		edge = ((t_ms_edge *)edges->contents + end);
		if (edge->finish >= longest->finish)
			longest = edge;
	}
	tree->rule_name = data->grammar->rules[longest->rule]->name;
	tree->end = longest->finish;
	return (0);
}

int	ms_syntax_error(void *input)
{
	int				i;
	t_parsing_data	*data;
	t_token			*token;

	data = (t_parsing_data *)(input);
	i = data->earley_sets->size - 1;
	while (i > -1 && !((t_earley_set *)ft_darray_get(
				data->earley_sets, i))->items->size)
		i--;
	token = ft_darray_get(data->tokens, i);
	if (!(token->flags & IS_EOF))
	{
		ft_putstr_fd("syntax error near unexpected token '", STDERR_FILENO);
		if (!token->string)
			ft_putstr_fd("newline", STDERR_FILENO);
		else
			ft_putstr_fd(token->string, STDERR_FILENO);
		ft_putstr_fd("'\n", STDERR_FILENO);
	}
	else
		ft_putstr_fd("syntax error: unexpected end of file\n", STDERR_FILENO);
	return (2);
}
