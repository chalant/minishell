#ifndef MINISHELL_GRAMMAR_H
# define MINISHELL_GRAMMAR_H

# define MS_NEW_LINE '0'
# define MS_EQUAL '1'
# define MS_INTEGER '2'
# define MS_STRING '3'

typedef struct	s_ms_symbol
{
	char		*name;
	int			symbol_type;
	int			rule;
	int			(*match)(struct s_ms_symbol*, char *);
}				t_ms_symbol;

typedef struct	s_ms_rule
{
	int			rule_id;
	int			length;
	const char	*name;
	t_ms_symbol	**symbols;
}				t_ms_rule;

typedef struct	s_ms_grammar
{
	const char		*start_rule;
	const char		*name;
	t_ms_rule		**rules;
	int				length;
}				t_ms_grammar;

# define MINISHELL_GRAMMAR "word:%'3'\n"\
						   "assignment-word:%'1'\n"\
						   "number:%'2'\n"\
						   "word_list:word\n"\
						   "word_list:word_list word\n"\
						   "redirection:='>' word\n"\
						   "redirection:='<' word\n"\
						   "redirection:number ='>' word\n"\
						   "redirection:number ='<' word\n"\
						   "redirection:='>>' word\n"\
						   "redirection:number ='>>' word\n"\
						   "redirection:='<<' word\n"\
						   "redirection:number ='<<' word\n"\
						   "simple_command_element:word\n"\
						   "simple_command_element:assignment-word\n"\
						   "simple_command_element:redirection\n"\
						   "redirection_list:redirection\n"\
						   "redirection_list:redirection_list redirection\n"\
						   "simple_command:simple_command_element\n"\
						   "simple_command:simple_command simple_command_element\n"\
						   "command:simple_command\n"\
						   "command:shell_command\n"\
						   "command:shell_command redirection_list\n"\
						   "shell_command:subshell\n"\
						   "subshell:'(' compound_list ')'\n"\
						   "compound_list:newline_list list0\n"\
						   "list0:list1 %'0' newline_list\n"\
						   "list0:list1 ='&' newline_list\n"\
						   "list0:list1 =';' newline_list\n"\
						   "list1:list1 ='&&' newline_list list1\n"\
						   "list1:list1 ='||' newline_list list1\n"\
						   "list1:list1 ='&' newline_list list1\n"\
						   "list1:list1 =';' newline_list list1\n"\
						   "list1:list1 %'0' newline_list list1\n"\
						   "list1:pipeline_command\n"\
						   "newline_list:newline_list %'0'\n"\
						   "simple_list:simple_list1\n"\
						   "simple_list:simple_list1 ='&'\n"\
						   "simple_list:simple_list1 =';'\n"\
						   "simple_list1:simple_list1 ='&&' newline_list simple_list1\n"\
						   "simple_list1:simple_list1 ='||' newline_list simple_list1\n"\
						   "simple_list1:simple_list1 ='&' simple_list1\n"\
						   "simple_list1:simple_list1 =';' simple_list1\n"\
						   "simple_list1:pipeline_command\n"\
						   "pipeline_command:pipeline\n"\
						   "pipeline:pipeline ='|' newline_list pipeline\n"\
						   "pipeline:pipeline ='|&' newline_list pipeline\n"\
						   "pipeline:command\n"

int	set_grammar(t_ms_grammar *grammar, const char *definition);
const char *get_test_definition(void);
// int	get_minishell_definition(char **dest);

#endif
