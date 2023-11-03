/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 14:45:06 by bvercaem          #+#    #+#             */
/*   Updated: 2023/11/03 14:02:19 by bvercaem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// prints error like (shellshock: cmd: msg: errno)
// handles NULL pointers
// doesn't free any arguments
// returns return_value
int	ms_print_error(const char *cmd, const char *msg, int return_value)
{
	extern int	errno;

// could just return errno? not sure if that is how it works
	printf("shellshock: ");
	if (cmd)
		printf("%s: ", cmd);
	if (msg)
		printf("%s: ", msg);
	printf("%s\n", strerror(errno));
	return (return_value);
}
