/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_commands.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 16:36:47 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/04 14:14:08 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_COMMANDS_H
# define MINISHELL_COMMANDS_H

# include <fcntl.h>
# include "minishell.h"

# define MS_PIPE 1 << 0
# define MS_OR 1 << 1
# define MS_AND 1 << 2
# define MS_OPERATOR 1 << 3
# define MS_OPERAND 1 << 4
# define MS_BUILTIN 1 << 5
# define MS_FORKED 1 << 6

// redirection flags:
# define MS_HEREDOC 1 << 0
# define MS_READ 1 << 1
# define MS_WRITE 1 << 2

//todo: this might not work on mac.
# define MS_HEREDOC_PATH "/tmp/ms_heredoc"

typedef struct s_ms_context
{
	int					status;
	int					env_excess;
	char				*line;
	char				**env;
	struct sigaction	act_sigint;
	struct sigaction	act_sigquit;
	t_parsing_data		parse_data;
	t_parse_tree		tree;
	t_darray			commands;
}			t_ms_context;

typedef struct	s_redirection
{
	int			redirection_flags;
	int			file_flags;
	char		*file_path;
	mode_t		mode;
}				t_redirection;

typedef struct	s_command
{
	char				*command_name;
	int					command_flags;
	int					input;
	int					output;
	t_darray			*redirections;
	t_darray			*arguments;
	struct	s_command	*left;
	struct	s_command	*right;
	t_ms_context		*context;
}				t_command;

int	init_command(t_command *command);
int	delete_commands(t_darray *commands);
int	set_command_fields(t_parse_tree *node, t_command *command);
int	redirection_command(t_parse_tree *node, t_command *command);
int	create_command(t_parse_tree *node, t_stack *stack, int (*factory)(t_parse_tree *, t_command *));

int	minishell_execute(t_command *command);
int	execute_command(t_command *command, int in_pipe[2], int out_pipe[2]);
int	execute_or(t_command *command, int in_pipe[2], int out_pipe[2]);
int	execute_and(t_command *command, int in_pipe[2], int out_pipe[2]);
int	execute_pipe(t_command *command, int in_pipe[2], int out_pipe[2]);

#endif