#include <stdio.h>

/**
 * main - Prints the alphabet in lowercase and then in uppercase
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
	char low = 'a';
	char up = 'A';

	while (low <= 'z')
	{
		putchar(low);
		low++;
	}
	while (up <= 'Z')
	{
		putchar(up);
		up++;
	}
	putchar('\n');
	return (0);
}

