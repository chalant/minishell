/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_redirections.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 13:40:52 by ychalant          #+#    #+#             */
/*   Updated: 2023/11/30 17:49:19 by bvercaem         ###   ########.fr       */
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
	else
		return (-1);
	return (0);
}

int	set_redirection(t_redirection *redirection, t_parse_tree *tree)
{
	t_parse_tree	*redir;

	//todo: need to set the right file permissions.
	if (!tree || !tree->rule_name)
		return (0);
	redirection->redirection_flags = 0;
	redirection->file_flags = 0;
	redirection->file_path = NULL;
	redirection->mode = 0664;
	redir = ft_darray_get(tree->children, 0);
	set_redirection_flags(redirection, redir->rule_name);
	//todo: maybe copy the word or set it to NULL in the tree.
	//might be safer to copy instead.
	redirection->file_path = *get_word(
			ft_darray_get(tree->children, 1));
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
	while (node->children)
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

int	ms_heredoc_prompt(t_redirection *redirection)
{
	char	*line;
	int		fd;

	fd = open(MS_HEREDOC_PATH,
			O_TRUNC | O_CREAT | O_RDWR, redirection->mode);
// check fd
	line = readline("> ");
	while (line && strcmp(line, redirection->file_path) != 0)
	{
		// pass along whether the delimiter was quoted or not
		ms_heredoc_write(fd, line, 0);
		free(line);
		line = readline("> ");
	}
	if (line)
		free(line);
	else
		printf("shellshock: warning: here-document delimited by end-of-file (wanted '%s')\n", redirection->file_path);
	close(fd);
	return (1);
}

int	ms_heredoc(t_darray *redirections)
{
	int				i;
	t_redirection	*redirection;

	i = -1;
	while (++i < redirections->size)
	{
		redirection = ft_darray_get(redirections, i);
		if (redirection->redirection_flags & MS_HEREDOC)
			ms_heredoc_prompt(redirection);
	}
	return (1);
}

int	create_files(t_command *command, t_darray *redirections)
{
	int				i;
	int				fd;
	t_redirection	*redirection;

	ms_heredoc(redirections);
	i = redirections->size;
	while (--i > -1)
	{
		redirection = ft_darray_get(redirections, i);
		//todo: handle errors
		if (!(redirection->redirection_flags & MS_HEREDOC))
			fd = open(redirection->file_path, redirection->file_flags, redirection->mode);
		else
			fd = open(MS_HEREDOC_PATH, redirection->file_flags, redirection->mode);
		if (fd < 0)
			return (-1);
		if (redirection->redirection_flags & MS_READ && !command->input)
			command->input = fd;
		else if (redirection->redirection_flags & MS_WRITE && !command->output)
			command->output = fd;
		else
			close(fd);
	}
	return (1);
}
