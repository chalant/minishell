#include "minishell.h"
//todo: add prompting.

int	ms_match_newline(t_ms_symbol *symbol, t_token *token)
{
	(void)symbol;

	if (!token)
		return (0);
	if (*token->string == '\n')
		return (1);
	return (0);
}

int	ms_match_subset(t_ms_symbol *symbol, t_token *token)
{
	int	i;

	i = 0;
	if (!token->string)
		return (0);
	while (token->string[i])
	{
		if (!ft_strchr(symbol->name, token->string[i]))
			return (0);
		i++;
	}
	return (1);
}

int ms_match_equal(t_ms_symbol *symbol, t_token *token)
{
	size_t	len;

	if (!token->string)
		return (0);
	len = ft_strlen(symbol->name);
	if (ft_strlen(token->string) != len)
		return (0);
	if (ft_strncmp(symbol->name, token->string, len) == 0)
		return (1);
	return (0);
}

//todo: implement integer matching
int	ms_match_integer(t_ms_symbol *symbol, t_token *token)
{
	(void)symbol;
	(void)token;
	return (0);
}

int	is_builtin(t_token *token)
{
	//todo: add more builtins
	if (strcmp(token->string, "cd") == 0)
		return (1);
	else if (strcmp(token->string, "echo") == 0)
		return (1);
	else if (strcmp(token->string, "pwd") == 0)
		return (1);
	// else if (strcmp(token->string, "export") == 0)
	// 	return (1);
	else if (strcmp(token->string, "env") == 0)
		return (1);
	else if (strcmp(token->string, "unset") == 0)
		return (1);
	else if (strcmp(token->string, "exit") == 0)
		return (1);
	return (0);
}

int	ms_match_string(t_ms_symbol *symbol, t_token *token)
{
	(void)symbol;
	int	i;

	if (!token->string)
		return (0);
	i = 0;
	if (token->flags & IS_RESERVED)
		return (0);
	if (is_builtin(token))
		return (0);
	while (token->string[i])
	{
		if (!ft_isascii(token->string[i]))
			return (0);
		i++;
	}
	return (1);
}
