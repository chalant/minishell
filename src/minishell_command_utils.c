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

int	clear_command(t_command *command)
{
	ft_darray_delete(command->redirections, clear_redirection);
	ft_darray_delete(command->arguments, NULL);
	free(command->redirections);
	free(command->arguments);
	command->command_name = NULL;
	command->left = NULL;
	command->right = NULL;
	command->command_name = NULL;
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

int	is_builtin(char *command_name)
{
	if (ft_strcmp(command_name, "cd") == 0)
		return (1);
	else if (ft_strcmp(command_name, "echo") == 0)
		return (1);
	else if (ft_strcmp(command_name, "pwd") == 0)
		return (1);
	else if (ft_strcmp(command_name, "export") == 0)
		return (1);
	else if (ft_strcmp(command_name, "env") == 0)
		return (1);
	else if (ft_strcmp(command_name, "unset") == 0)
		return (1);
	else if (ft_strcmp(command_name, "exit") == 0)
		return (1);
	return (0);
}
