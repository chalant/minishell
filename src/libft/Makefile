C_FLAGS = -O3 -Wall -Wextra -Werror -Wno-unused-result
SRC_FILES = ft_atoi.c \
			ft_bzero.c \
			ft_calloc.c \
			ft_isalnum.c \
			ft_isalpha.c \
			ft_isascii.c \
			ft_isdigit.c \
			ft_isprint.c \
			ft_itoa.c \
			ft_memccpy.c \
			ft_memchr.c \
			ft_memcmp.c \
			ft_memcpy.c \
			ft_memmove.c \
			ft_memset.c \
			ft_putchar_fd.c \
			ft_putendl_fd.c \
			ft_putnbr_fd.c \
			ft_putstr_fd.c \
			ft_split.c \
			ft_setsplit.c \
			ft_strchr.c \
			ft_strdup.c \
			ft_striteri.c \
			ft_strjoin.c \
			ft_strlcat.c \
			ft_strlcpy.c \
			ft_strlen.c \
			ft_strmapi.c \
			ft_strncmp.c \
			ft_strcmp.c \
			ft_strnstr.c \
			ft_strrchr.c \
			ft_strtrim.c \
			ft_substr.c \
			ft_tolower.c \
			ft_toupper.c \
			ft_darray.c \
			ft_darray_interface.c \
			ft_darray_operations.c \
			ft_stack.c \
			ft_count.c

BONUS_FILES = ft_lstnew_bonus.c \
			  ft_lstadd_front_bonus.c \
			  ft_lstsize_bonus.c \
			  ft_lstlast_bonus.c \
			  ft_lstadd_back_bonus.c \
			  ft_lstdelone_bonus.c \
			  ft_lstclear_bonus.c \
			  ft_lstiter_bonus.c \
			  ft_lstmap_bonus.c

OBJ = $(SRC_FILES:.c=.o)
BONUS_OBJ = $(BONUS_FILES:.c=.o)
NAME = libft.a

all: $(NAME)

$(NAME): $(OBJ)
	ar rc $(NAME) $(OBJ)

%.o:%.c libft.h
	cc $(C_FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(BONUS_OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

bonus: $(BONUS_OBJ)
	ar r $(NAME) $(BONUS_OBJ)
