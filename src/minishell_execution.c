/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_execution.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 15:02:24 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/14 17:37:00 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//copies file descriptors
void	copy_pipe(int *src_pipe, int *dest_pipe)
{
	dest_pipe[0] = src_pipe[0];
	dest_pipe[1] = src_pipe[1];
}

int	launch_heredocs(t_command *command, int *id)
{
	if (command->redirections)
	{
		*id += 1;
		return (ms_heredoc(command->redirections, *id));
	}
	launch_heredocs(command->left, id);
	launch_heredocs(command->right, id);
	return (1);
}

/* runs the command as a subprocess, if the command is builtin, we exit
with the returned status, if is not, we exit with an error code. */
int	execute_and(t_command *parent, t_command *command, int in_pipe[2], int out_pipe[2])
{
	int	status;
	//todo: check if command exists
	//todo: handle errors
	if (command->redirections && command->redirections->size)
	{
		create_files(command, command->redirections);
	}
	if (command->left && !command->left->input)
		command->left->input = command->input;
	if (command->left && !command->left->output)
		command->left->output = command->output;
	if (command->right && !command->right->input)
		command->right->input = command->input;
	if (command->right && !command->right->output)
		command->right->output = command->output;
	status = execute_command(command, command->left, in_pipe, out_pipe);
	if (!parent || !(command->command_flags & MS_AND))
	{
		if (out_pipe[1] != -1)
			close(out_pipe[1]);
		out_pipe[1] = -1;
	}
	if (status == 0)
		return (execute_command(command, command->right, in_pipe, out_pipe));
	return (status);
}

//todo: if the left command fails, we shouldn't close the fds... how ?
int	execute_or(t_command *parent, t_command *command, int in_pipe[2], int out_pipe[2])
{
	int	status;

	//todo: handle errors
	if (command->redirections && command->redirections->size)
	{
		create_files(command, command->redirections);
	}
	if (!command->left->input)
		command->left->input = command->input;
	if (!command->left->output)
		command->left->output = command->output;
	if (!command->right->input)
		command->right->input = command->input;
	if (!command->right->output)
		command->right->output = command->output;
	if (!parent || !(command->command_flags & MS_OR))
	{
		if (out_pipe[1] != -1)
			close(out_pipe[1]);
		out_pipe[1] = -1;
	}
	status = execute_command(command, command->left, in_pipe, out_pipe);
	if (status > 0)
		return (execute_command(command, command->right, in_pipe, out_pipe));
	return (status);
}

int	execute_pipe(t_command *parent, t_command *command, int in_pipe[2], int out_pipe[2])
{
	if (out_pipe[1] == -1)
	{
		if (out_pipe[0] != -1)
			close(out_pipe[0]);
		if (out_pipe[1] != -1)
			close(out_pipe[1]);
		if (pipe(out_pipe) < 0)
			return ((ms_perror("pipe", NULL, NULL, errno) - 2));
	}
	// //todo: handle errors
	if (command->redirections && command->redirections->size)
	{
		create_files(command, command->redirections);
	}
	if (command->left && !command->left->input)
		command->left->input = command->input;
	//todo: if the left command fails, close and return -1;
	//todo: heredoc should recursively go down the tree and call the heredocs
	// if (command->left && command->left->redirections)
	// 	ms_heredoc(command->left->redirections);
	execute_command(command, command->left, in_pipe, out_pipe);
	// if (command->right && command->right->redirections)
	// 	ms_heredoc(command->right->redirections);
	//todo: crash if it is not EAGAIN (return -1)
	if (command && command->error == EAGAIN && wait(NULL) != -1)
		execute_command(command, command->left, in_pipe, out_pipe);
	else if (command && command->error == -1)
		return (-1);
	copy_pipe(out_pipe, in_pipe);
	pipe(out_pipe);
	if (!parent || !(parent->command_flags & MS_PIPE))
	{
		close(out_pipe[1]);
		out_pipe[1] = -1;
	}
	if (command->right && !command->right->output)
		command->right->output = command->output;
	
	//todo: if there is no left command, the right command should'nt redirect in.
	// if (!command->left || command->left->command_flags & MS_REDIR)
	// 	command->right->redirections = NULL;
	//todo: if the parent is not a pipe, the right command should pipe to .
	if (command->command_flags & MS_LAST)
	{
		command->command_flags &= ~(MS_LAST);
		command->right->command_flags |= MS_LAST;
	}
	if (parent && (!(parent->command_flags & MS_PIPE) || command->command_flags & MS_LAST))
	{
		command->command_flags &= ~(MS_LAST);
		parent->command_flags &= ~(MS_LAST);
		command->right->command_flags |= MS_LAST;
	}
	//todo: crash if it is not EAGAIN (return -1)
	execute_command(command, command->right, in_pipe, out_pipe);
	if (command && command->error == EAGAIN && wait(NULL) != -1)
		execute_command(command, command->right, in_pipe, out_pipe);
	else if (command && command->error == -1)
		return (-1);
	if ((parent && !(parent->command_flags & MS_PIPE)) || (command->right && command->right->command_flags & MS_LAST))
		return (get_exit_status(command->pid));
	return (0);
}


int	execute_command(t_command *parent, t_command *command, int in_pipe[2], int out_pipe[2])
{
	if (!command)
	{
		close(out_pipe[1]);
		out_pipe[1] = -1;
		return (0);
	}
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
		g_global_status = 0;
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
	print_commands(command, 0);
	out_pipe[0] = -1;
	out_pipe[1] = -1;
	in_pipe[0] = -1;
	in_pipe[1] = -1;
	launch_heredocs(command, &hd_id);
	status = execute_command(NULL, command, in_pipe, out_pipe);
	//todo: check if pipes are opened first.
	close(in_pipe[0]);
	close(in_pipe[1]);
	close(out_pipe[0]);
	close(out_pipe[1]);
	while (wait(NULL) != -1)
		continue ;
	if (!access(MS_HEREDOC_PATH, F_OK))
		if (unlink(MS_HEREDOC_PATH))
			ms_perror("unlink", MS_HEREDOC_PATH, NULL, errno);
	return (status);
}
