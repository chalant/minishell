/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_expand_var.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:49:58 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/26 19:14:42 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	ms_decide_qt_or_special(char *str, int qt, t_ms_context *data)
{
	if (str[1] == '?')
		return (data->status);
	return (qt);
}

// always clears 'token', never 'tokens'
// error: ERR_MALLOC
int	ms_expand_var(t_darray *tokens, t_token *token, t_ms_context *data)
{
	char	*str;
	t_token	new;
	int		ret;
	int		qt;
	int		qt_or_special;

	str = token->string;
	ms_init_token(&new);
	ret = 0;
	qt = 0;
	while (!ret && *str)
	{
		if (ms_add_str(&new, &str, &qt))
			ret = ERR_MALLOC;
		qt_or_special = ms_decide_qt_or_special(str, qt, data);
		if (!ret && ms_add_var(tokens, &new, &str, &qt_or_special))
			ret = ERR_MALLOC;
	}
	ms_clear_token(token);
	if (!ret && ms_add_tkn_var(tokens, &new, NULL, 0))
		ret = ERR_MALLOC;
	if (ret)
		ms_clear_token(&new);
	return (ret);
}
