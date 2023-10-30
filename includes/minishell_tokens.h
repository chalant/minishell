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

# define RESERVED_SYMBOLS "|&<>() "
# define SKIP_SYMBOLS " "
# define DOUBLE_SYMBOLS "<>&|"

// # define RESERVED_SYMBOLS "+-*/() "
// # define SKIP_SYMBOLS " "
// # define DOUBLE_SYMBOLS "<>&|"

typedef struct	s_token
{
	char	*string;
	int		is_reserved;
}				t_token;

typedef	struct	s_token_info
{
	const char	reserved_symbols;
	const char	skip_symbols;
	const char	double_symbols;
}				t_token_info;


void	ms_tokeniser(const char *input, t_darray *tokens);
void	ms_clear_token(void *token);
void	ms_print_tokens(t_darray *tokens);

#endif
