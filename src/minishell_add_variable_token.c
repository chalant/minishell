/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_add_variable_token.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 19:43:52 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/20 18:47:45 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_add_tkn_var(t_darray *tokens, t_token *new, char **value, int i)
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

static int	ms_add_var_return(char **value, int ret, char print)
{
	ft_clear_ds(value);
	if (print)
		ms_perror("expansion", NULL, NULL, errno);
	return (ret);
}

static int	ms_append_value_tokens(t_darray *tokens, t_token *new, char **value)
{
	int	i;

	i = 1;
	while (value[i])
	{
		if (ms_add_tkn_var(tokens, new, value, i))
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

int	ms_add_var(t_darray *tokens, t_token *new, char **str, int *qt)
{
	char	*end;
	char	temp;
	char	**value;
	char	mask_fill;

	end = ms_end_of_name(*str);
	if (end == *str || end == *str + 1)
		return (0);
	mask_fill = '1';
	temp = *end;
	*end = 0;
	value = ms_malloc_getenv(*str + 1, qt, &mask_fill, new);
	if (!value)
		return (ms_add_var_return(NULL, ERR_MALLOC, 1));
	*end = temp;
	*str = end;
	if (!*value)
		return (ms_add_var_return(value, 0, 0));
	if (ms_join_str(new, value[0], mask_fill))
		return (ms_add_var_return(value, ERR_MALLOC, 0));
	free(value[0]);
	return (ms_append_value_tokens(tokens, new, value));
}
