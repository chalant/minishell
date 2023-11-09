/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_expand_var.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:49:58 by bvercaem          #+#    #+#             */
/*   Updated: 2023/11/09 16:05:07 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// handles a string pointing to '$' but not truncated
int	ms_add_var(char *token, int index)
{
}

// returns new token length based on index i
static int	ms_length(int i)
{
	static int	old_i = 0;
	int			ret;

	ret = i - old_i;
	old_i = i;
	return (ret);
}

static int	ms_cursor_on_var(char *str)
{
	static int	i = 0;

	while (str[i] && str[i] != '$')
	{
		if (str[i] == '\'')
			while (str[++i] != '\'')
				(void) i;
		i++;
	}
	return (i);
}

// increments str till after var
// returns ERR_MALLOC
static int	ms_cut_var(char **str, char **var)
{
	int	len;

	len = 0;
	while ((*str)[len])
// add var cutoff values
		len++;
	(*var) = malloc(sizeof(char) * (len + 1));
	if (!*var)
		return (ERR_MALLOC);
	ft_strlcpy(*var, *str, len + 1);
	(*str) += len;
	return (0);
}

static int	ms_cut_str(char **str, char **new, int len)
{
// do we cut it before we get a var value?
	(*new) = malloc(sizeof(char) * (len + 1));
	if (!*new)
		return (ERR_MALLOC);
	ft_strlcpy(*new, *str, len + 1);
	(*str) += len;
	return (0);
}

char	*ms_expand_var(t_darray *buf, t_token *token)
{
	char	*str;
	char	*var;
	char	*new;
	int		i;

	str = token->string;
	i = ms_cursor_on_var(str);
// combine with ms_length? just return len and then str += len (no need for statics too)

	if (ms_cut_var(&str, &var))
		return (ERR_MALLOC);
}

// set flag IS_WILDCARD somewhere in expansion

//results should be split on spaces:
//	e.g. abc='a b c';	cat z$abc -> cat 'za'; cat 'b'; cat 'c';
//results should NOT be quote removed:
//	e.g. abc='a" b "c';	cat 'z'$abc -> cat 'za"'; cat 'b'; cat '"c';

// a token can be 'a'$b with b='b' which expands to 'a''b' and results in a'b'
