/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_execution.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 15:02:24 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/08 16:35:52 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//copies file descriptors
void	copy_pipe(int *src_pipe, int *dest_pipe)
{
	dest_pipe[0] = src_pipe[0];
	dest_pipe[1] = src_pipe[1];
}

/* runs the command as a subprocess, if the command is builtin, we exit
with the returned status, if is not, we exit with an error code. */
int	execute_and(t_command *command, int in_pipe[2], int out_pipe[2])
{
	int	status;

	//todo: checking for the last command is not sufficient!
	if (command->right->command_flags & MS_LAST)
	{
		fprintf(stderr, "last %s\n", command->right->command_name);
		if (out_pipe[1] != -1)
			close(out_pipe[1]);
		out_pipe[1] = -1;
	}
	status = execute_command(command, command->left, in_pipe, out_pipe);
	if (status == 0)
		return (execute_command(command, command->right, in_pipe, out_pipe));
	return (status);
}

int	execute_or(t_command *command, int in_pipe[2], int out_pipe[2])
{
	int	status;

	status = execute_command(command, command->left, in_pipe, out_pipe);
	if (status > 0)
		return (execute_command(command, command->right, in_pipe, out_pipe));
	return (status);
}

//todo: make this cleaner
int	execute_pipe(t_command *command, int in_pipe[2], int out_pipe[2])
{
	int	status;

	if (in_pipe[0] == -1)
	{
		if(in_pipe[1] != -1)
			close(in_pipe[1]);
		if (pipe(in_pipe) < 0)
			return ((ms_perror("pipe", NULL, NULL, errno) - 2));
		//printf("PIPE IN %d %d\n", in_pipe[0], in_pipe[1]);
	}
	if (out_pipe[1] == -1)
	{
		if (out_pipe[0] != -1)
			close(out_pipe[0]);
		if (pipe(out_pipe) < 0)
			return ((ms_perror("pipe", NULL, NULL, errno) - 2));
	}
	status = execute_command(command, command->left, in_pipe, out_pipe);
	close(in_pipe[0]);
	close (in_pipe[1]);
	copy_pipe(out_pipe, in_pipe);
	pipe(out_pipe);
	command->command_flags |= MS_RIGHT;
	status = execute_command(command, command->right, in_pipe, out_pipe);
	return (status);
}


int	execute_command(t_command *parent, t_command *command, int in_pipe[2], int out_pipe[2])
{
	int	status;

	if (!command)
		return (0);
	status = 1;
	if (command->command_flags & MS_OPERAND)
		status = execute_command_core(parent, command, in_pipe, out_pipe);
	else if (command->command_flags & MS_AND)
		status = execute_and(command, in_pipe, out_pipe);
	else if (command->command_flags & MS_OR)
		status = execute_or(command, in_pipe, out_pipe);
	else if (command->command_flags & MS_PIPE)
		status = execute_pipe(command, in_pipe, out_pipe);
	command->context->status = status;
	return (status);
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
		while(++i < command->arguments->size)
			printf("%s ", ((t_token *)ft_darray_get(command->arguments, i))->string);
	}
	i = -1;
	if (command->redirections)
	{
		while(++i < command->redirections->size)
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

	// printf("Commands: \n");
	print_commands(command, 0);
	//todo: we either create a pipe or open a redirection as input here.
	// if (pipe(in_pipe) < 0)
	// 	return (-1);
	out_pipe[0] = -1;
	out_pipe[1] = -1;
	in_pipe[0] = -1;
	in_pipe[1] = -1;
	status = execute_command(NULL, command, in_pipe, out_pipe);
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
