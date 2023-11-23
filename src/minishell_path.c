#include "minishell.h"

static int  delete_strings(char **s)
{
	int	from;

	if (!s)
		return (0);
	from = 0;
	while (s[from])
	{
		free(s[from]);
		from++;
	}
	free(s);
	return (0);
}

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
	int		i;

	i = 0;
	while (paths[i])
	{
		tmp = paths[i];
		paths[i] = ft_strjoin(tmp, "/");
		if (!paths[i])
		{
			delete_strings(paths);
            return (-1);
		}
		free(tmp);
		i++;
	}
	return (1);
}

char	*find_command(char **paths, char *command)
{
	int		i;
	char	*path;

	i = 0;
	while (paths[i])
	{
		path = ft_strjoin(paths[i], command);
		if (!path)
			return (NULL);
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

char	*get_command(char *command)
{
	char        **paths;
	char	    *command_path;
    extern char **environ;

	if (!command)
		return (NULL);
	if (ft_strchr(command, '/'))
		return (command);
	if (get_paths(&paths, environ) < 0)
        return (NULL);
	command_path = find_command(paths, command);
	if (!command_path)
	{
		delete_strings(paths);
		return (NULL);
	}
	delete_strings(paths);
	return (command_path);
}
