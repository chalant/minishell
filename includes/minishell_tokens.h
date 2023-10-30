/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokens.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 14:38:16 by bvercaem          #+#    #+#             */
/*   Updated: 2023/10/30 17:57:58 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_TOKENS_H
# define MINISHELL_TOKENS_H

# define RESERVED_SINGLE "|<>() "
# define RESERVED_DOUBLE "<>&|"

# define RESERVED_SKIP " "

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

typedef	struct	s_tokeniser_info
{
	const char	*reserved_single;
	const char	*reserved_double;
	const char	*reserved_skip;
}				t_tokeniser_info;


int		ms_tokeniser(const char *input, t_darray *tokens, t_tokeniser_info *info);

void	ms_clear_token(void *token);
void	ms_print_tokens(t_darray *tokens);

#endif
