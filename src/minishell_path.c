/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_path.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 12:05:53 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/18 14:18:34 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**get_path_env_var(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strnstr(env[i], "PATH=", 5))
			return (&env[i]);
		i++;
	}
	return (NULL);
}

int	make_paths(char **paths)
{
	char	*tmp;
	char	*res;
	int		i;

	i = 0;
	while (paths[i])
	{
		tmp = paths[i];
		res = ft_strjoin(tmp, "/");
		if (!res)
			return (-1);
		paths[i] = res;
		free(tmp);
		i++;
	}
	return (1);
}

char	*find_command(char **paths, char *command, int *mfailed)
{
	int		i;
	char	*path;

	i = 0;
	while (paths[i])
	{
		path = ft_strjoin(paths[i], command);
		if (!path)
		{
			*mfailed = 1;
			return (NULL);
		}
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	return (NULL);
}

int	get_paths(char ***paths, char **env)
{
	env = get_path_env_var(env);
	if (!env)
		return (0);
	*paths = ft_split(&env[0][5], ':');
	if (!*paths)
		return (-1);
	return (make_paths(*paths));
}

char	*get_binary(char *command_name)
{
	int			failed;
	char		**paths;
	char		*command_path;
	extern char	**environ;

	if (!command_name)
		return (NULL);
	if (ft_strchr(command_name, '/'))
		return (ft_strdup(command_name));
	failed = get_paths(&paths, environ);
	if (failed < 0)
		return (NULL);
	if (!failed)
		return (ft_strdup(command_name));
	failed = 0;
	command_path = find_command(paths, command_name, &failed);
	if (!command_path)
	{
		ft_clear_ds(paths);
		if (failed)
			return (NULL);
		return (ft_strdup(command_name));
	}
	ft_clear_ds(paths);
	return (command_path);
}
