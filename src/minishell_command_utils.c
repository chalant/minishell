/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_command_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 12:01:23 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/23 10:33:34 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_command(t_command *command)
{
	command->command_flags = 0;
	command->pid = -1;
	command->input = 0;
	command->output = 0;
	command->left = NULL;
	command->right = NULL;
	command->command_name = NULL;
	command->arguments = NULL;
	command->redirections = NULL;
	command->context = NULL;
	command->error = 0;
	command->token = NULL;
	return (1);
}

void	clear_expanded(void *address)
{
	t_token	*token;

	token = (t_token *)address;
	if (!(token->flags & IS_EXPANDED))
		return ;
	free(token->string);
	free(token->mask_exp);
	token->string = NULL;
	token->mask_exp = NULL;
}

int	clear_command(t_command *command)
{
	ft_darray_delete(command->redirections, clear_redirection);
	ft_darray_delete(command->arguments, clear_expanded);
	free(command->redirections);
	free(command->arguments);
	if (command->command_flags & MS_OPERAND)
		free(command->command_name);
	command->command_name = NULL;
	command->left = NULL;
	command->right = NULL;
	command->command_flags = 0;
	if (command->input > 0)
		close(command->input);
	if (command->output > 0)
		close(command->output);
	command->input = 0;
	command->output = 0;
	command->redirections = NULL;
	command->arguments = NULL;
	return (0);
}

int	reset_commands(t_darray *commands)
{
	int	i;

	i = -1;
	while (++i < commands->actual_size)
		clear_command(ft_darray_get(commands, i));
	ft_darray_reset(commands, NULL);
	return (0);
}

int	delete_commands(t_darray *commands)
{
	int	i;

	i = -1;
	while (++i < commands->actual_size)
		clear_command(ft_darray_get(commands, i));
	ft_darray_delete(commands, NULL);
	return (0);
}
