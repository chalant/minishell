/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokens.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 14:38:16 by bvercaem          #+#    #+#             */
/*   Updated: 2023/11/17 14:37:56 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_TOKENS_H
# define MINISHELL_TOKENS_H

# define RESERVED_SINGLE "|<>() "
# define RESERVED_DOUBLE "<>&|"
# define RESERVED_SKIP " "

# define IS_RESERVED 1 << 0
# define IS_QUOTED 1 << 1
# define IS_VAR 1 << 2
# define IS_WILDCARD 1 << 3

// # define RESERVED_SYMBOLS "+-*/() "
// # define SKIP_SYMBOLS " "
// # define DOUBLE_SYMBOLS "<>&|"

typedef struct s_token
{
	char	*string;
	char	*mask_exp;
	int		flags;
}				t_token;

typedef struct s_token_info
{
	const char	*reserved_single;
	const char	*reserved_double;
	const char	*reserved_skip;
}				t_token_info;

int		ms_tokeniser(const char *input, t_darray *tokens, t_token_info *info);
t_token	*ms_init_token(t_token *token);
void	ms_clear_token(void *token);

int		ms_expand_var(t_darray *tokens, t_token *token);
int		ms_expand_wildcard(t_darray *tokens, t_token *token);
void	ms_token_expansion(t_darray *tokens);
void	ms_remove_quotes(char *str, char *mask_exp);

void	ms_print_tokens(t_darray *tokens);

#endif
