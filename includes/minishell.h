/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 16:53:20 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/11 14:58:31 by bvercaem         ###   ########.fr       */
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

# define MS_PROMPT_MSG "shellshock$ "

extern int	g_global_status;

int		ms_exit(t_ms_context *context, char **arg);
void	ms_flush_exit(t_ms_context *context, int exit_value);
int     ms_echo(char **arg, int fd);
int		ms_cd(t_ms_context *context, char **arg, int fd);
int		ms_pwd(int);
int		ms_env(void);
int		ms_export(t_ms_context *context, char **arg);
int		ms_export_var(t_ms_context *context, char *var);
int		ms_unset(t_ms_context *context, char **arg);
void	ms_unset_var(t_ms_context *context, char *name);
int		ms_envcpy(t_ms_context *context);
char	*ms_get_var_env(t_ms_context *context, char *var);
char	**ms_realloc(char **ptr, int add);
char	**ms_get_var_envp(t_ms_context *context, char *var);
int		ms_check_varname(char *var);

int		execute(t_ms_context *context, t_parse_tree *tree, t_darray *commands);

int		ms_perror(const char *cmd, const char *item, const char *msg, int err);
int		ms_message_header(void *data, int(*printer)(void *), int fd);

#endif
