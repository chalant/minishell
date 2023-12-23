/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_execution_args.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 12:01:03 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/23 17:33:01 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// there used to be a lot more in this ft lol (? expansion)
static char	*make_argi(t_command *command, int i)
{
	t_token	*token;

	token = ft_darray_get(command->arguments, i - 1);
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
