INCLUDES_DIR = ./includes
SRC_DIR = ./src
OBJ_DIR = ./obj

INCLUDES_FILES = minishell.h \
				 #minishell_parsing.h

SRC_FILES = minishell_cd.c \
			#minishell_pwd.c \
			#minishell_export.c \
			minishell_variable_util.c \
			minishell_env.c \
			#minishell_main.c \
			#minishell_echo.c \
			#minishell_recogniser.c \
			minishel.c \
			minishell_parser.c

INCLUDES = $(addprefix $(INCLUDES_DIR)/, $(INCLUDES_FILES))
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

NAME = minishell

LIBFT_DIR = ./src/libft
LIBFT = $(LIBFT_DIR)/libft.a

C_FLAGS = -g -O3 -I$(INCLUDES_DIR) -I$(LIBFT_DIR) -I$(SRC_DIR) -Wall -Wextra -Werror  -I ~/homebrew/opt/readline/include
#READLINE = -lreadline -lhistory -L ~/homebrew/opt/readline/lib

all:
	mkdir -p $(OBJ_DIR)
	make $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	cc $(C_FLAGS) $(OBJ) $(LIBFT) -o $(NAME) $(READLINE)

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
