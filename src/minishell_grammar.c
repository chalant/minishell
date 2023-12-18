/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_grammar.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 13:49:44 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/18 13:44:16 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static const char	*g_minishell_grammar = "word:$'3'\n"\

	"redirection:='>' word\n"\
	"redirection:='<' word\n"\
	"redirection:='>>' word\n"\
	"redirection:='<<' word\n"\

	"redirection_list:redirection\n"\
	"redirection_list:redirection redirection_list\n"\

	"command_argument:word command_argument\n"\
	"command_argument:word\n"\

	"command_element:redirection\n"\
	"command_element:word\n"\
	"command_element:word command_element\n"\
	"command_element:redirection command_element\n"\

	"simple_command:word command_element\n"\
	"simple_command:word\n"\

	"parenthesis:='(' command =')' redirection_list\n"\
	"parenthesis:='(' command $'4' redirection_list\n"\
	"parenthesis:='(' command =')'\n"\
	"parenthesis:='(' command $'4'\n"\

	"command_factor:parenthesis\n"\
	"command_factor:simple_command\n"\
	"command_factor:redirection_list\n"\

	"command_operand:command_factor\n"\
	"command_operand:command_operand ='|' $'4'\n"\
	"command_operand:command_operand ='|' command_factor\n"\

	"red_command:simple_command\n"\
	"red_command:red_command ='|' $'4'\n"\
	"red_command:red_command ='|' command_factor\n"\

	"redirection_command:redirection_list\n"\
	"redirection_command:redirection_list red_command\n"\

	"command:redirection_command\n"\
	"command:command_operand\n"\
	"command:command ='&&' $'4'\n"\
	"command:command ='||' $'4'\n"\
	"command:command ='||' command_operand\n"\
	"command:command ='&&' command_operand\n";

char	**ft_reverse_strings(char **strings)
{
	int		i;
	int		j;
	int		max;
	char	*tmp;

	if (!strings)
		return (NULL);
	i = 0;
	j = ft_count_strings(strings) - 1;
	max = j + 1;
	while (i < j)
	{
		tmp = strings[i];
		strings[i] = strings[j];
		strings[j] = tmp;
		i++;
		j--;
	}
	strings[max] = 0;
	return (strings);
}

int	set_minishell_grammar(t_ms_grammar *grammar)
{
	char	**definition;

	definition = ft_reverse_strings(ft_split(g_minishell_grammar, '\n'));
	if (!definition)
	{
		ft_clear_ds(definition);
		return (-1);
	}
	if (set_grammar(grammar, definition) < 0)
	{
		ft_clear_ds(definition);
		return (-1);
	}
	ft_clear_ds(definition);
	return (1);
}

static void	delete_rule(t_ms_rule *rule)
{
	if (rule->name)
		free(rule->name);
	if (rule->symbols)
	{
		while (rule->length--)
		{
			free(rule->symbols[rule->length]->name);
			free(rule->symbols[rule->length]);
		}
		free(rule->symbols);
	}
	free(rule);
}

void	delete_grammar(t_ms_grammar *grammar)
{
	if (!grammar)
		return ;
	if (grammar->rules)
	{
		while (grammar->length--)
			delete_rule(grammar->rules[grammar->length]);
		free(grammar->rules);
	}
}

int	set_grammar(t_ms_grammar *grammar, char **definition)
{
	int	i;

	i = 0;
	if (!definition)
		return (-1);
	grammar->rules = malloc(ft_count_strings(definition) * sizeof(t_ms_rule *));
	if (!grammar->rules)
		return (-1);
	while (definition[i])
	{
		if (add_rule(grammar, definition, i) < 0)
			return (-1);
		i++;
	}
	grammar->start_rule = grammar->rules[0]->name;
	grammar->length = i;
	return (1);
}
