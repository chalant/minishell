/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 14:45:06 by bvercaem          #+#    #+#             */
/*   Updated: 2023/11/20 15:34:13 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// prints error like (shellshock: cmd: item: msg: err)
// handles NULL pointers, and err <= 0 (takes 'msg' as last arg)
// returns 1
int	ms_perror(const char *cmd, const char *item, const char *msg, int err)
{
	printf("shellshock: ");
	if (cmd)
		printf("%s: ", cmd);
	if (item)
		printf("%s: ", item);
	if (msg)
	{
		printf("%s", msg);
		if (err > 0)
			printf(": ");
	}
	if (err > 0)
		printf("%s", strerror(err));
	printf("\n");
	return (1);
}
