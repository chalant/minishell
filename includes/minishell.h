/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 17:37:28 by bvercaem          #+#    #+#             */
/*   Updated: 2023/11/14 16:35:16 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "minishell_parsing.h"
#include "minishell_graph.h"
#include "minishell_tokens.h"
#include "minishell_errors.h"
#include "minishell_commands.h"
#include "minishell_semantics.h"
#include "libft.h"

int	connect_pipes(int in_pipe[2], int out_pipe[2]);

#endif