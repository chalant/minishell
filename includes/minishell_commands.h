#ifndef MINISHELL_COMMANDS_H
# define MINISHELL_COMMANDS_H

#include "minishell.h"

typedef	struct	s_syntax_tree
{
	char		*rule_name;
	t_darray	children;
}				t_syntax_tree;


#endif