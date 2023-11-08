/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_grammar.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 13:49:44 by ychalant          #+#    #+#             */
/*   Updated: 2023/10/30 13:17:53 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static const char	*g_test_grammar = "sum:sum <'+-' product\n"\
	"sum:product\n"\
	"product:product <'*/' factor\n"\
	"product:factor\n"\
	"factor:='(' sum =')'\n"\
	"factor:number\n"\
	"number:<'0123456789' number\n"\
	"number:<'0123456789'\n";

static const char	*g_minishell_grammar = "word:$'3'\n"\
	"number:$'2'\n"\
	"assignment-word:$'1'\n"\

	"redirection:='>' word\n"\
	"redirection:='<' word\n"\
	"redirection:='>>' word\n"\
	"redirection:='<<' word\n"\

	"redirection_list:redirection\n"\
	"redirection_list:redirection redirection_list\n"\

	"command_argument:word command_argument\n"\
	"command_argument:word\n"\

	"builtin:='echo'\n"\
	"builtin:='cd'\n"\
	"builtin:='pwd'\n"\
	"builtin:='export'\n"\
	"builtin:='env'\n"\
	"builtin:='unset'\n"\
	"builtin:='exit'\n"\

	"command_element:command_argument redirection_list\n"\
	"command_element:redirection_list\n"\
	"command_element:command_argument\n"\

	"simple_command:word command_element\n"\
	"simple_command:word\n"\
	"simple_command:builtin command_element\n"\
	"simple_command:builtin\n"\

	"redirection_command:redirection_list\n"\
	"redirection_command:='(' redirection_list =')' redirection_list\n"\
	"redirection_command:redirection_list simple_command\n"\

	"subshell:='(' pipeline =')'\n"\

	"command:command ='||' command_factor\n"\
	"command:command ='&&' command_factor\n"\
	"command:command_factor\n"\
	"command_factor:subshell\n"\
	"command_factor:subshell redirection_list\n"\
	"command_factor:simple_command\n"\
	"command_factor:redirection_command\n"\

	"pipeline:command\n"\
	"pipeline:pipeline ='|' command\n";

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

char	**get_test_definition(void)
{
	return (ft_split(g_test_grammar, '\n'));
}

char	**get_minishell_definition(void)
{
	return (ft_reverse_strings(ft_split(g_minishell_grammar, '\n')));
}

//todo use t_darray for splits, symbols and rules.
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
		add_rule(grammar, definition, i);
		i++;
	}
	grammar->start_rule = grammar->rules[0]->name;
	grammar->length = i;
	return (1);
}

int	print_grammar(t_ms_grammar *grammar)
{
	int			i;
	int			j;
	t_ms_rule	*rule;

	i = 0;
	while (i < grammar->length)
	{
		rule = grammar->rules[i];
		printf("%s ->", rule->name);
		j = -1;
		while (++j < rule->length)
			printf(" %s", rule->symbols[j]->name);
		printf("\n");
		i++;
	}
	return (1);
}
