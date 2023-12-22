/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_export_print.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 15:20:31 by bvercaem          #+#    #+#             */
/*   Updated: 2023/12/22 15:29:31 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ms_sort_env(char **envp, int size)
{
	int		i;
	char	*temp;

	while (size > 1)
	{
		i = 0;
		while (i + 1 < size)
		{
			if (ft_strncmp(envp[i], envp[i + 1], ft_strlen(envp[i])) > 0)
			{
				temp = envp[i];
				envp[i] = envp[i + 1];
				envp[i + 1] = temp;
			}
			i++;
		}
		size--;
	}
}

static void	ms_env_alpha_print(char **temp_envp, int fd)
{
	int		i;
	char	*equal_sign;

	i = 0;
	while (temp_envp[i])
	{
		equal_sign = ft_strchr(temp_envp[i], '=');
		*equal_sign = 0;
		write(fd, "declare -x ", 11);
		write(fd, temp_envp[i], ft_strlen(temp_envp[i]));
		*equal_sign = '=';
		write(fd, "=\"", 2);
		write(fd, equal_sign + 1, ft_strlen(equal_sign + 1));
		write(fd, "\"\n", 2);
		i++;
	}
}

// prints env but in order of ascending ascii value
// error: nomem, prints msg
int	ms_env_alpha(char **env, int fd)
{
	int		i;
	char	**temp_envp;

	i = 0;
	while (env[i])
		i++;
	temp_envp = malloc(sizeof(char *) * (i + 1));
	if (!temp_envp)
		return (ms_perror("export", NULL, NULL, errno));
	i = -1;
	while (env[++i])
		temp_envp[i] = env[i];
	temp_envp[i] = NULL;
	ms_sort_env(temp_envp, i);
	ms_env_alpha_print(temp_envp, fd);
	free(temp_envp);
	return (0);
}
