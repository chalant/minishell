/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_execution_expansion.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 12:09:39 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/22 13:55:30 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//(bvercaem) insert expansion code here please :)
//IMPORTANT NOTE: this should not free this token, also don't add this token in the array
//other-wise it might get double freed. this token is freed later.
int expand_token(t_token *token, t_darray *into)
{
	return (1);
}

//adds elements at the begging of the array.
int prepend_arguments(t_darray *arguments, t_darray *into)
{
	ft_darray_reverse(into);
	ft_darray_reverse(arguments);
	arguments->size = arguments->size - 1;
	ft_darray_join(into, arguments);
	ft_darray_reverse(into);
}

int expand_arguments(t_darray *arguments, t_darray *tmp)
{
	int			i;
	char		*arguments;
	t_token		*token;
	t_darray	remainder;

	//todo: handle errors.
	if (ft_darray_init(&remainder, arguments->type_size, 10) < 0)
		return (-1);
	i = -1;
	while (++i < arguments->size)
	{
		token = ft_darray_get(arguments, i);
		expand_token(token, tmp);
		ft_darray_insert(arguments, &tmp, &remainder, i);
		ft_darray_reset(&tmp, NULL);
	}
}

int expand_redirection(t_redirection *redirection)
{
	t_darray		tmp;
	t_redirection	*redirection;

	//todo: return if there is no expansion needed.
	if (expand_token(redirection->token, &tmp) < 0)
		return (-1);
	//not sure about these conditions
	if (tmp.size > 1 || tmp.size == 0)
	{
		ft_darray_delete(&tmp, ms_clear_token);
		return (ms_perror(redirection->token->string, 
			NULL, "ambiguous redirect", 0) * -1);
	}
	//todo: might need to free this, but avoid double frees!
	redirection->file_path = ft_darray_get(&tmp, 0);
	redirection->redirection_flags |= MS_FREE;
}

int expand_command_fields(t_command *command)
{
	t_darray	tmp;

	//todo: check if it should be expanded... return outher-wise.
	if (ft_darray_init(&tmp, sizeof(t_token), 10) < 0)
		return (-1);
	expand_token(command->token, &tmp);
	//this will be freed when clearing the arguments.
	command->command_name = ((t_token *)ft_darray_get(&tmp, 0))->string;
	prepend_arguments(&tmp, command->arguments);
	ft_darray_reset(&tmp, NULL);
	expand_arguments(command->arguments, &tmp);
}
