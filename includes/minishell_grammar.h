#ifndef MINISHELL_GRAMMAR_H
# define MINISHELL_GRAMMAR_H

#define MNISHELL_TEST_GRAMMAR "sum:sum <'+-' product\n"\
							  "sum:product\n"\
							  "product:product <'*/' factor\n"\
							  "product:factor\n"\
							  "factor:='(' sum =')'\n"\
							  "factor:number\n"\
							  "number:<'0123456789' number\n"\
							  "number:<'0123456789'\n"

#ifndef MINISHELL_GRAMMAR
# define MINISHELL_GRAMMAR "word:%'string'\n"\
						   "assignment-word:%'='\n"\
						   "number:%'integer'\n"\
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
						   "list0:list1 %'NL' newline_list\n"\
						   "list0:list1 ='&' newline_list\n"\
						   "list0:list1 =';' newline_list\n"\
						   "list1:list1 ='&&' newline_list list1\n"\
						   "list1:list1 ='||' newline_list list1\n"\
						   "list1:list1 ='&' newline_list list1\n"\
						   "list1:list1 =';' newline_list list1\n"\
						   "list1:list1 %'NL' newline_list list1\n"\
						   "list1:pipeline_command\n"\
						   "newline_list:newline_list %'NL'\n"\
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
#endif
#endif
