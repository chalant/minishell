/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokens.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 14:38:16 by bvercaem          #+#    #+#             */
/*   Updated: 2023/10/16 18:14:21 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_TOKENS_H
# define MINISHELL_TOKENS_H

# define RESERVED_SYMBOLS "|<> "
# define SKIP_SYMBOLS " "
# define DOUBLE_SYMBOLS "<>"

typedef struct s_token
{
	char	*string;
}			t_token;

void	ms_tokeniser(const char *input, t_darray *tokens);
void	ms_clear_token(void *token);

#endif
