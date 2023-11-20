/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_commands.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 16:36:47 by ychalant          #+#    #+#             */
/*   Updated: 2023/11/17 16:50:39 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_COMMANDS_H
# define MINISHELL_COMMANDS_H

#include <fcntl.h>
#include "minishell.h"

#define MS_PIPE 1 << 0
#define MS_OR 1 << 1
#define MS_AND 1 << 2
#define	MS_OPERATOR 1 << 3
#define MS_OPERAND 1 << 4
#define MS_BUILTIN 1 << 5
#define MS_FORKED 1 << 6

#define MS_HEREDOC 1 << 0
#define MS_READ 1 << 1
#define MS_WRITE 1 << 2

typedef struct	s_redirection
{
	int			redirection_flags;
	int			file_flags;
	char		*file_path;
	mode_t		mode;
}				t_redirection;

typedef struct	s_command
{
	int					command_flags;
	char				*command_name;
	struct	s_command	*left;
	struct	s_command	*right;
	t_darray			*redirections;
	t_redirection		*input;
	t_redirection		*output;
	t_darray			*arguments;
}				t_command;

int	execute_command(t_command *command, int in_pipe[2], int out_pipe[2]);
int	execute_or(t_command *command, int in_pipe[2], int out_pipe[2]);
int	execute_and(t_command *command, int in_pipe[2], int out_pipe[2]);
int	execute_pipe(t_command *command, int in_pipe[2], int out_pipe[2]);

#endif