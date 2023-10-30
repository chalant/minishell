/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokens.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 14:38:16 by bvercaem          #+#    #+#             */
/*   Updated: 2023/10/30 14:09:48 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_TOKENS_H
# define MINISHELL_TOKENS_H

# define RESERVED_SYMBOLS "|&<>() "
# define SKIP_SYMBOLS " "
# define DOUBLE_SYMBOLS "<>&|"

# define IS_RESERVED 1 << 0
# define IS_QUOTED 1 << 1

// # define RESERVED_SYMBOLS "+-*/() "
// # define SKIP_SYMBOLS " "
// # define DOUBLE_SYMBOLS "<>&|"

typedef struct	s_token
{
	char	*string;
	int		flags;
}				t_token;

typedef	struct	s_tokenizer_info
{
	const char	reserved_symbols;
	const char	skip_symbols;
	const char	double_symbols;
}				t_tokenizer_info;


int		ms_tokeniser(const char *input, t_darray *tokens);

void	ms_clear_token(void *token);
void	ms_print_tokens(t_darray *tokens);

#endif
