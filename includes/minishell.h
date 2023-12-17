/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 16:53:20 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/17 16:39:47 by ychalant         ###   ########.fr       */
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
# include <fcntl.h>
# include "minishell_parsing.h"
# include "minishell_graph.h"
# include "minishell_tokens.h"
# include "minishell_errors.h"
# include "minishell_commands.h"
# include "minishell_semantics.h"
# include "minishell_execution.h"
# include "libft.h"

# define MS_PROMPT_MSG "shellshock$ "
# define MS_SUBPROMPT_MSG "> "
# define MS_SUBPROMPT_INT -2

extern int	g_global_status;

char	*ms_get_var_env(t_ms_context *data, char *var);
char	**ms_realloc(char **ptr, int add);
char	**ms_get_var_envp(t_ms_context *data, char *var);

void	ms_flush_exit(t_ms_context *data, int exit_value);
void	ms_unset_var(t_ms_context *data, char *name);

int		ms_set_signals(t_ms_context *data);
int		ms_exit(t_ms_context *data, char **arg);
int		ms_echo(char **arg, int fd);
int		ms_cd(t_ms_context *data, char **arg, int fd);
int		ms_pwd(char **arg, int fd);
int		ms_env(void);
int		ms_export(t_ms_context *data, char **arg);
int		ms_export_var(t_ms_context *data, char *var);
int		ms_unset(t_ms_context *data, char **arg);
int		ms_envcpy(t_ms_context *data);
int		ms_check_varname(char *var);

int		execute(t_ms_context *data, t_parse_tree *tree, t_darray *command_array);

int		ms_perror(const char *cmd, const char *item, const char *msg, int err);
int		ms_message_header(void *data, int (*printer)(void *), int fd);

int		ms_add_separator(t_ms_context *context, char *line, const char *delim);
int		ms_join_line(t_ms_context *context, char *line, const char *delim);

int		clear_command(t_command *command);

#endif
