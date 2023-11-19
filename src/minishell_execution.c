#include "minishell.h"

int	get_exit_status(int pid)
{
	int	status;

	status = 0;
	waitpid(pid, &status, 2);
	while (wait(NULL) != -1)
		continue ;
	if (((status) & 0x7f) == 0)
		return ((((status) & 0xff00) >> 8));
	return (0);
}

//copies file descriptors
void	copy_pipe(int src_pipe[2], int dest_pipe[2])
{
	dest_pipe[0] = src_pipe[0];
	dest_pipe[1] = src_pipe[1];
}

/* runs the command as a subprocess, if the command is builtin, we exit
with the returned status, if is not, we exit with an error code. */
int	execute_process(t_command *command, int *in_pipe, int *out_pipe)
{
	int	pid;
	int	status;

	pid = fork();
	//todo: maybe print an error ?
	if (pid < 0)
		return (-1);
	command->command_flags |= MS_FORKED;
	if (pid == 0)
	{
		dup2(in_pipe[0], STDIN_FILENO);
		close(out_pipe[0]);
		dup2(out_pipe[1], STDOUT_FILENO);
		close(out_pipe[1]);
		status = execute_command(command, in_pipe, out_pipe);
		if (command->command_flags & MS_BUILTIN)
			exit(status);
		exit(126);
	}
	close(out_pipe[1]);
	return (pid);
}

int	execute_and(t_command *command, int *in_pipe, int *out_pipe)
{
	int	status;

	//todo: if there are redirections, the out_pipe has
	//a write file descriptor in append mode.
	status = execute_command(command->left, in_pipe, out_pipe);
	if (status == 0)
		return (execute_command(command->right, in_pipe, out_pipe));
	return (status);
}

int	execute_or(t_command *command, int *in_pipe, int *out_pipe)
{
	int	status;

	//todo: if there are redirections, the out_pipe has
	//a write file descriptor in append mode.
	status = execute_command(command->left, in_pipe, out_pipe);
	if (status > 0)
		return (execute_command(command->right, in_pipe, out_pipe));
	return (status);
}

int	execute_pipe(t_command *command, int *in_pipe, int *out_pipe)
{
	pid_t	pid;

	if (pipe(out_pipe) < 0)
		return (-1);
	//todo: out_pipe could also be a redirection so we need to either create a pipe or open a file...
	//read from in pipe of the parent
	pid = execute_process(command->left, in_pipe, out_pipe);
	close(in_pipe[0]);
	close(in_pipe[1]);
	copy_pipe(out_pipe, in_pipe);
	pid = execute_process(command->right, in_pipe, out_pipe);
	return (get_exit_status(pid));
}

int	launch_execve(t_command *command, int *in_pipe, int *out_pipe)
{
	(void)in_pipe;
	(void)out_pipe;
	extern char	**environ;
	char		**arguments;
	int			i;
	//todo: lookup into the environment.
	//doing this in the child process to avoid unnecessary copies.
	arguments = malloc(sizeof(char *) * (command->arguments->size + 2));
	if (!arguments)
		exit(1);
	arguments[0] = command->command_name;
	i = 0;
	while (++i < command->arguments->size + 1)
		arguments[i] = *(char **)ft_darray_get(command->arguments, i - 1);
	arguments[i] = NULL;
	//todo: free all
	execve(command->command_name, arguments, environ);
	return (1);
}

int	execute_builtin(t_command *command, int *in_pipe, int *out_pipe)
{
	(void)command;
	(void)in_pipe;
	(void)out_pipe;
	return (0);
}

//this is the core execution function.
int	execute_simple_command(t_command *command, int *in_pipe, int *out_pipe)
{
	pid_t	pid;

	//todo: if the simple command has a redirection,
	//it will not write to the write-end of the out_pipe, so close it.
	if (command->command_flags & MS_BUILTIN)
		return (execute_builtin(command, in_pipe, out_pipe));
	if (!(command->command_flags & MS_FORKED))
	{
		pid = fork();
		if (pid == 0)
			launch_execve(command, in_pipe, out_pipe);
		return (get_exit_status(pid));
	}
	launch_execve(command, in_pipe, out_pipe);
	return (1);
}

int	execute_command(t_command *command, int *in_pipe, int *out_pipe)
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

int	minishell_execute(t_command *command)
{
	int			in_pipe[2];
	int			out_pipe[2];

	printf("Commands: \n");
	print_commands(command, 0);
	//todo: we either create a pipe or open a redirection as input here.
	if (pipe(in_pipe) < 0)
		return (-1);
	execute_command(command, in_pipe, out_pipe);
	return (0);
}
