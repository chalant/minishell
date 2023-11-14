#include "minishell.h"
#include "minishell_commands.h"

// int	execute_and(t_command *left, t_command *right, t_stack *commands, int in_pipe[2], int out_pipe[2], int status)
// {
// 	status = build_command(left, commands, in_pipe, out_pipe, -1);
// 	//if (status == 0)
// 	return (build_command(right, commands, in_pipe, out_pipe, -1));
// 	//todo should also pop the commands without executing when they are not executed. ?
// 	return (status);
// }

// int	execute_or(t_command *left, t_command *right, t_stack *commands, int in_pipe[2], int out_pipe[2], int status)
// {
// 	status = build_command(left, commands, in_pipe, out_pipe, -1);
// 	//if (status > 0)
// 	return (build_command(right, commands, in_pipe, out_pipe, -1));
// 	//todo should also pop the commands without executing when they are not executed. ?
// 	return (status);
// }

// int	execute_pipe(t_command *left, t_command *right, t_stack *commands, int in_pipe[2], int out_pipe[2], int status)
// {
// 	//skip
// 	// printf("Executing OR!\n");
// 	status = build_command(left, commands, in_pipe, out_pipe, -1);
// 	//if (status > 0)
// 	return (build_command(right, commands, in_pipe, out_pipe, -1));
// 	//todo should also pop the commands without executing when they are not executed. ?
// 	return (status);
// }

// int	execute_simple_command()
// {
// 	return (1);
// }

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
