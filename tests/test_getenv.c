#include <stdlib.h>
#include <stdio.h>

int	main(int ac, char *av[])
{
	const char	*ret;

	if (ac < 2)
		return (1);
	ret = getenv(av[1]);
	if (!ret)
	{
		printf("ERROR: av[1] = '%s'\n", av[1]);
		perror(av[1]);
		return (1);
	}
	printf("%s = '%s'\n", av[1], ret);
	return (0);
}

/*
	only seems to grab the variables that also show with the bash command 'env'
*/
