#include "main.h"
#include <stdlib.h>

/**
 * alloc_grid - returns a pointer to a 2D array of integers
 * @width: width of the grid
 * @height: height of the grid
 *
 * Return: pointer to the 2D array, or NULL on failure
 */
int **alloc_grid(int width, int height)
{
	int **grid;
	int h, w;

	if (width <= 0 || height <= 0)
		return (NULL);

	grid = malloc(sizeof(int *) * height);
	if (grid == NULL)
		return (NULL);

	for (h = 0; h < height; h++)
	{
		grid[h] = malloc(sizeof(int) * width);
		if (grid[h] == NULL)
		{
			while (h > 0)
			{
				h--;
				free(grid[h]);
			}
			free(grid);
			return (NULL);
		}
		for (w = 0; w < width; w++)
			grid[h][w] = 0;
	}

	return (grid);
}

