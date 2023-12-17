/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_execution_redirections.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 16:28:54 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/17 17:03:12 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clear_redirection(void *address)
{
	t_redirection	*redirection;

	redirection = (t_redirection *)address;
	free(redirection->tmp_file);
	redirection->tmp_file = NULL;	
}

int	redirect_in(t_command *command)
{
	if (dup2(command->input, STDIN_FILENO) < 0)
		return (ms_perror("dup in", NULL, NULL, errno) - 2);
	return (0);
}

int	redirect_out(t_command *command)
{
	if (dup2(command->output, STDOUT_FILENO) < 0)
		return (ms_perror("dup out", NULL, NULL, errno) - 2);
	return (1);
}

int redirect_io(t_command *command)
{
	int	status;

	status = 0;
	if (command->input > 0)
		status = redirect_in(command);
	if (command->output > 0)
		status = redirect_out(command);
	return (status);
}

int	handle_redirections(t_command *command)
{
	if (command->redirections && command->redirections->size)
	{
		if (create_files(command, command->redirections) < 0)
			return (-1);
	}
	return (1);
}
