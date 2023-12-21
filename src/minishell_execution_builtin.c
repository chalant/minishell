/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_execution_builtin.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 13:25:19 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/21 23:39:48 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	dispatch_builtin(t_command *command, int in_fd, int out_fd)
{
	int		status;
	char	**arguments;

	(void)in_fd;
	status = 0;
	arguments = make_arguments(command, command->command_name);
	if (!arguments)
		return (-1);
	if (ft_strcmp(command->command_name, "echo") == 0)
		status = ms_echo(arguments, out_fd);
	else if (ft_strcmp(command->command_name, "cd") == 0)
		status = ms_cd(command->context, arguments, out_fd);
	else if (ft_strcmp(command->command_name, "pwd") == 0)
		status = ms_pwd(arguments, out_fd);
	else if (ft_strcmp(command->command_name, "export") == 0)
		status = ms_export(command->context, arguments, out_fd);
	else if (ft_strcmp(command->command_name, "env") == 0)
		status = ms_env(out_fd);
	else if (ft_strcmp(command->command_name, "unset") == 0)
		status = ms_unset(command->context, arguments);
	else if (ft_strcmp(command->command_name, "exit") == 0)
		status = ms_exit(command->context, arguments);
	free(arguments);
	return (status);
}

int	execute_builtin(t_command *command)
{
	if (!(command->command_flags & MS_FORKED))
	{
		if (command->output > 0)
			return (dispatch_builtin(command, command->input, command->output));
		else
			return (dispatch_builtin(command, command->input, STDOUT_FILENO));
	}
	else if (command->command_flags & MS_FORKED)
	{
		if (redirect_io(command) < 0)
			exit(1);
		return (dispatch_builtin(command, STDIN_FILENO, STDOUT_FILENO));
	}
	return (1);
}
