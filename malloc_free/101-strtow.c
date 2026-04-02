#include "main.h"
#include <stdlib.h>

/**
 * word_count - counts words in a string
 * @str: the string
 *
 * Return: number of words
 */
int word_count(char *str)
{
	int count = 0;
	int in_word = 0;

	while (*str != '\0')
	{
		if (*str == ' ')
			in_word = 0;
		else if (in_word == 0)
		{
			in_word = 1;
			count++;
		}
		str++;
	}
	return (count);
}

/**
 * word_len - gets length of current word
 * @str: the string starting at a word
 *
 * Return: length of the word
 */
int word_len(char *str)
{
	int len = 0;

	while (str[len] != '\0' && str[len] != ' ')
		len++;

	return (len);
}

/**
 * free_all - frees array of strings
 * @words: the array
 * @count: number of strings to free
 */
void free_all(char **words, int count)
{
	int i;

	for (i = 0; i < count; i++)
		free(words[i]);
	free(words);
}

/**
 * strtow - splits a string into words
 * @str: the string to split
 *
 * Return: pointer to array of strings, or NULL
 */
char **strtow(char *str)
{
	char **words;
	int i, j, wc, wl, pos = 0;

	if (str == NULL || *str == '\0')
		return (NULL);
	wc = word_count(str);
	if (wc == 0)
		return (NULL);
	words = malloc(sizeof(char *) * (wc + 1));
	if (words == NULL)
		return (NULL);
	for (i = 0; i < wc; i++)
	{
		while (str[pos] == ' ')
			pos++;
		wl = word_len(str + pos);
		words[i] = malloc(sizeof(char) * (wl + 1));
		if (words[i] == NULL)
		{
			free_all(words, i);
			return (NULL);
		}
		for (j = 0; j < wl; j++)
		{
			words[i][j] = str[pos];
			pos++;
		}
		words[i][j] = '\0';
	}
	words[wc] = NULL;
	return (words);
}

