/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokens.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 14:38:16 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/23 17:15:27 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_TOKENS_H
# define MINISHELL_TOKENS_H

# define RESERVED_SINGLE "|<>() \n\t\v\f\r"
# define RESERVED_DOUBLE "<>&|"
// used in the tokeniser and var expansion:
# define RESERVED_SKIP " \n\t\v\f\r"

# define SPECIAL_VAR "?"

# define IS_RESERVED 1 << 0
# define IS_QUOTED 1 << 1
# define IS_VAR 1 << 2
# define IS_WILDCARD 1 << 3
# define IS_HEREDOC 1 << 4
# define IS_DELIMITER 1 << 5
# define IS_EOF 1 << 7
# define IS_EXPANDED 1 << 8

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
}				t_tkn_info;

typedef struct dirent	t_dirent;

typedef struct s_wildcard_data
{
	char	*card;
	char	*name;
	char	*mask;
}			t_wildcard_data;

t_tkn_info	*ms_token_info(t_tkn_info *ti, const char *res_single,
				const char *res_double, const char *res_skip);
int			ms_tokeniser(char **input, t_darray *tokens,
				t_tkn_info *info);
int			ms_until_reserved(char **input, char **start_end,
				t_token *token, t_tkn_info *info);
t_token		*ms_init_token(t_token *token);
void		ms_clear_token(void *token);

void		ms_add_flags_char(t_token *token, char c);
void		ms_add_flags_str(t_token *new);
int			ms_expand_var(t_darray *tokens, t_token *token);
int			ms_add_var(t_darray *tokens, t_token *new, char **str, int *qt);
int			ms_add_tkn_var(t_darray *tokens, t_token *new, char **value, int i);
int			ms_join_str(t_token *token, char *str, char mask);
int			ms_join_mask(t_token *token, int add_len, char fill);
char		*ms_end_of_name(const char *str);
char		**ms_malloc_getenv(char *str, int *qt);
int			ms_expand_wildcard(t_darray *tokens, t_token *token);
int			ms_pre_check(t_token *token, t_dirent *entryp);
int			ms_wildcard_cmp(t_dirent *entryp, t_token *token, t_token *new);
int			ms_cmp_until_wc(t_wildcard_data *d, int *qts, t_token *token);
int			ms_cpy_mask(int n, char **mask, char *card, t_token *token);
int			ms_wildcard_add(t_darray *tokens, t_dirent *entryp, t_token *new);
void		ms_quote_expansion(t_darray *tokens, int start_index);
void		ms_remove_quotes(char *str, char *mask_exp);
void		ms_shift_strings(char *a, char *b, int i);

#endif
