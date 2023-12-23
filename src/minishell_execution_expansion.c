/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_execution_expansion.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 12:09:39 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/23 14:26:11 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 'into' will either contain the result of an expansion or a copy of 'token'
//IMPORTANT NOTE: this should not free this token, also don't add this token in the array
//otherwise it might get double freed. this token is freed later.
int	expand_token(t_token *token, t_darray *into)
{
	t_token	copy;

	copy.flags = token->flags;
	copy.string = ft_strdup(token->string);
	copy.mask_exp = NULL;
	if (!copy.string)
	{
		ms_clear_token(&copy);
		return (ERR_NOMEM);
	}
	if (!(token->flags & IS_DELIMITER) && token->flags & IS_VAR)
	{
		if (ms_expand_var(into, &copy))
			return (ERR_NOMEM);
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
	ms_quote_expansion(into, 0);
	return (1);
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
	t_token		*token;
	t_darray	remainder;

	//todo: handle errors.
	if (ft_darray_init(&remainder, arguments->type_size, 10) < 0)
		return (-1);
	i = -1;
	while (++i < arguments->size)
	{
		token = ft_darray_get(arguments, i);
		if (!ft_strchr(token->string, '$'))
			continue;
		expand_token(token, tmp);
		ft_darray_onsert(arguments, tmp, &remainder, i);
		ft_darray_reset(tmp, NULL);
		ft_darray_reset(&remainder, NULL);
	}
	return (0);
}

int expand_redirection(t_redirection *redirection)
{
	t_darray		tmp;

	if (!ft_strchr(redirection->token->string, '$'))
		return (0);
	//todo: return if there is no expansion needed.
	if (expand_token(redirection->token, &tmp) < 0)
		return (-1);
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

	//todo: check if it should be expanded... return outherwise.
	if (ft_darray_init(&tmp, sizeof(t_token), 10) < 0)
		return (-1);
	if (ft_strchr(command->token->string, '$'))
	{
		//todo: set command to null if the expansion is empty.
		expand_token(command->token, &tmp);
		//this will be freed when clearing the arguments.
		command->command_name = ((t_token *)ft_darray_get(&tmp, 0))->string;
		prepend_arguments(&tmp, command->arguments);
		ft_darray_reset(&tmp, NULL);
	}
	return (expand_arguments(command->arguments, &tmp));
}
