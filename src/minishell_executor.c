#include "minishell.h"
#include "minishell_commands.h"

int	execute_and(t_command *left, t_command *right, t_stack *commands, int in_pipe[2], int out_pipe[2], int status)
{
	status = execute_command(left, commands, in_pipe, out_pipe, -1);
	//if (status == 0)
	return (execute_command(right, commands, in_pipe, out_pipe, -1));
	//todo should also pop the commands without executing when they are not executed. ?
	return (status);
}

int	execute_or(t_command *left, t_command *right, t_stack *commands, int in_pipe[2], int out_pipe[2], int status)
{
	status = execute_command(left, commands, in_pipe, out_pipe, -1);
	//if (status > 0)
	return (execute_command(right, commands, in_pipe, out_pipe, -1));
	//todo should also pop the commands without executing when they are not executed. ?
	return (status);
}

int	execute_pipe(t_command *left, t_command *right, t_stack *commands, int in_pipe[2], int out_pipe[2], int status)
{
	//skip
	// printf("Executing OR!\n");
	status = execute_command(left, commands, in_pipe, out_pipe, -1);
	//if (status > 0)
	return (execute_command(right, commands, in_pipe, out_pipe, -1));
	//todo should also pop the commands without executing when they are not executed. ?
	return (status);
}

int	execute_operand(t_command *command, t_stack *commands, int in_pipe[2], int out_pipe[2], int status)
{
	(void)in_pipe;
	(void)out_pipe;
	(void)commands;

	printf("Executed! %s\n", command->command_name);
	return (status);
}

int	execute_operator(t_command *command, t_stack *commands, int in_pipe[2], int out_pipe[2], int status)
{
	t_command	*right;
	t_command	*left;

	right = (t_command *)ft_stack_pop(commands);
	left = (t_command *)ft_stack_pop(commands);
	printf("Command %s\n", command->command_name);
	printf("Left %s\n", left->command_name);
	printf("Right %s\n", right->command_name);
	if (command->command_flags & MS_AND)
		status = execute_and(left, right, commands, in_pipe, out_pipe, status);
	else if (command->command_flags & MS_OR)
		status = execute_or(left, right, commands, in_pipe, out_pipe, status);
	else if (command->command_flags & MS_PIPE)
		status = execute_pipe(left, right, commands, in_pipe, out_pipe, status);
	return (status);
}

int	execute_command(t_command *command, t_stack *commands, int in_pipe[2], int out_pipe[2], int status)
{
	if (command->command_flags & MS_OPERAND)
		return (execute_operand(command, commands, in_pipe, out_pipe, status));
	else if (command->command_flags & MS_OPERATOR)
		return (execute_operator(command, commands, in_pipe, out_pipe, status));
	return (status);
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
	while (commands->size)
	{
		// if (pipe(out_pipe) < 0)
		// 	return (minishell_error());
		command = (t_command *)ft_stack_pop(commands);
		if (command->command_flags & MS_OPERAND)
			status = execute_operand(command, commands, in_pipe, out_pipe, status);
		else if (command->command_flags & MS_OPERATOR)
			status = execute_operator(command, commands, in_pipe, out_pipe, status);
		//close_read_write(in_pipe);
		//dup_pipe(out_pipe, in_pipe);
	}
	return (0);
}
