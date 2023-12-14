/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_cd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 17:20:20 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/14 15:08:04 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// updates OLDPWD and PWD
// error: 1 or nomem, msg
static int	ms_update_pwd_env(t_ms_context *data)
{
	char	*pwd;
	char	*oldpwd;

	pwd = ms_get_var_env(data, "PWD");
	if (pwd)
	{
		oldpwd = ft_strjoin("OLD", pwd);
		if (!oldpwd || ms_export_var(data, oldpwd))
			return (ms_perror("cd", NULL, NULL, errno) * ERR_NOMEM);
	}
	else
		ms_unset_var(data, "OLDPWD");
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		if (errno == ENOMEM)
			return (ms_perror("cd", NULL, NULL, errno) * ERR_NOMEM);
		return (ms_perror("cd", NULL, NULL, errno));
	}
	pwd = ft_strjoin("PWD=", oldpwd);
	free(oldpwd);
	if (!pwd || ms_export_var(data, pwd))
		return (ms_perror("cd", NULL, NULL, errno) * ERR_NOMEM);
	return (0);
}

static int	ms_cd_pwd_update(t_ms_context *data, char **arg, int fd)
{
	int	check;

	check = 0;
	if (arg[1] && !ft_strncmp(arg[1], "-", 2))
		check = ms_pwd(arg, fd);
	if (!check)
		check = ms_update_pwd_env(data);
	return (check);
}

// error: 1 or nomem, prints msg
int	ms_cd(t_ms_context *data, char **arg, int fd)
{
	char	*path;

	if (arg[1] && arg[2])
		return (ms_perror(arg[0], NULL, "too many arguments", 0));
	path = arg[1];
	if (!path || !ft_strncmp(path, "~", 2))
	{
		path = getenv("HOME");
		if (!path)
			return (ms_perror(arg[0], NULL, "HOME not set", 0));
	}
	if (!*path)
		return (0);
	if (!ft_strncmp(path, "-", 2))
		path = getenv("OLDPWD");
	if (!path)
		return (ms_perror(arg[0], NULL, "OLDPWD not set", 0));
	if (chdir(path))
		return (ms_perror(arg[0], path, NULL, errno));
	return (ms_cd_pwd_update(data, arg, fd));
}
