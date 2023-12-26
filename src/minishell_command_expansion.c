/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_command_expansion.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 11:35:49 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/25 11:36:47 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//todo: expanded tokens should be marked in expand token...
static void	mark_expanded(t_darray *tmp)
{
	int	j;

	j = -1;
	while (++j < tmp->size)
		((t_token *)ft_darray_get(tmp, j))->flags |= IS_EXPANDED;
}

static int	expand_arguments(t_darray *arguments,
	t_darray *tmp, t_ms_context *data)
{
	int			i;
	t_token		*token;
	t_darray	remainder;
	int			size;

	if (ft_darray_init(&remainder, arguments->type_size, 10) < 0)
		return (ERR_NOMEM);
	i = -1;
	size = arguments->size;
	while (++i < size)
	{
		token = ft_darray_get(arguments, i);
		if (!should_expand(token))
			continue ;
		if (expand_token(token, tmp, data) < 0)
			return (ft_darray_delete(&remainder, NULL) - 1);
		mark_expanded(tmp);
		if (ft_darray_onsert(arguments, tmp, &remainder, i) < 0)
			return (ft_darray_delete(&remainder, NULL) - 1);
		ft_darray_reset(tmp, NULL);
		ft_darray_reset(&remainder, NULL);
	}
	ft_darray_delete(&remainder, NULL);
	return (0);
}

//adds elements at the begining of "into" array.
int	prepend_arguments(t_darray *arguments, t_darray *into)
{
	t_token	*token;

	token = ft_darray_get(arguments, 0);
	free(token->mask_exp);
	free(token->string);
	mark_expanded(arguments);
	ft_darray_reverse(into);
	ft_darray_reverse(arguments);
	arguments->size = arguments->size - 1;
	if (ft_darray_join(into, arguments) < 0)
		return (-1);
	ft_darray_reverse(into);
	return (0);
}

int	expand_command_name(t_command *command, t_darray *tmp)
{
	if (should_expand(command->token))
	{
		free(command->command_name);
		command->command_name = NULL;
		expand_token(command->token, tmp, command->context);
		if (!tmp->size)
			return (0);
		command->command_name = ft_strdup(((t_token *)ft_darray_get(
						tmp, 0))->string);
		if (!command->command_name)
			return (-1);
		if (prepend_arguments(tmp, command->arguments) < 0)
			return (-1);
		ft_darray_reset(tmp, NULL);
	}
	return (1);
}

int	expand_command_fields(t_command *command)
{
	t_darray	tmp;
	int			status;

	if (ft_darray_init(&tmp, sizeof(t_token), 10) < 0)
		return (ERR_NOMEM);
	if (expand_command_name(command, &tmp) < 0)
		return (flush_expansion(&tmp, ERR_NOMEM));
	status = expand_arguments(command->arguments, &tmp, command->context);
	if (status < 0)
		return (flush_expansion(&tmp, ERR_NOMEM));
	if (!command->command_name && command->arguments->size)
	{
		command->command_name = ft_strdup(((t_token *)ft_darray_get(
						command->arguments, 0))->string);
		if (!command->command_name)
			return (flush_expansion(&tmp, ERR_NOMEM));
		ft_darray_reverse(command->arguments);
		command->arguments->size = command->arguments->size - 1;
		ft_darray_reverse(command->arguments);
	}
	ft_darray_delete(&tmp, NULL);
	return (status);
}
