/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_redirection_factory.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 13:40:52 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/18 14:29:04 by ychalant         ###   ########.fr       */
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
	t_parse_tree	*node;
	t_parse_tree	*leaf;

	if (!tree || !tree->rule_name)
		return (0);
	redirection->redirection_flags = 0;
	redirection->file_flags = 0;
	redirection->file_path = NULL;
	redirection->mode = 0664;
	redirection->tmp_file = NULL;
	node = ft_darray_get(tree->children, 0);
	set_redirection_flags(redirection, node->rule_name);
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

int	set_file_descriptor(t_command *command, t_redirection *redirection, int fd)
{
	if (redirection->redirection_flags & MS_READ)
	{
		if (command->input > 0 && !(command->command_flags & MS_INJECTI))
			close(command->input);
		command->input = fd;
	}
	else if (redirection->redirection_flags & MS_WRITE)
	{
		if (command->output > 0 && !(command->command_flags & MS_INJECTO))
			close(command->output);
		command->output = fd;
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
			fd = open(redirection->file_path,
					redirection->file_flags, redirection->mode);
		else
			fd = open(redirection->tmp_file,
					redirection->file_flags, redirection->mode);
		set_file_descriptor(command, redirection, fd);
		if (fd < 0)
			return (ms_perror(redirection->file_path, NULL, NULL, errno) * -1);
	}
	return (1);
}
