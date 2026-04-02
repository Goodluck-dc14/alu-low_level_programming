#include "main.h"
#include <stdlib.h>

/**
 * free_grid - frees a 2D grid
 * @grid: the grid to free
 * @height: height of the grid
 */
void free_grid(int **grid, int height)
{
	int h;

	if (grid == NULL)
		return;

	for (h = 0; h < height; h++)
		free(grid[h]);

	free(grid);
}

