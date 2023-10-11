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
# define MINISHELL_GRAMMAR "alpha:<'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ'\n"\
						   "digit:<'0123456789'\n"\
						   "number:digit\n"\
						   "number:number digit\n"\
						   "word:alpha\n"\
						   "word:word alpha\n"\
						   "word:word ='_'\n"\
						   "word-list:word\n"\
						   "word-list:word-list word\n"\
						   "assignment-word:word ='=' word\n"\
						   "redirection:='>' word\n"\
						   "redirection:='<' word\n"\
						   "redirection:number ='>' word\n"\
						   "redirection:number ='<' word\n"\
						   "redirection:='>>' word\n"\
						   "redirection:number ='>>' word\n"\
						   "redirection:='<<' word\n"\
						   "redirection:number ='<<' word\n"
#endif
#endif
