/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_redirection_filters.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 15:23:13 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/27 15:23:15 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

int open_all(t_redirection *redirection)
{
	(void)redirection;
	return (1);
}

int	open_prepend(t_redirection *redirection)
{
	return (redirection->redirection_flags & MS_PREPEND);
}
