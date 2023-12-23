/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_execution_expansion.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 12:09:39 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/23 17:37:15 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static void	test_print_tokens(t_ms_context *data, t_darray *tokens)
// {
// // DEBUG FUNCTION
// 	if (data)
// 		tokens = data->parse_data.tokens;
// 	printf("## tokens: ");
// 	int i = 0;
// 	while (i < tokens->size)
// 	{
// 		printf("%6s, ", ((t_token *)ft_darray_get(tokens, i))->string);
// 		i++;
// 	}
// 	printf("\n## masks : ");
// 	i = 0;
// 	while (i < tokens->size)
// 	{
// 		printf("%6s, ", ((t_token *)ft_darray_get(tokens, i))->mask_exp);
// 		i++;
// 	}
// 	printf("\n");
// // REMOVE WHEN DONE
// }

// 'into' will either contain the result of an expansion or a copy of 'token'
//IMPORTANT NOTE: this should not free this token, also don't add this token in the array
//otherwise it might get double freed. this token is freed later.
static int	expand_token(t_token *token, t_darray *into/*, t_ms_context *data*/)
{
	t_token	copy;

	copy.flags = token->flags;
	copy.string = ft_strdup(token->string);
	copy.mask_exp = ft_strdup(token->mask_exp);
	if (!copy.string || (token->mask_exp && !copy.mask_exp))
	{
		ms_clear_token(&copy);
		return (ERR_NOMEM);
	}
	if (!(token->flags & IS_DELIMITER) && token->flags & IS_VAR)
	{
		/*if (!g_global_state.status)
			g_global_state.status = data->status;*/
		if (ms_expand_var(into, &copy))
			return (ERR_NOMEM);
		/*g_global_state.status = 0;*/
	}
	else if (!(token->flags & IS_DELIMITER) && token->flags & IS_WILDCARD)
	{
		if (ms_expand_wildcard(into, &copy))
			return (ERR_NOMEM);
	}
	else if (ft_darray_append(into, &copy))
	{
		ms_clear_token(&copy);
		return (ERR_NOMEM);
	}
// test_print_tokens(NULL, into);
	ms_quote_expansion(into, 0);
	return (1);
}

// returns 0 if not expanded, 1 if it was, or ERR_NOMEM
// always removes quotes
int	expand_or_rmv_qts(t_token *token, t_darray *into/*, t_ms_context *data*/)
{
	if (token->flags & IS_DELIMITER
		|| (!(token->flags & IS_VAR) && !(token->flags & IS_WILDCARD)))
	{
		if (token->flags & IS_QUOTED)
			ms_remove_quotes(token->string, token->mask_exp);
		return (0);
	}
	return (expand_token(token, into/*, data*/));
}

//adds elements at the begging of the array.
int prepend_arguments(t_darray *arguments, t_darray *into)
{
	ft_darray_reverse(into);
	ft_darray_reverse(arguments);
	arguments->size = arguments->size - 1;
	ft_darray_join(into, arguments);
	ft_darray_reverse(into);
	return (0);
}

int expand_arguments(t_darray *arguments, t_darray *tmp)
{
	int			i;
	int			check;
	t_token		*token;
	t_darray	remainder;

	//todo: handle errors.
	if (ft_darray_init(&remainder, arguments->type_size, 10) < 0)
		return (-1);
	i = -1;
	while (++i < arguments->size)
	{
		token = ft_darray_get(arguments, i);
		check = expand_or_rmv_qts(token, tmp);
		if (!check)
			continue ;
		// handle check == ERR_MALLOC
		ft_darray_onsert(arguments, tmp, &remainder, i);
		ft_darray_reset(tmp, NULL);
		ft_darray_reset(&remainder, NULL);
	}
	return (0);
}

int expand_redirection(t_redirection *redirection)
{
	t_darray	tmp;
	int			check;

	//todo: return if there is no expansion needed.
	//	-> should be good now
	check = expand_or_rmv_qts(redirection->token, &tmp);
	if (!check || check == ERR_MALLOC)
		return (check);
	//not sure about this condition
	if (tmp.size != 1)
	{
		ft_darray_delete(&tmp, ms_clear_token);
		return (ms_perror(redirection->token->string, 
			NULL, "ambiguous redirect", 0) * -1);
	}
	//todo: might need to free this, but avoid double frees!
	redirection->file_path = ft_darray_get(&tmp, 0);
	redirection->redirection_flags |= MS_FREE;
	return (0);
}

int expand_command_fields(t_command *command)
{
	t_darray	tmp;
	int			check;

	//todo: check if it should be expanded... return outherwise.
	if (ft_darray_init(&tmp, sizeof(t_token), 10) < 0)
		return (-1);
	check = expand_or_rmv_qts(command->token, &tmp);
	if (check == 1)
	{
		//todo: set command to null if the expansion is empty.
		//this will be freed when clearing the arguments.
		command->command_name = ((t_token *)ft_darray_get(&tmp, 0))->string;
		prepend_arguments(&tmp, command->arguments);
		ft_darray_reset(&tmp, NULL);
	}
	return (expand_arguments(command->arguments, &tmp));
}
