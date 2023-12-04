/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_expand_var.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:49:58 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/04 16:32:07 by bvercaem         ###   ########.fr       */
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

// it's advised to pass the pointer of the starting '$'
// returns pointer to right after the name
char	*ms_end_of_name(const char *str)
{
	if (*str == '$')
		str++;
	if ('0' <= *str && *str <= '9')
		return ((char *) str + 1);
	while (('a' <= *str && *str <= 'z') || ('A' <= *str && *str <= 'Z')
		|| *str == '_' || ('0' <= *str && *str <= '9'))
		str++;
	return ((char *) str);
}

// skipped if (!add_len || !fill);
// error: perror, return
static int	ms_join_mask(t_token *token, int add_len, char fill)
{
	char	*new;
	int		old_len;

	if (!add_len || !fill)
		return (0);
	if (!token->mask_exp)
		token->mask_exp = ft_strdup("");
	if (!token->mask_exp)
	{
		ms_perror("expansion", NULL, NULL, errno);
		return (ERR_MALLOC);
	}
	old_len = ft_strlen(token->mask_exp);
	new = malloc(sizeof(char) * (old_len + add_len + 1));
	if (!new)
	{
		ms_perror("expansion", NULL, NULL, errno);
		return (ERR_MALLOC);
	}
	ft_strlcpy(new, token->mask_exp, old_len + 1);
	new[old_len + add_len] = 0;
	while (add_len--)
		new[old_len + add_len] = fill;
	free(token->mask_exp);
	token->mask_exp = new;
	return (0);
}

// never free's str
// error: perror, return
static int	ms_join_str(t_token *token, char *str, char mask)
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

static int	ms_append_tokens_var(t_darray *tokens, t_token *new, char **value, int i)
{
	int	err;

	if (!new->string)
		return (0);
	err = 0;
	ms_add_flags_str(new);
	if (new->flags & IS_WILDCARD)
		err = ms_expand_wildcard(tokens, new);
	else
		err = ft_darray_append(tokens, new);
	if (err)
	{
		if (!(new->flags & IS_WILDCARD))
			ms_perror("expansion", NULL, NULL, errno);
		while (value && value[i])
		{
			free(value[i]);
			i++;
		}
		if (value)
			free(value);
		return (ERR_MALLOC);
	}
	return (0);
}

static char	**ms_handle_getenv(char *str, int *qt)
{
	char	**ret;
	char	*value;

	value = getenv(str);
	if (!value || !*value)
	{
		ret = malloc(sizeof(char *));
		if (ret)
			*ret = NULL;
		return (ret);
	}
	if (!*qt && value)
		return (ft_setsplit(value, " \n"));
	ret = malloc(sizeof(char *) * 2);
	if (!ret)
		return (NULL);
	ret[1] = NULL;
	ret[0] = ft_strdup(value);
	if (!ret[0])
	{
		free(ret);
		return (NULL);
	}
	return (ret);
}

static int	ms_add_var(t_darray *tokens, t_token *new, char **str, int *qt)
{
	char	*end;
	char	temp;
	char	**value;
	int		i;

	end = ms_end_of_name(*str);
	if (*str == end)
		return (0);
	temp = *end;
	*end = 0;
	value = ms_handle_getenv(*str + 1, qt);
	if (!value)
	{
		ms_perror("expansion", NULL, NULL, errno);
		return (ERR_MALLOC);
	}
	*end = temp;
	*str = end;
	if (!*value)
	{
		free(value);
		return (0);
	}
	if (ms_join_str(new, value[0], '1'))
	{
		ft_clear_ds(value);
		return (ERR_MALLOC);
	}
	free(value[0]);
	i = 1;
	while (value[i])
	{
		if (ms_append_tokens_var(tokens, new, value, i))
			return (ERR_MALLOC);
		ms_init_token(new);
		new->string = value[i];
		if (ms_join_mask(new, ft_strlen(new->string), '1'))
		{
			while (value[++i])
				free(value[i]);
			free(value);
			return (ERR_MALLOC);
		}
		i++;
	}
	free(value);
	return (0);
}

static int	ms_add_str(t_token *new, char **str, int *qt)
{
	char	*end;
	char	temp;

	end = ms_next_var(*str, qt);
	if (*end == '$' && ms_end_of_name(end) == end + 1)
		end = ms_next_var(end + 1, qt);
// does that skip enough? maybe while loop?
// consider special vars here maybe (e.g. '?')
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
