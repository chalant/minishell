/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 16:53:20 by bvercaem          #+#    #+#             */
/*   Updated: 2023/10/27 13:51:11 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include "minishell_parsing.h"
# include "libft.h"

typedef struct s_shellshock
{
	int		env_excess;
	char	**env;
}			t_shellshock;

# define MS_PROMPT "shellshock$ "

int		ms_echo(char **arg);

void	ms_cd(t_shellshock *data, char *path);
void	ms_pwd(t_shellshock *data);

int		ms_env(void);
int		ms_export(t_shellshock *data, char **arg);
int		ms_unset(t_shellshock *data, char **arg);

int		ms_envcpy(t_shellshock *data);
char	**ms_realloc(char **ptr, int add);
char	*ms_get_var_env(char *var);

#endif
