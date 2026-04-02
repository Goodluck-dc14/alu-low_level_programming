#include "main.h"

/**
 * check_sqrt - helper to find square root recursively
 * @n: the number
 * @i: current guess
 *
 * Return: i if i*i == n, -1 if no natural square root
 */
int check_sqrt(int n, int i)
{
	if (i * i == n)
		return (i);
	if (i * i > n)
		return (-1);

	return (check_sqrt(n, i + 1));
}

/**
 * _sqrt_recursion - returns the natural square root of a number
 * @n: the number
 *
 * Return: square root, or -1 if no natural square root
 */
int _sqrt_recursion(int n)
{
	if (n < 0)
		return (-1);

	return (check_sqrt(n, 1));
}

