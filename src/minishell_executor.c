#include "minishell.h"

int	execute_and(t_command *command, int in_pipe[2], int out_pipe[2])
{
	int	status;

	status = execute_command(command, in_pipe, out_pipe);
	if (status == 0)
		return (execute_command(command, in_pipe, out_pipe));
	return (status);
}

int	execute_or(t_command *command, int in_pipe[2], int out_pipe[2])
{
	int	status;

	status = execute_command(command, in_pipe, out_pipe);
	if (status > 0)
		return (execute_command(command, in_pipe, out_pipe));
	return (status);
}

//copies file descriptors
void	dup_pipe(int src_pipe[2], int dest_pipe[2])
{
	dest_pipe[0] = src_pipe[0];
	dest_pipe[1] = src_pipe[1];
}

int	execute_pipe(t_command *command, int in_pipe[2], int out_pipe[2])
{
	int	pid;

	if (pipe(out_pipe) < 0)
		return (-1);
	pid = execute_process(command->left, in_pipe, out_pipe);
	close_read_write(in_pipe);
	dup_pipe(out_pipe, in_pipe);
	//todo: out_pipe could be a redirection.
	if (pipe(out_pipe) < 0)
		return (-1);
	pid = execute_process(command->right, in_pipe, out_pipe);
	close_read_write(in_pipe);
	dup_pipe(out_pipe, in_pipe);
	return (1);
}

int	execute_simple_command(t_command *command, int in_pipe[2], int out_pipe[2])
{
	return (1);
}

int	execute_command(t_command *command, int in_pipe[2], int out_pipe[2])
{
	if (command->command_flags & MS_OPERAND)
		return (execute_simple_command(command, in_pipe, out_pipe));
	else if (command->command_flags & MS_AND)
		return (execute_and(command, in_pipe, out_pipe));
	else if (command->command_flags & MS_OR)
	return (1);
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
	//int			i;
	//int			status;
	//int			in_pipe[2];
	//int			out_pipe[2];

	//status = -1;
	// while (commands->size)
	// {
		// if (pipe(out_pipe) < 0)
		// 	return (minishell_error());
		//close_read_write(in_pipe);
		//dup_pipe(out_pipe, in_pipe);
	// }
	print_commands(command, 0);
	return (0);
}
