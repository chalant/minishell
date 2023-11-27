/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 16:53:20 by bvercaem          #+#    #+#             */
/*   Updated: 2023/11/27 17:39:53 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <dirent.h>
# include "minishell_parsing.h"
# include "minishell_graph.h"
# include "minishell_tokens.h"
# include "minishell_errors.h"
# include "minishell_commands.h"
# include "minishell_semantics.h"
# include "libft.h"

typedef struct s_shellshock
{
	int					env_excess;
	char				**env;
	struct sigaction	act_sigint;
	struct sigaction	act_sigquit;
}			t_shellshock;

# define MS_PROMPT_MSG "shellshock$ "

int		ms_exit(t_shellshock *data, char **arg, char *line);
void	ms_flush_exit(t_shellshock *data, int exit_value);
int		ms_echo(char **arg);
int		ms_cd(t_shellshock *data, char **arg);
int		ms_pwd(void);
int		ms_env(void);
int		ms_export(t_shellshock *data, char **arg);
int		ms_export_var(t_shellshock *data, char *var);
int		ms_unset(t_shellshock *data, char **arg);
void	ms_unset_var(t_shellshock *data, char *name);
int		ms_envcpy(t_shellshock *data);
char	**ms_realloc(char **ptr, int add);
char	*ms_get_var_env(t_shellshock *data, char *var);
char	**ms_get_var_envp(t_shellshock *data, char *var);
int		ms_check_varname(char *var);

int		ms_perror(const char *cmd, const char *item, const char *msg, int err);
char	*get_command(char *command);

#endif
