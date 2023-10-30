# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bvercaem <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/16 17:36:04 by bvercaem          #+#    #+#              #
#    Updated: 2023/10/30 16:25:12 by bvercaem         ###   ########.fr        #
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
				 minishell_grammar.h

SRC_FILES = minishell_tokens.c \
			minishell.c \
			minishell_recogniser.c \
			minishell_parser.c \
			minishell_graph.c \
			minishell_grammar.c \
			minishell_grammar_rules.c \
			minishell_grammar_symbols.c \
			minishell_grammar_matching.c

INCLUDES = $(addprefix $(INCLUDES_DIR)/, $(INCLUDES_FILES))
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

NAME = minishell

LIBFT_DIR = ./src/libft
LIBFT = $(LIBFT_DIR)/libft.a

C_FLAGS = -g -O3 -I$(INCLUDES_DIR) -I$(LIBFT_DIR) -I$(SRC_DIR) -Wall -Wextra -Werror

all:
	mkdir -p $(OBJ_DIR)
	make $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	cc $(C_FLAGS) $(OBJ) $(LIBFT) -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c $(INCLUDES)
	cc $(C_FLAGS) -c $< -o $@

clean:
	make -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

bonus: all

re: fclean all

.PHONY: all clean fclean bonus re
