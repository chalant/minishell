#include "libft.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	n1;
	size_t	n2;

	n1 = ft_strlen(s1);
	n2 = ft_strlen(s2);
	if (n2 > n1)
		return (ft_strncmp(s1, s2, n2));
	return (ft_strncmp(s1, s2, n1));
}
