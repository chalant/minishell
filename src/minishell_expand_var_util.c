/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_expand_var_util.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 19:00:26 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/26 19:00:51 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// skipped if (!add_len || !fill);
// error: perror, return
int	ms_join_mask(t_token *token, int add_len, char fill)
{
	char	*new;
	int		old_len;

	if (!add_len || !fill)
		return (0);
	old_len = ft_strlen(token->mask_exp);
	new = malloc(sizeof(char) * (old_len + add_len + 1));
	if (!new)
	{
		ms_perror("expansion", NULL, NULL, errno);
		return (ERR_MALLOC);
	}
	if (token->mask_exp)
		ft_strlcpy(new, token->mask_exp, old_len + 1);
	new[old_len + add_len] = 0;
	while (add_len--)
		new[old_len + add_len] = fill;
	if (token->mask_exp)
		free(token->mask_exp);
	token->mask_exp = new;
	return (0);
}

// never free's str
// error: perror, return
int	ms_join_str(t_token *token, char *str, char mask)
{
	char	*new;
	int		str_len;

	if (!str)
		return (0);
	str_len = ft_strlen(str);
	if (!token->string)
		token->string = ft_strdup("");
	if (!token->string)
	{
		ms_perror("expansion", NULL, NULL, errno);
		return (ERR_MALLOC);
	}
	new = ft_strjoin(token->string, str);
	if (!new)
	{
		ms_perror("expansion", NULL, NULL, errno);
		return (ERR_MALLOC);
	}
	free(token->string);
	token->string = new;
	return (ms_join_mask(token, str_len, mask));
}
