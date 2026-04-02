#include "main.h"
#include <stdlib.h>

/**
 * argstostr - concatenates all arguments of the program
 * @ac: argument count
 * @av: argument vector
 *
 * Return: pointer to new string, or NULL
 */
char *argstostr(int ac, char **av)
{
	char *str;
	int i, j, len = 0, pos = 0;

	if (ac == 0 || av == NULL)
		return (NULL);

	for (i = 0; i < ac; i++)
	{
		j = 0;
		while (av[i][j] != '\0')
		{
			len++;
			j++;
		}
		len++;
	}

	str = malloc(sizeof(char) * (len + 1));
	if (str == NULL)
		return (NULL);

	for (i = 0; i < ac; i++)
	{
		j = 0;
		while (av[i][j] != '\0')
		{
			str[pos] = av[i][j];
			pos++;
			j++;
		}
		str[pos] = '\n';
		pos++;
	}
	str[pos] = '\0';

	return (str);
}

