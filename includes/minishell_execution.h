/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_execution.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yves <yves@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 15:28:16 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/16 17:02:32 by yves             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_EXECUTION_H
# define MINISHELL_EXECUTION_H

# include "minishell_commands.h"

char	**make_arguments(t_command *command, char *binary);

int		get_exit_status(pid_t pid);
int		redirect_io(t_command *command);
int		pipe_io(t_command *command, int in_pipe[2], int out_pipe[2]);
int		minishell_execute(t_command *command);
int		execute_simple_command(t_command *parent, t_command *command, int in_pipe[2], int out_pipe[2]);
int		execute_command_core(t_command *parent, t_command *command, int in_pipe[2], int out_pipe[2]);
int		execute_command(t_command *parent, t_command *command, int in_pipe[2], int out_pipe[2]);
int		execute_or(t_command *parent, t_command *command, int in_pipe[2], int out_pipe[2]);
int		execute_and(t_command *parent, t_command *command, int in_pipe[2], int out_pipe[2]);
int		execute_pipe(t_command *parent, t_command *command, int in_pipe[2], int out_pipe[2]);

pid_t	execute_process(t_command *parent, t_command *command, int in_pipe[2], int out_pipe[2]);

#endif
