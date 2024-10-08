/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychalant <ychalant@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 12:08:36 by ychalant          #+#    #+#             */
/*   Updated: 2023/12/23 11:37:20 by ychalant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# define FT_LIFO 0
# define FT_FIFO 1

# include <stdlib.h>
# include <string.h>

# define DARRAY_INCREMENT 100

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}				t_list;

typedef struct s_darray
{
	void		*contents;
	int			max_size;
	int			block_size;
	int			size;
	int			actual_size;
	int			type_size;
}				t_darray;

typedef struct s_stack
{
	int			top;
	int			size;
	int			mode;
	t_darray	*elements;
}				t_stack;

int					ft_isalnum(int c);
int					ft_isalpha(int c);
int					ft_isascii(int c);
int					ft_isdigit(int c);
int					ft_isprint(int c);
int					ft_tolower(int c);
int					ft_toupper(int c);
int					ft_atoi(const char *nptr);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
int					ft_strcmp(const char *s1, const char *s2);
int					ft_memcmp(const void *s1, const void *s2, size_t n);

void				ft_bzero(void *s, size_t n);
void				ft_putchar_fd(char c, int fd);
void				ft_putendl_fd(char *s, int fd);
void				ft_putstr_fd(char *s, int fd);
void				ft_putnbr_fd(int n, int fd);

void				*ft_calloc(size_t nmemb, size_t size);
void				*ft_memcpy(void *dest, const void *src, size_t n);
void				*ft_memccpy(void *dest, const void *src, int c, size_t n);
void				*ft_memmove(void *dest, const void *src, size_t n);
void				*ft_memset(void *s, int c, size_t n);
void				*ft_memchr(const void *s, int c, size_t n);
void				*ft_striteri(char *s, void (*f)(unsigned int, char *));

char				*ft_strchr(const char *s, int c);
char				*ft_strnstr(const char *big, const char *little,
						size_t len);
char				*ft_strrchr(const char *s, int c);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
char				*ft_strdup(const char *s);
char				*ft_strtrim(char const *s1, char const *set);
char				*ft_itoa(int n);
char				*ft_substr(char const *s, unsigned int start, size_t len);

char				**ft_split(char const *s, char c);
char				**ft_setsplit(char *str, char *charset);
char				**ft_clear_ds(char **s);

size_t				ft_strlcat(char *dst, const char *src, size_t size);
size_t				ft_strlcpy(char *dst, const char *src, size_t size);
size_t				ft_strlen(const char *s);
size_t				ft_count_strings(char **strings);

int					ft_lstsize(t_list *lst);

void				ft_lstadd_front(t_list **lst, t_list *new);
void				ft_lstadd_back(t_list **lst, t_list *new);
void				ft_lstdelone(t_list *lst, void (*del)(void *));
void				ft_lstclear(t_list **lst, void (*del)(void *));
void				ft_lstiter(t_list *lst, void (*f)(void *));

t_list				*ft_lstlast(t_list *lst);
t_list				*ft_lstmap(t_list *lst, void *(*f)(void *),
						void (*del)(void *));

int					ft_darray_exists(t_darray *darray, int index);
void				*ft_darray_get(t_darray *darray, int index);
int					ft_darray_init(t_darray *darray, int type_size, int size);
int					ft_darray_set(t_darray *darray, void *element, int index);
int					ft_darray_append(t_darray *darray, void *element);
int					ft_darray_reset(t_darray *darray, void del_content(void *));
int					ft_darray_delete(t_darray *darray,
						void del_content(void *));
int					ft_darray_reverse(t_darray *darray);
int					ft_darray_join(t_darray *darray, t_darray *with);
int					ft_darray_slice(t_darray *darray, t_darray *result,
						int start, int end);
int					ft_darray_insert(t_darray *darray, t_darray *with,
						t_darray *tmp, int at);
int					ft_darray_onsert(t_darray *darray, t_darray *with,
						t_darray *tmp, int at);
void				*ft_stack_peek(t_stack *stack);
void				*ft_stack_pop(t_stack *stack);
int					ft_stack_init(t_stack *stack, t_darray *darray);
int					ft_stack_push(t_stack *stack, void *element);

#endif
