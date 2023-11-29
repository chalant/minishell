# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/16 17:36:04 by bvercaem          #+#    #+#              #
#    Updated: 2023/11/28 18:25:04 by ychalant         ###   ########.fr        #
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
				 minishell_semantics.h

SRC_FILES =	minishell.c \
			minishell_exit.c \
			minishell_cd.c \
			minishell_pwd.c \
			minishell_export.c \
			minishell_unset.c \
			minishell_variable_util.c \
			minishell_env.c \
			minishell_echo.c \
			minishell_wildcard.c \
			minishell_expand_var.c \
			minishell_error.c \
			minishell_tokens.c \
			minishell_tokens_util.c \
			minishell_token_expansion.c \
			minishell_recogniser.c \
			minishell_parser.c \
			minishell_parse_tree.c \
			minishell_graph.c \
			minishell_grammar.c \
			minishell_grammar_rules.c \
			minishell_grammar_symbols.c \
			minishell_grammar_matching.c \
			minishell_semantics.c \
			minishell_execution.c \
			minishell_path.c \
			minishell_commands.c \
			minishell_redirections.c \
			minishell_semantic_operators.c \
			minishell_semantics_utils.c \
			minishell_earley_set.c

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
