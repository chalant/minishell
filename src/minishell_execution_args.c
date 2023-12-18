/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_execution_args.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 12:01:03 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/18 13:40:51 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	fill_in_status(char *status, t_token *token, int *j)
{
	if (!token->string[*j])
		return ;
	token->string[*j] = *status;
	(*j)++;
	status++;
	if (*status)
	{
		token->string[*j] = *status;
		(*j)++;
		status++;
	}
	else
		ms_shift_strings(token->string, token->mask_exp, *j);
	if (*status)
	{
		token->string[*j] = *status;
		(*j)++;
		status++;
	}
	else
		ms_shift_strings(token->string, token->mask_exp, *j);
}

static char	*make_argi(t_command *command, int i)
{
	t_token	*token;
	int		j;
	char	*status;

	token = ft_darray_get(command->arguments, i - 1);
	if (token->flags & IS_SPECIAL)
	{
		status = ft_itoa((unsigned char) command->context->status);
		if (!status)
			return (NULL);
		j = 0;
		while (token->string[j])
		{
			while (token->string[j] && token->mask_exp[j] != '3')
				j++;
			fill_in_status(status, token, &j);
		}
		free(status);
	}
	return (token->string);
}

// returns NULL on malloc fail
char	**make_arguments(t_command *command, char *command_name)
{
	int			nargs;
	int			i;
	char		**arguments;

	nargs = 2;
	if (command->arguments)
		nargs = command->arguments->size + 2;
	arguments = malloc(sizeof(char *) * nargs);
	if (!arguments)
		return (NULL);
	arguments[0] = command_name;
	i = 0;
	while (++i < nargs - 1)
	{
		arguments[i] = make_argi(command, i);
		if (!arguments[i])
		{
			free(arguments);
			return (NULL);
		}
	}
	arguments[i] = NULL;
	return (arguments);
}
