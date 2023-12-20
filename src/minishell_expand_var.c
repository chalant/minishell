/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_expand_var.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:49:58 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/20 17:19:24 by bvercaem         ###   ########.fr       */
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

static char	*ms_next_var(const char *str, int *qt)
{
	while (*str && *str != '$')
	{
		if (!*qt && *str == '"')
			*qt = 1;
		else if (*qt && *str == '"')
			*qt = 0;
		if (!*qt && *str == '\'')
			while (*++str != '\'')
				(void) str;
		str++;
	}
	return ((char *) str);
}

static int	ms_add_str(t_token *new, char **str, int *qt)
{
	char	*end;
	char	temp;

	end = ms_next_var(*str, qt);
	if (*end == '$' && ms_end_of_name(end) == end + 1)
		end = ms_next_var(end + 1, qt);
	if (*str == end)
		return (0);
	temp = *end;
	*end = 0;
	if (ms_join_str(new, *str, '0'))
		return (ERR_MALLOC);
	*end = temp;
	*str = end;
	return (0);
}

// always clears 'token', never 'tokens'
int	ms_expand_var(t_darray *tokens, t_token *token)
{
	char	*str;
	t_token	new;
	int		ret;
	int		qt;

	str = token->string;
	ms_init_token(&new);
	ret = 0;
	qt = 0;
	while (!ret && *str)
		if (ms_add_str(&new, &str, &qt) || ms_add_var(tokens, &new, &str, &qt))
			ret = ERR_MALLOC;
	ms_clear_token(token);
	if (!ret && ms_append_tokens_var(tokens, &new, NULL, 0))
		ret = ERR_MALLOC;
	if (ret)
		ms_clear_token(&new);
	return (ret);
}
