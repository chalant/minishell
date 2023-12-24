/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_execution_expansion.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 12:09:39 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/24 10:39:33 by ychalant         ###   ########.fr       */
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
static int	expand_token(t_token *token, t_darray *into, t_ms_context *data)
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
		if (!g_global_state.status)
			g_global_state.status = data->status;
		if (ms_expand_var(into, &copy))
			return (ERR_NOMEM);
		g_global_state.status = 0;
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

int	should_expand(t_token *token)
{
	if (token->flags & IS_DELIMITER
		|| (!(token->flags & IS_VAR) && !(token->flags & IS_WILDCARD)))
	{
		if (token->flags & IS_QUOTED)
			ms_remove_quotes(token->string, token->mask_exp);
		return (0);
	}
	return (1);
}

//adds elements at the begging of the array.
int prepend_arguments(t_darray *arguments, t_darray *into)
{
	t_token	*token;
	int		i;

	token = ft_darray_get(arguments, 0);
	free(token->mask_exp);
	free(token->string);
	i = -1;
	while (++i < arguments->size)
		((t_token *)ft_darray_get(arguments, i))->flags |= IS_EXPANDED;
	ft_darray_reverse(into);
	ft_darray_reverse(arguments);
	arguments->size = arguments->size - 1;
	ft_darray_join(into, arguments);
	ft_darray_reverse(into);
	return (0);
}

int expand_arguments(t_darray *arguments, t_darray *tmp, t_ms_context *data)
{
	int			i;
	int			j;
	t_token		*token;
	t_darray	remainder;

	//todo: handle errors.
	if (ft_darray_init(&remainder, arguments->type_size, 10) < 0)
		return (-1);
	i = -1;
	while (++i < arguments->size)
	{
		token = ft_darray_get(arguments, i);
		if (!should_expand(token))
			continue ;
		//todo: handle errors.
		expand_token(token, tmp, data);
		//todo:mark each token as epxanded.
		j = -1;
		while (++j < tmp->size)
			((t_token *)ft_darray_get(tmp, j))->flags |= IS_EXPANDED;
		ft_darray_onsert(arguments, tmp, &remainder, i);
		ft_darray_reset(tmp, NULL);
		ft_darray_reset(&remainder, NULL);
	}
	ft_darray_delete(&remainder, NULL);
	return (0);
}

int expand_redirection(t_redirection *redirection, t_ms_context *context)
{
	t_darray	tmp;

	if (!should_expand(redirection->token))
		return (0);
	if (ft_darray_init(&tmp, sizeof(t_token), 10) < 0)
		return (-1);
	if (expand_token(redirection->token, &tmp, context) < 0)
	{
		ft_darray_delete(&tmp, ms_clear_token);
		return (ERR_NOMEM);
	}
	if (tmp.size != 1)
	{
		ft_darray_delete(&tmp, ms_clear_token);
		return (ms_perror(redirection->token->string, 
			NULL, "ambiguous redirect", 0) * -1);
	}
	redirection->file_path = ft_strdup(((t_token *)ft_darray_get(&tmp, 0))->string);
	if (!redirection->file_path)
	{
		ft_darray_delete(&tmp, ms_clear_token);
		return (ERR_NOMEM);
	}
	redirection->redirection_flags |= MS_FREE;
	ft_darray_delete(&tmp, ms_clear_token);
	return (0);
}

int expand_command_fields(t_command *command)
{
	t_darray	tmp;
	int			status;

	//todo: check if it should be expanded... return outherwise.
	if (ft_darray_init(&tmp, sizeof(t_token), 10) < 0)
		return (-1);
	if (should_expand(command->token))
	{
		expand_token(command->token, &tmp, command->context);
		free(command->command_name);
		if (!tmp.size)
		{
			ft_darray_delete(&tmp, NULL);
			command->command_name = NULL;
			return (0);
		}
		command->command_name = ft_strdup(((t_token *)ft_darray_get(&tmp, 0))->string);
		if (!command->command_name)
			return (ft_darray_delete(&tmp, ms_clear_token) - 1);
		prepend_arguments(&tmp, command->arguments);
		ft_darray_reset(&tmp, NULL);
	}
	status = expand_arguments(command->arguments, &tmp, command->context);
	ft_darray_delete(&tmp, NULL);
	return (status);
}
