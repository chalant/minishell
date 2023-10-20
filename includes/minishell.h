#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include "minishell_parsing.h"
# include "libft.h"

int		ms_envcpy(void);
char	**ms_realloc(char **ptr, int addon);
char	*ms_get_var_env(char *var);

#endif