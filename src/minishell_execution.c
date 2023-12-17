/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_execution.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 15:02:24 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/17 16:50:58 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	launch_heredocs(t_command *command, int *id)
{
	if (command->redirections && (command->command_flags & MS_OPERAND || command->command_flags & MS_REDIR))
	{
		*id += 1;
		return (ms_heredoc(command, *id));
	}
	if (command->left)
		launch_heredocs(command->left, id);
	if (command->right)
		launch_heredocs(command->right, id);
	if (command->redirections && command->command_flags & MS_OPERATOR)
	{
		*id += 1;
		ms_heredoc(command, *id);
	}
	return (1);
}

int	distribute_fds(t_command *parent, t_command *command)
{
	if (command && !command->input)
	{
		command->input = parent->input;
		command->command_flags |= MS_I_INJECT;
	}
	if (command && !command->output)
	{
		command->output = parent->output;
		command->command_flags |= MS_O_INJECT;
	}
	return (0);
}

int	execute_and(t_command *parent, t_command *command, int in_pipe[2], int out_pipe[2])
{
	int	status;
	//todo: check if command exists
	//todo: handle errors
	status = handle_redirections(command) < 0;
	if (status)
		return (close_fd(&out_pipe[1]) * status);
	distribute_fds(command, command->left);
	if (!parent || !(command->command_flags & MS_AND))
		close_fd(&out_pipe[1]);
	status = execute_command(command, command->left, in_pipe, out_pipe);
	if (status == 0)
	{
		distribute_fds(command, command->right);
		return (execute_command(command, command->right, in_pipe, out_pipe));
	}
	return (status);
}

int	execute_or(t_command *parent, t_command *command, int in_pipe[2], int out_pipe[2])
{
	int	status;

	status = handle_redirections(command) < 0;
	if (status)
		return (close_fd(&out_pipe[1]) * status);
	distribute_fds(command, command->left);
	if (!parent || !(command->command_flags & MS_OR))
		close_fd(&out_pipe[1]);
	status = execute_command(command, command->left, in_pipe, out_pipe);
	if (status > 0)
	{
		distribute_fds(command, command->right);
		return (execute_command(command, command->right, in_pipe, out_pipe));
	}
	return (status);
}

int	execute_command(t_command *parent, t_command *command, int in_pipe[2], int out_pipe[2])
{
	if (!command)
		return (close_fd(&out_pipe[1]) * 0);
	if (command->command_flags & MS_OPERAND)
		command->context->status = execute_command_core(parent, command, in_pipe, out_pipe);
	else if (command->command_flags & MS_AND)
		command->context->status = execute_and(parent, command, in_pipe, out_pipe);
	else if (command->command_flags & MS_OR)
		command->context->status = execute_or(parent, command, in_pipe, out_pipe);
	else if (command->command_flags & MS_PIPE)
		command->context->status = execute_pipe(parent, command, in_pipe, out_pipe);
	if (g_global_status)
	{
		command->context->status = g_global_status;
		g_global_status = command->context->status;
	}
	return (command->context->status);
}

void	print_commands(t_command *command, int depth)
{
	int	i;
	if (!command)
		return ;
	if (!command->command_name)
		return ;
	for (int i = 0; i < depth; i++)
		printf("   |");
	printf("%s ", command->command_name);
	i = -1;
	if (command->arguments)
	{
		while (++i < command->arguments->size)
			printf("%s ", ((t_token *)ft_darray_get(command->arguments, i))->string);
	}
	i = -1;
	if (command->redirections)
	{
		while (++i < command->redirections->size)
			printf("> %s ", ((t_redirection *)ft_darray_get(command->redirections, i))->file_path);
	}
	printf("input %d output %d\n", command->input, command->output);
	printf("\n");
	if (command->command_flags & MS_OPERAND)
		return ;
	print_commands(command->left, depth + 1);
	print_commands(command->right, depth + 1);
}

int	minishell_execute(t_command *command)
{
	int	in_pipe[2];
	int	out_pipe[2];
	int	status;
	int	hd_id;

	hd_id = 0;
	// printf("Commands: \n");
	//print_commands(command, 0);
	out_pipe[0] = -1;
	out_pipe[1] = -1;
	in_pipe[0] = -1;
	in_pipe[1] = -1;
	launch_heredocs(command, &hd_id);
	if (handle_redirections(command) < 0)
		return (1);
	if (!command->command_name)
		return (0);
	status = execute_command(NULL, command, in_pipe, out_pipe);
	//todo: check if pipes are opened first.
	close_fd(&in_pipe[0]);
	close_fd(&in_pipe[1]);
	close_fd(&out_pipe[0]);
	close_fd(&out_pipe[1]);
	while (wait(NULL) != -1)
		continue ;
	if (!access(MS_HEREDOC_PATH, F_OK))
		if (unlink(MS_HEREDOC_PATH))
			ms_perror("unlink", MS_HEREDOC_PATH, NULL, errno);
	return (status);
}
