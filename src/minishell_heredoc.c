/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 14:27:42 by yves              #+#    #+#             */
/*   Updated: 2023/12/20 17:02:26 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ms_heredoc_write(int fd, char *line, int delim_quoted)
{
	char	*end;
	char	temp;

	if (delim_quoted)
		write(fd, line, ft_strlen(line));
	while (!delim_quoted && *line)
	{
		end = ft_strchr(line, '$');
		if (end)
			write(fd, line, end - line);
		else
			write(fd, line, ft_strlen(line));
		line = end;
		if (!line)
			break ;
		end = ms_end_of_name(line);
		temp = *end;
		*end = 0;
		line = getenv(line + 1);
		write(fd, line, ft_strlen(line));
		*end = temp;
		line = end;
	}
	write(fd, "\n", 1);
}

int	eof_warning(void *subject)
{
	write(STDERR_FILENO, "warning: here-document delimited by end-of-file", 47);
	write(STDERR_FILENO, " (wanted '", 10);
	write(STDERR_FILENO, (char *) subject, ft_strlen((char *) subject));
	write(STDERR_FILENO, "')\n", 3);
	return (1);
}

int	ms_heredoc_prompt(t_command *command, t_redirection *redirection, int fd)
{
	char	*line;

	g_global_state.prompt = 1;
	line = readline("> ");
	if (ms_join_line(command->context, line, "\n") < 0)
		return (-1);
	while (line && ft_strcmp(line, redirection->file_path) != 0)
	{
		ms_heredoc_write(fd, line, redirection->redirection_flags & MS_QUOTED);
		free(line);
		line = readline("> ");
		if (ms_join_line(command->context, line, "\n") < 0)
			return (-1);
	}
	if (line && ms_add_separator(command->context, line, "\n") < 0)
		return (-1);
	if (!line && g_global_state.prompt)
		ms_message_header(redirection->file_path, eof_warning, STDERR_FILENO);
	free(line);
	return (1);
}

static char	*ms_get_heredoc_path(int id)
{
	char	*idstr;
	char	*path;

	idstr = ft_itoa(id);
	if (!idstr)
		return (NULL);
	path = ft_strjoin(MS_HEREDOC_PATH, idstr);
	free(idstr);
	return (path);
}

int	ms_heredoc(t_command *command, int id)
{
	int				fd;
	int				i;
	char			*path;
	t_redirection	*red;

	i = -1;
	while (++i < command->redirections->size)
	{
		red = ft_darray_get(command->redirections, i);
		if (red->redirection_flags & MS_HEREDOC)
		{
			path = ms_get_heredoc_path(id);
			if (!path)
				return (-1);
			fd = open(path, O_TRUNC | O_CREAT | O_RDWR, red->mode);
			if (fd < 0)
				return (ms_perror(path, NULL, NULL, errno) * -1);
			red->tmp_file = path;
			ms_heredoc_prompt(command, red, fd);
			close(fd);
		}
	}
	return (1);
}
