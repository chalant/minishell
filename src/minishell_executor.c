#include "minishell.h"
#include "minishell_commands.h"

int	execute_and(t_command *left, t_command *right, t_stack *commands, int in_pipe[2], int out_pipe[2], int status)
{
	status = build_command(left, commands, in_pipe, out_pipe, -1);
	//if (status == 0)
	return (build_command(right, commands, in_pipe, out_pipe, -1));
	//todo should also pop the commands without executing when they are not executed. ?
	return (status);
}

int	execute_or(t_command *left, t_command *right, t_stack *commands, int in_pipe[2], int out_pipe[2], int status)
{
	status = build_command(left, commands, in_pipe, out_pipe, -1);
	//if (status > 0)
	return (build_command(right, commands, in_pipe, out_pipe, -1));
	//todo should also pop the commands without executing when they are not executed. ?
	return (status);
}

int	execute_pipe(t_command *left, t_command *right, t_stack *commands, int in_pipe[2], int out_pipe[2], int status)
{
	//skip
	// printf("Executing OR!\n");
	status = build_command(left, commands, in_pipe, out_pipe, -1);
	//if (status > 0)
	return (build_command(right, commands, in_pipe, out_pipe, -1));
	//todo should also pop the commands without executing when they are not executed. ?
	return (status);
}

int	build_operand(t_command *command, t_stack *commands, int in_pipe[2], int out_pipe[2], int status)
{
	(void)in_pipe;
	(void)out_pipe;
	(void)commands;

	command->left = NULL;
	command->right = NULL;
	printf("Executed! %s\n", command->command_name);
	return (status);
}

int	build_operator(t_command *command, t_stack *commands, int in_pipe[2], int out_pipe[2], int status)
{
	t_command	*right;
	t_command	*left;

	right = (t_command *)ft_stack_pop(commands);
	if (right->command_flags & MS_OPERATOR)
		build_operator(right, commands, in_pipe, out_pipe, status);
	left = (t_command *)ft_stack_pop(commands);
	if (left->command_flags & MS_OPERATOR)
		build_operator(left, commands, in_pipe, out_pipe, status);
	command->left = left;
	command->right = right;
	return (status);
}

int	build_command(t_command *command, t_stack *commands, int in_pipe[2], int out_pipe[2], int status)
{
	if (command->command_flags & MS_OPERAND)
		return (build_operand(command, commands, in_pipe, out_pipe, status));
	else if (command->command_flags & MS_OPERATOR)
		return (build_operator(command, commands, in_pipe, out_pipe, status));
	return (status);
}

void	print_commands(t_command *command)
{
	printf("Parent: %s Left: %s Right: %s\n", command->command_name, command->left->command_name, command->right->command_name);
	if (command->left->command_flags & MS_OPERATOR)
		print_commands(command->left);
	if (command->right->command_flags & MS_OPERATOR)
		print_commands(command->right);
}

//executes instructions by going through the stack.
int	minishell_execute(t_stack *commands)
{
	//int			i;
	int			status;
	int			in_pipe[2];
	int			out_pipe[2];
	t_command	*command;

	status = -1;
	// while (commands->size)
	// {
		// if (pipe(out_pipe) < 0)
		// 	return (minishell_error());
	command = (t_command *)ft_stack_pop(commands);
	if (command->command_flags & MS_OPERAND)
		build_operand(command, commands, in_pipe, out_pipe, status);
	else if (command->command_flags & MS_OPERATOR)
		build_operator(command, commands, in_pipe, out_pipe, status);
		//close_read_write(in_pipe);
		//dup_pipe(out_pipe, in_pipe);
	// }
	print_commands(command);
	return (0);
}
