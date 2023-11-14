#include "minishell.h"

//executes the right command when the left command succeedes
int	execute_and(t_command *command, int in_pipe[2], int out_pipe[2])
{
	int	status;

	status = execute_command(command->left, in_pipe, out_pipe);
	if (status == 0)
		return	(execute_command(command->right, in_pipe, out_pipe));
	return (status);
}

//executes right command when the left commands fails.
int	execute_or(t_command *command, int in_pipe[2], int out_pipe[2])
{
	int	status;

	status = execute_command(left, commands, in_pipe, out_pipe);
	if (status > 0)
		return (build_command(right, commands, in_pipe, out_pipe));
	return (status);
}

int	execute_pipe(t_command *command, int in_pipe[2], int out_pipe[2])
{
	status = execute_command(left, commands, in_pipe, out_pipe);
	return (execute_command(right, commands, in_pipe, out_pipe));
}

int	execute_simple_command(t_command *command, int in_pipe[2], int out_pipe[2])
{
	printf("Executed! %s\n", command->command_name);
	return (1);
}

int	execute_command(t_command *command, int in_pipe[2], int out_pipe[2])
{
	if (!command->command_name)
		return (0);
	//printf("Executing %s\n", command->command_name);
	if (command->command_flags & MS_OPERAND)
		return (execute_simple_command(command, in_pipe, out_pipe));
	else if (command->command_flags & MS_OPERATOR)
	{
		if (command->command_flags & MS_AND)
			return (execute_and(command));
		else if (command->command_flags & MS_OR)
			return (execute_or(command));
		else if (command->command_flags & MS_PIPE)
			return (execute_pipe(command));
	}
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
