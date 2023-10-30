#include "libft.h"

size_t	ft_count(void *elements, size_t type_size)
{
	int				i;
	unsigned char	*address;

	i = 0;
	address = (unsigned char *)elements;
	while (*address)
	{
		address += type_size * i;
		i++;
	}
	return (i);
}

size_t	ft_count_strings(char **strings)
{
	int	i;

	i = 0;
	while (strings[i])
		i++;
	return (i);
}
