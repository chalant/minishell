/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_tokeniser_parser.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 18:16:14 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/13 18:25:36 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// returns 1 if *symbol is a reserved sequence
static int	ms_is_reserved(const char *symbol, t_token_info *info)
{
	if (ft_strchr(info->reserved_single, *symbol))
		return (1);
	if (ft_strchr(info->reserved_double, *symbol) && symbol[0] == symbol[1])
		return (1);
	return (0);
}

// on error, *line = NULL
static int	ms_prompt_quote(char **end, char **start, char **line)
{
	char	*temp;
	char	*add_line;
	size_t	index[2];

	index[0] = *start - *line;
	index[1] = *end - *line;
	temp = ft_strjoin(*line, "\n");
	free(*line);
	*line = NULL;
	if (!temp)
		return (ms_perror("tokenising", NULL, NULL, errno));
	add_line = readline("> ");
	if (!add_line)
	{
		free(temp);
		return (ERR_EOF_QUOTED);
	}
	*line = ft_strjoin(temp, add_line);
	free(temp);
	free(add_line);
	if (!*line)
		return (ms_perror("tokenising", NULL, NULL, errno));
	*start = (*line) + index[0];
	*end = (*line) + index[1];
	return (0);
}

// returns 1 on error, and *line = NULL
// returns the next instance of *end
// adds the IS_VAR flag if applicable
static int	ms_skip_quoted(t_token *tkn, char **end, char **start, char **line)
{
	char	quote;
	int		check;

	tkn->flags |= IS_QUOTED;
	quote = **end;
	check = 0;
	while (!check)
	{
		while (**end)
		{
			(*end)++;
			if (**end == quote)
				return (0);
			if (quote == '"')
				if (**end == '$')
					tkn->flags |= IS_VAR;
		}
		check = ms_prompt_quote(end, start, line);
	}
	if (check != ERR_EOF_QUOTED)
		return (check);
	write(2, "shellshock: unexpected EOF while looking for matching `", 55);
	write(STDERR_FILENO, &quote, 1);
	write(STDERR_FILENO, "'\n", 2);
	return (check);
}

// error: 1, prints msg
// tokeniser: moves *end until a reserved sequence
int	ms_until_reserved(
	char **input, char **start_end, t_token *token, t_token_info *info)
{
	start_end[0] = start_end[1];
	while (*(start_end[1]) && !ms_is_reserved(start_end[1], info))
	{
		if (*(start_end[1]) == '"' || *(start_end[1]) == '\'')
			if (ms_skip_quoted(token, start_end + 1, start_end, input))
				return (1);
		ms_add_flags_char(token, *(start_end[1]));
		start_end[1]++;
	}
	return (0);
}
