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

int make_paths(char **paths)
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

int get_paths(char ***paths, char **env)
{
	env = get_path_env_var(env);
	if (!env)
		return (-1);
	*paths = ft_split(&env[0][5], ':');
	if (!*paths)
        return (-1);
	make_paths(*paths);
    return (1);
}

char	*get_binary(char *command)
{
	int			failed;
	char        **paths;
	char	    *command_path;
    extern char **environ;

	if (!command)
		return (NULL);
	if (ft_strchr(command, '/'))
		return (ft_strdup(command));
	if (get_paths(&paths, environ) < 0)
        return (NULL);
	failed = 0;
	command_path = find_command(paths, command, &failed);
	if (!command_path)
	{
		ft_clear_ds(paths);
		if (failed)
			return (NULL);
		return (ft_strdup(command));
	}
	ft_clear_ds(paths);
	return (command_path);
}
