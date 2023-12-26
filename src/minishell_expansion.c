/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_expansion.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 12:09:39 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/26 13:56:14 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	flush_token(t_token *token, int status)
{
	ms_clear_token(token);
	return (status);
}

int	expand_token(t_token *token, t_darray *into, t_ms_context *data)
{
	t_token	copy;

	copy.flags = token->flags;
	copy.string = ft_strdup(token->string);
	copy.mask_exp = ft_strdup(token->mask_exp);
	if (!copy.string || (token->mask_exp && !copy.mask_exp))
		return (flush_token(&copy, ERR_NOMEM));
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
		return (flush_token(&copy, ERR_NOMEM));
	ms_quote_expansion(into, 0);
	return (1);
}

int	flush_expansion(t_darray *expansion, int status)
{
	ft_darray_delete(expansion, ms_clear_token);
	return (status);
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

int	expand_redirection(t_redirection *redirection, t_ms_context *context)
{
	t_darray	tmp;

	if (!should_expand(redirection->token))
		return (0);
	if (ft_darray_init(&tmp, sizeof(t_token), 10) < 0)
		return (-1);
	if (expand_token(redirection->token, &tmp, context) < 0)
		return (flush_expansion(&tmp, ERR_NOMEM));
	if (tmp.size != 1)
	{
		flush_expansion(&tmp, 0);
		return (ms_perror(redirection->token->string,
				NULL, "ambiguous redirect", 0) * -1);
	}
	redirection->file_path = ft_strdup(((t_token *)ft_darray_get(
					&tmp, 0))->string);
	if (!redirection->file_path)
		return (flush_expansion(&tmp, ERR_NOMEM));
	redirection->redirection_flags |= MS_FREE;
	ft_darray_delete(&tmp, ms_clear_token);
	return (0);
}
