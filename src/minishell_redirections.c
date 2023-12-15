/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_redirections.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 13:40:52 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/15 19:20:03 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	set_redirection_flags(t_redirection *redirection, char *rule_name)
{
	if (strcmp(rule_name, ">") == 0)
	{
		redirection->file_flags = O_TRUNC | O_CREAT | O_APPEND | O_RDWR;
		redirection->redirection_flags |= MS_WRITE;
	}
	else if (strcmp(rule_name, "<") == 0)
	{
		redirection->file_flags |= O_RDONLY;
		redirection->redirection_flags |= MS_READ;
	}
	else if (strcmp(rule_name, "<<") == 0)
	{
		redirection->redirection_flags |= MS_HEREDOC;
		redirection->redirection_flags |= MS_READ;
		redirection->file_flags = O_RDONLY;
	}
	else if (strcmp(rule_name, ">>") == 0)
	{
		redirection->file_flags = O_CREAT | O_APPEND | O_RDWR;
		redirection->redirection_flags |= MS_WRITE;
	}
	return (1);
}

int	set_redirection(t_redirection *redirection, t_parse_tree *tree)
{
	t_parse_tree	*redir;
	t_parse_tree	*leaf;

	if (!tree || !tree->rule_name)
		return (0);
	redirection->redirection_flags = 0;
	redirection->file_flags = 0;
	redirection->file_path = NULL;
	redirection->mode = 0664;
	redirection->tmp_file = NULL;
	redir = ft_darray_get(tree->children, 0);
	set_redirection_flags(redirection, redir->rule_name);
	leaf = get_leaf(ft_darray_get(tree->children, 1));
	if (leaf->token->flags & IS_QUOTED)
		redirection->redirection_flags |= MS_QUOTED;
	redirection->file_path = leaf->rule_name;
	return (0);
}

int	set_redirections(t_command *command, t_parse_tree *tree)
{
	t_parse_tree	*node;
	t_redirection	redirection;

	set_redirection(&redirection, ft_darray_get(tree->children, 0));
	if (ft_darray_append(command->redirections, &redirection) < 0)
		return (-1);
	node = ft_darray_get(tree->children, 1);
	while (!node->terminal)
	{
		set_redirection(&redirection, ft_darray_get(node->children, 0));
		if (ft_darray_append(command->redirections, &redirection) < 0)
			return (-1);
		node = ft_darray_get(node->children, 1);
	}
	return (0);
}

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

int	ms_new_line(t_command *command, char *line)
{
	char	*tmp;

	tmp = ft_strjoin(command->context->line, "\n");
	if (!tmp)
	{
		free(line);
		return (-1);
	}
	free(command->context->line);
	command->context->line = tmp;
	return (1);
}

int	ms_join_line(t_command *command, char *line)
{
	char	*tmp;

	if (ms_new_line(command, line) < 0)
		return (-1);
	tmp = ft_strjoin(command->context->line, line);
	if (!tmp)
	{
		free(line);
		return (-1);
	}
	free(command->context->line);
	command->context->line = tmp;
	return (1);
}

int	eod_warning(void *subject)
{
	char	*limiter;

	limiter = (char *)subject;
	ft_putstr_fd("shellshock: warning: here-document delimited by end-of-file (wanted '", STDERR_FILENO);
	ft_putstr_fd(limiter, STDERR_FILENO);
	write(STDERR_FILENO, "')\n", 3);
	return (1);
}

int	ms_heredoc_prompt(t_command *command, t_redirection *redirection, int fd)
{
	char	*line;

	line = readline("> ");
	if (ms_join_line(command, line) < 0)
		return (-1);
	while (line && strcmp(line, redirection->file_path) != 0)
	{
		ms_heredoc_write(fd, line, redirection->redirection_flags & MS_QUOTED);
		free(line);
		line = readline("> ");
		if (ms_join_line(command, line) < 0)
			return (-1);
	}
	if (ms_new_line(command, line) < 0)
		return (-1);
	if (!line)
		ms_message_header(redirection->file_path, eod_warning, STDERR_FILENO);
	free(line);
	return (1);
}

int	ms_heredoc(t_command *command, int id)
{
	int				fd;
	char			*path;
	int				i;
	t_redirection	*red;

	i = -1;
	while (++i < command->redirections->size)
	{
		red = ft_darray_get(command->redirections, i);
		if (red->redirection_flags & MS_HEREDOC)
		{
			path = ft_strjoin(MS_HEREDOC_PATH, ft_itoa(id));
			if (!path)
				return (-1);
			fd = open(path, O_TRUNC | O_CREAT | O_RDWR, red->mode);
			if (fd < 0)
				return (ms_perror(path, NULL, NULL, errno) - 2);
			red->tmp_file = path;
			ms_heredoc_prompt(command, red, fd);
			close(fd);
		}
	}
	return (1);
}
 
int	create_files(t_command *command, t_darray *redirections)
{
	int				i;
	int				fd;
	t_redirection	*redirection;

	i = -1;
	while (++i < redirections->size)
	{
		redirection = ft_darray_get(redirections, i);
		if (!(redirection->redirection_flags & MS_HEREDOC))
			fd = open(redirection->file_path, redirection->file_flags, redirection->mode);
		else
			fd = open(redirection->tmp_file, redirection->file_flags, redirection->mode);
		if (redirection->redirection_flags & MS_READ)
		{
			if (command->input && !(command->command_flags & MS_IO_INJECTED))
				close(command->input);
			command->input = fd;
		}
		else if (redirection->redirection_flags & MS_WRITE)
		{
			if (command->output && !(command->command_flags & MS_IO_INJECTED))
				close(command->output);
			command->output = fd;
		}
		if (fd < 0)
			return (ms_perror(redirection->file_path, NULL, NULL, errno) - 2);
	}
	return (1);
}
