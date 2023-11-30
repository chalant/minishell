/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokens.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 14:38:16 by bvercaem          #+#    #+#             */
/*   Updated: 2023/11/30 14:55:10 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_TOKENS_H
# define MINISHELL_TOKENS_H

# define RESERVED_SINGLE "|<>() \n"
# define RESERVED_DOUBLE "<>&|"
# define RESERVED_SKIP " \n"

# define IS_RESERVED 1 << 0
# define IS_QUOTED 1 << 1
# define IS_VAR 1 << 2
# define IS_WILDCARD 1 << 3
# define IS_HEREDOC 1 << 4
# define IS_DELIMITER 1 << 5

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

typedef struct dirent t_dirent;

t_token_info	*ms_token_info(t_token_info *ti, const char *res_single,
	const char *res_double, const char *res_skip);
int				ms_tokeniser(char **input, t_darray *tokens, t_token_info *info);
t_token			*ms_init_token(t_token *token);
void			ms_clear_token(void *token);

void			ms_add_flags_char(t_token *token, char c);
void			ms_add_flags_str(t_token *new);
int				ms_expand_var(t_darray *tokens, t_token *token);
char			*ms_end_of_name(const char *str);
int				ms_expand_wildcard(t_darray *tokens, t_token *token);
void			ms_token_expansion(t_darray *tokens);
void			ms_remove_quotes(char *str, char *mask_exp);

#endif
