# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/16 17:36:04 by bvercaem          #+#    #+#              #
#    Updated: 2023/12/18 16:37:04 by ychalant         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

INCLUDES_DIR = ./includes
SRC_DIR = ./src
OBJ_DIR = ./obj

INCLUDES_FILES = minishell.h \
				 minishell_errors.h \
				 minishell_tokens.h \
				 minishell_graph.h \
				 minishell_parsing.h \
				 minishell_grammar.h \
				 minishell_commands.h \
				 minishell_semantics.h \
				 minishell_execution.h

SRC_FILES =	minishell_main.c \
			minishell_signal_handlers.c \
			minishell_exit.c \
			minishell_cd.c \
			minishell_pwd.c \
			minishell_export.c \
			minishell_unset.c \
			minishell_variable_util.c \
			minishell_env.c \
			minishell_echo.c \
			minishell_wildcard.c \
			minishell_wildcard_cmp.c \
			minishell_wildcard_cmp_util.c \
			minishell_wildcard_add.c \
			minishell_expand_var.c \
			minishell_malloc_getenv.c \
			minishell_add_variable_token.c \
			minishell_error.c \
			minishell_tokens.c \
			minishell_tokeniser_parser.c \
			minishell_tokens_util.c \
			minishell_token_expansion.c \
			minishell_recogniser_utils.c \
			minishell_recogniser.c \
			minishell_treebuilder_utils.c \
			minishell_tree_builder.c \
			minishell_parse_tree.c \
			minishell_parse_data.c \
			minishell_parser_utils.c \
			minishell_parser.c \
			minishell_graph.c \
			minishell_grammar.c \
			minishell_grammar_rules.c \
			minishell_grammar_symbols.c \
			minishell_grammar_matching.c \
			minishell_grammar_utils.c \
			minishell_semantics.c \
			minishell_pipe_utils.c \
			minishell_execution_operators.c \
			minishell_execution_pipe.c \
			minishell_execution_io.c \
			minishell_execution_args.c \
			minishell_execution_builtin.c \
			minishell_execution_operands.c \
			minishell_execution.c \
			minishell_path.c \
			minishell_command_utils.c \
			minishell_command_factory.c \
			minishell_redirection_utils.c \
			minishell_redirection_factory.c \
			minishell_semantic_operators.c \
			minishell_semantic_utils.c \
			minishell_earley_set.c \
			minishell_history_utils.c \
			minishell_heredoc.c

INCLUDES = $(addprefix $(INCLUDES_DIR)/, $(INCLUDES_FILES))
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

NAME = minishell

LIBFT_DIR = ./src/libft
LIBFT = $(LIBFT_DIR)/libft.a

C_FLAGS = -Wall -Wextra -Werror -g -O3 -Wno-unused-result -I$(INCLUDES_DIR) -I$(LIBFT_DIR) -I$(SRC_DIR)
#C_FLAGS = -g -O3 -I$(INCLUDES_DIR) -I$(LIBFT_DIR) -I$(SRC_DIR)
READLINE = -lreadline -lhistory -L/Users/$(USER)/homebrew/opt/readline/lib
IREADLINE = -I/Users/$(USER)/homebrew/opt/readline/include
#READLINE = -lreadline -lhistory

all:
	mkdir -p $(OBJ_DIR)
	make $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	cc $(C_FLAGS) $(OBJ) $(LIBFT) $(READLINE) -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c $(INCLUDES)
	cc $(C_FLAGS) $(IREADLINE) -c $< -o $@

clean:
	make -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

bonus: all

re: fclean all

.PHONY: all clean fclean bonus re
