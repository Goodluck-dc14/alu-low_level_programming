#include "main.h"

/**
 * check_prime - helper to check prime recursively
 * @n: the number to check
 * @i: current divisor
 *
 * Return: 1 if prime, 0 if not
 */
int check_prime(int n, int i)
{
	if (n % i == 0)
		return (0);
	if (i * i > n)
		return (1);

	return (check_prime(n, i + 1));
}

/**
 * is_prime_number - returns 1 if prime, 0 otherwise
 * @n: the number to check
 *
 * Return: 1 if prime, 0 if not
 */
int is_prime_number(int n)
{
	if (n <= 1)
		return (0);

	return (check_prime(n, 2));
}

