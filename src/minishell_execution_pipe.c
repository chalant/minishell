#include "minishell.h"

//copies file descriptors
void	copy_pipe(int *src_pipe, int *dest_pipe)
{
	dest_pipe[0] = src_pipe[0];
	dest_pipe[1] = src_pipe[1];
}

int	execute_pipe(t_command *parent, t_command *command, int in_pipe[2], int out_pipe[2])
{
    int status;

    status = 0;
	if (out_pipe[1] == -1)
	{
		close_fd(&out_pipe[0]);
		close_fd(&out_pipe[1]);
		if (pipe(out_pipe) < 0)
			return ((ms_perror("pipe", NULL, NULL, errno) * -1));
	}
	// //todo: handle errors
	if (command->redirections && command->redirections->size && create_files(command, command->redirections) < 0)
        return (1);
	if (command->left && !command->left->input)
		command->left->input = command->input;
	//todo: if the left command fails, close and return -1;
	execute_command(command, command->left, in_pipe, out_pipe);
	//todo: crash if it is not EAGAIN (return -1)
	if (command && command->error == EAGAIN && wait(NULL) != -1)
		execute_command(command, command->left, in_pipe, out_pipe);
	else if (command && command->error == -1)
		return (-1);
	copy_pipe(out_pipe, in_pipe);
	if (pipe(out_pipe) < 0)
		return ((ms_perror("pipe", NULL, NULL, errno) * -1));
	if (!parent || !(parent->command_flags & MS_PIPE))
		close_fd(&out_pipe[1]);
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
		status = get_exit_status(command->pid);
    if (status)
		return (close_fd(&out_pipe[1]));
	return (0);
}
