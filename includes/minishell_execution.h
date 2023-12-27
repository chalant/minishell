/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_execution.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 15:28:16 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/24 01:33:11 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_EXECUTION_H
# define MINISHELL_EXECUTION_H

# include "minishell_commands.h"

char	**make_arguments(t_command *command, char *binary);

int		get_binary(char *command_name, char **path);
int		get_exit_status(pid_t pid);
int		close_fd(int *fd);
int		create_files(t_command *command, t_darray *redirections);
int		should_expand(t_token *token);
int		flush_expansion(t_darray *expansion, int status);
int		expand_token(t_token *token, t_darray *into, t_ms_context *data);
int		expand_command_fields(t_command *command);
int		expand_redirection(t_redirection *redirection, t_ms_context *context);
int		handle_redirections(t_command *command);
int		redirect_in(t_command *command);
int		redirect_out(t_command *command);
int		redirect_io(t_command *command);
int		pipe_io(t_command *command, int in_pipe[2], int out_pipe[2]);
int		start_execution(t_command *command);
int		execute_simple_command(t_command *command, int in_pipe[2],
			int out_pipe[2]);
int		execute_operand(t_command *parent, t_command *command, int in_pipe[2],
			int out_pipe[2]);
int		execute_builtin(t_command *command);
int		execute_command(t_command *parent, t_command *command, int in_pipe[2],
			int out_pipe[2]);
int		execute_or(t_command *parent, t_command *command, int in_pipe[2],
			int out_pipe[2]);
int		execute_and(t_command *parent, t_command *command, int in_pipe[2],
			int out_pipe[2]);
int		execute_pipe(t_command *parent, t_command *command, int in_pipe[2],
			int out_pipe[2]);

pid_t	execute_process(t_command *parent, t_command *command, int in_pipe[2], int out_pipe[2]);

#endif
