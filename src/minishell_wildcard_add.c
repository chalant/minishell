/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_wildcard_add.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 20:25:00 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/15 20:25:42 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// only looks for quotes
static void	ms_add_flags(t_token *token)
{
	int	i;

	token->flags = 0;
	if (!token->string)
		return ;
	i = 0;
	while (token->string[i] && !(token->flags & IS_QUOTED))
	{
		if (token->mask_exp[i] == '0'
			&& (token->string[i] == '"' || token->string[i] == '\''))
			token->flags |= IS_QUOTED;
		i++;
	}
}

// returns ERR_MALLOC on error, doesn't clear 'new'
int	ms_wildcard_add(t_darray *tokens, t_dirent *entryp, t_token *new)
{
	int	namlen;

	namlen = ft_strlen(entryp->d_name);
	new->string = malloc(sizeof(char) * (namlen + 1));
	if (!new->string)
		return (ERR_MALLOC);
	ft_strlcpy(new->string, entryp->d_name, namlen + 1);
	ms_add_flags(new);
	if (ft_darray_append(tokens, new))
		return (ERR_MALLOC);
	return (0);
}
