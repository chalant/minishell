#ifndef MINISHELL_COMMANDS_H
# define MINISHELL_COMMANDS_H

#include "minishell.h"

#define MS_PIPE 1 << 0
#define MS_OR 1 << 1
#define MS_AND 1 << 2
#define	MS_OPERATOR 1 << 3
#define MS_OPERAND 1 << 4
#define MS_BUILTIN 1 << 5

typedef struct	s_redirection
{
	char		*output;
}				t_redirection;

typedef struct	s_command
{
	int					command_flags;
	char				*command_name;
	struct	s_command	*left;
	struct	s_command	*right;
	t_darray			*arguments;
	t_darray			*redirections;
	
}				t_command;

#endif