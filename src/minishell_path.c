/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_path.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 12:05:53 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/19 17:53:47 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "sys/stat.h"

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

int	find_command(char **paths, char *command, char **result)
{
	int			i;
	char		*path;
	struct stat	statbuf;

	i = 0;
	while (paths[i])
	{
		path = ft_strjoin(paths[i], command);
		if (!path)
			return (-1);
		if (stat(path, &statbuf) == -1)
		{
			free(path);
			i++;
			continue ;
		}
		if (S_ISREG(statbuf.st_mode) && access(path, X_OK) == 0)
		{
			*result = path;
			return (1);
		}
		free(path);
		i++;
	}
	return (0);
}

int	get_paths(char ***paths, char **env)
{
	env = get_path_env_var(env);
	if (!env)
	{
		*paths = NULL;
		return (0);
	}
	*paths = ft_split(&env[0][5], ':');
	if (!*paths)
		return (-1);
	return (make_paths(*paths));
}

int	get_binary(char *command_name, char **result)
{
	char		**paths;
	extern char	**environ;

	if (!command_name)
		return (0);
	if (get_paths(&paths, environ) < 0)
		return (-1);
	if (!paths)
	{
		*result = ft_strdup(command_name);
		if (!*result)
			return (-1);
		return (0);
	}
	if (find_command(paths, command_name, result) < 0)
	{
		ft_clear_ds(paths);
		return (-1);
	}
	ft_clear_ds(paths);
	return (0);
}
