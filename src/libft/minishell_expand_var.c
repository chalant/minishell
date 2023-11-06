/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_expand_var.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:49:58 by bvercaem          #+#    #+#             */
/*   Updated: 2023/11/06 18:13:48 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// handles a string pointing to '$' but not truncated
int	ms_add_var(char *token, int index)
{
	//add var to a darray with struct that contains:
	//name start index, name end index, value, value_len
}

char	*ms_expand_var(t_token *token)
{

}

// results should be split on spaces		e.g. abc='a b c';	cat z$abc -> cat 'a'; cat 'b'; cat 'c';
// results should NOT be quote removed		e.g. abc='a" b "c';	cat 'z'$abc -> cat 'a"'; cat 'b'; cat '"c';

// a token can be 'a'$b with b='b' which should result in a'b'
