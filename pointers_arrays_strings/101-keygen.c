#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * main - generates random valid passwords for 101-crackme
 *
 * Return: Always 0
 */
int main(void)
{
	int sum = 0;
	int c;

	srand(time(NULL));

	while (sum < 2772)
	{
		c = (rand() % 126) + 1;
		if (sum + c > 2772)
			c = 2772 - sum;
		sum += c;
		printf("%c", c);
	}

	return (0);
}

