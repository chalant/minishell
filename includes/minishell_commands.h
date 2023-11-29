/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_commands.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 16:36:47 by ychalant          #+#    #+#             */
/*   Updated: 2023/11/29 18:18:00 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_COMMANDS_H
# define MINISHELL_COMMANDS_H

# include <fcntl.h>
# include "minishell.h"

# define MS_PIPE 1 << 0
# define MS_OR 1 << 1
# define MS_AND 1 << 2
# define	MS_OPERATOR 1 << 3
# define MS_OPERAND 1 << 4
# define MS_BUILTIN 1 << 5
# define MS_FORKED 1 << 6

// redirection flags:
# define MS_HEREDOC 1 << 0
# define MS_READ 1 << 1
# define MS_WRITE 1 << 2

//todo: this might not work on mac.
# define MS_HEREDOC_PATH "/tmp/ms_heredoc"

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
	int					input;
	int					output;
	char				*command_name;
	t_darray			*redirections;
	t_darray			*arguments;
	struct	s_command	*left;
	struct	s_command	*right;
}				t_command;

int	init_command(t_command *command);
int	create_redirection_command(t_parse_tree *node, t_stack *stack);
int	create_simple_command(t_parse_tree *node, t_stack *stack);

int	execute_command(t_command *command, int in_pipe[2], int out_pipe[2]);
int	execute_or(t_command *command, int in_pipe[2], int out_pipe[2]);
int	execute_and(t_command *command, int in_pipe[2], int out_pipe[2]);
int	execute_pipe(t_command *command, int in_pipe[2], int out_pipe[2]);

#endif