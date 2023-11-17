/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 14:45:06 by bvercaem          #+#    #+#             */
/*   Updated: 2023/11/17 16:49:51 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// prints error like (shellshock: cmd: item: msg: err)
// handles NULL pointers, and err <= 0
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
