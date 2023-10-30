#include "minishell.h"

int	ms_match_newline(t_ms_symbol *symbol, t_token *token)
{
	(void)symbol;

	if (!token)
		return (0);
	printf("Match new liiiiiiiiiiiiiiiiiine! %s\n", token->string);
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
	//printf("Matching %s\n", token->string);
	len = ft_strlen(symbol->name);
	if (ft_strlen(symbol->name) != len)
		return (0);
	if (ft_strnstr(symbol->name, token->string, len))
		return (1);
	return (0);
}

int	ms_match_integer(t_ms_symbol *symbol, t_token *token)
{
	(void)symbol;
	(void)token;
	return (0);
}

int	ms_match_string(t_ms_symbol *symbol, t_token *token)
{
	(void)symbol;
	int	i;

	if (!token->string)
		return (0);
	i = 0;
	//printf("Macthing %s\n", token->string);
	//todo: also check if it is not a reserved symbol!
	//put it in the token struct.
	while (token->string[i])
	{
		if (!ft_isalpha(token->string[i]))
			return (0);
		i++;
	}
	return (1);
}
