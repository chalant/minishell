/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_cd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 17:20:20 by bvercaem          #+#    #+#             */
/*   Updated: 2023/11/23 17:29:28 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// updates OLDPWD and PWD
static int	ms_update_pwd_env(t_shellshock *data)
{
	char	*pwd;
	char	*oldpwd;

	pwd = ms_get_var_env(data, "PWD");
	if (pwd)
	{
		oldpwd = ft_strjoin("OLD", pwd);
		if (!oldpwd)
			return (ERR_MALLOC);
// malloc failed
		if (ms_export_var(data, oldpwd))
			return (ERR_MALLOC);
// malloc failed (i think this is the only fail from export_var)
	}
	else
		ms_unset_var(data, "OLDPWD");
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (ms_perror("cd", NULL, NULL, errno));
// error from getcwd
	pwd = ft_strjoin("PWD=", oldpwd);
	free(oldpwd);
	if (!pwd)
		return (ERR_MALLOC);
// error from ft_strjoin
	if (ms_export_var(data, pwd))
		return (ERR_MALLOC);
// malloc failed (i think this is the only fail from export_var)
	return (0);
}

// seems to always return 1 on error
int	ms_cd(t_shellshock *data, char **arg)
{
	char	*path;

	path = arg[1];
	if (!path)
	{
		path = getenv("HOME");
		if (!path)
			return (ms_perror("cd", NULL, "HOME not set", 0));
	}
	else if (arg[2])
		return (ms_perror("cd", NULL, "too many arguments", 0));
	if (!*path)
		return (0);
	if (arg[1] && !ft_strncmp(arg[1], "-", 2))
		path = getenv("OLDPWD");
	if (!path)
		return (ms_perror("cd", NULL, "OLDPWD not set", 0));
	if (chdir(path))
		return (ms_perror("cd", path, NULL, errno));
	if (arg[1] && !ft_strncmp(arg[1], "-", 2))
		if (ms_pwd())
			return (1);
// error!? (can only be malloc i think? kinda? except for getcwd maybe)
	if (ms_update_pwd_env(data))
		return (1);
// error!? (can only be malloc i think? kinda? except for getcwd maybe)
	return (0);
}
