#include "minishell.h"

int	get_exit_status(int pid)
{
	int	status;

	status = 0;
	waitpid(pid, &status, 2);
	//todo: we most likely don't need to wait for all processes to end.
	// while (wait(NULL) != -1)
	// 	continue ;
	if (((status) & 0x7f) == 0)
		return ((((status) & 0xff00) >> 8));
	return (0);
}

//copies file descriptors
void	dup_pipe(int src_pipe[2], int dest_pipe[2])
{
	dest_pipe[0] = src_pipe[0];
	dest_pipe[1] = src_pipe[1];
}

/* runs the command as a subprocess, if the command is builtin, we exit
with the returned status, if is not, we exit with an error code. */
int	execute_process(t_command *command, int in_pipe[2], int out_pipe[2])
{
	int	pid;
	int	status;

	pid = fork();
	//todo: maybe print an error ?
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		status = execute_command(command, in_pipe, out_pipe);
		if (command->command_flags & MS_BUILTIN)
			exit(status);
		exit(126);
	}
	return (pid);
}

int	execute_and(t_command *command, int in_pipe[2], int out_pipe[2])
{
	int	status;

	//todo: if there are redirections, the out_pipe has
	//a write file descriptor in append mode.
	status = execute_command(command->left, in_pipe, out_pipe);
	if (status == 0)
		return (execute_command(command->right, in_pipe, out_pipe));
	return (status);
}

int	execute_or(t_command *command, int in_pipe[2], int out_pipe[2])
{
	int	status;

	//todo: if there are redirections, the out_pipe has
	//a write file descriptor in append mode.
	status = execute_command(command->left, in_pipe, out_pipe);
	if (status > 0)
		return (execute_command(command->right, in_pipe, out_pipe));
	return (status);
}

int	execute_pipe(t_command *command, int in_pipe[2], int out_pipe[2])
{
	int	pid;

	//todo: out_pipe could also be a redirection so we need to either create a pipe or open a file...
	if (pipe(out_pipe) < 0)
		return (-1);
	pid = execute_process(command->left, in_pipe, out_pipe);
	//close_read_write(in_pipe);
	dup_pipe(out_pipe, in_pipe);
	//todo: out_pipe could also be a redirection so we need to either create a pipe or open a file...
	if (pipe(out_pipe) < 0)
		return (-1);
	pid = execute_process(command->right, in_pipe, out_pipe);
	//close_read_write(in_pipe);
	dup_pipe(out_pipe, in_pipe);
	return (get_exit_status(pid));
}

//this is the core execution function.
int	execute_simple_command(t_command *command, int in_pipe[2], int out_pipe[2])
{
	//todo: if the simple command has a redirection, it will not write to the write-end of the out_pipe.
	(void)in_pipe;
	(void)out_pipe;
	//todo: execute then return the status of the execution. in case of an execve, this will exit.
	printf("Executing simple command! %s\n", command->command_name);
	return (0);
}

int	execute_command(t_command *command, int in_pipe[2], int out_pipe[2])
{
	if (command->command_flags & MS_OPERAND)
		return (execute_simple_command(command, in_pipe, out_pipe));
	//todo: if the operand have redirections, open them in append mode, and pass them to the write-end of the out_pipe.
	else if (command->command_flags & MS_AND)
		return (execute_and(command, in_pipe, out_pipe));
	else if (command->command_flags & MS_OR)
		return (execute_or(command, in_pipe, out_pipe));
	else if (command->command_flags & MS_PIPE)
		return (execute_pipe(command, in_pipe, out_pipe));
	return (-1);
}

void	print_commands(t_command *command, int depth)
{
	if (!command->command_name)
		return ;
	for (int i = 0; i < depth; i++)
		printf("   |");
	printf("%s\n", command->command_name);
	if (command->command_flags & MS_OPERAND)
		return ;
	print_commands(command->left, depth + 1);
	print_commands(command->right, depth + 1);
}

//executes instructions by going through the stack.
int	minishell_execute(t_command *command)
{
	int			in_pipe[2];
	int			out_pipe[2];

	print_commands(command, 0);
	//todo: we either create a pipe or open a redirection as input here.
	if (pipe(in_pipe) < 0)
		return (-1);
	execute_command(command, in_pipe, out_pipe);
	return (0);
}
