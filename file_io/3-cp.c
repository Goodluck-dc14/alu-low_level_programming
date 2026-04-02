#include "main.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * error_and_exit - prints error message and exits
 * @code: exit code
 * @msg: error message
 * @name: filename or fd string
 */
void error_and_exit(int code, char *msg, char *name)
{
	dprintf(STDERR_FILENO, msg, name);
	exit(code);
}

/**
 * safe_close - closes fd or exits with error
 * @fd: file descriptor
 */
void safe_close(int fd)
{
	int c;

	c = close(fd);
	if (c == -1)
	{
		dprintf(STDERR_FILENO, "Error: Can't close fd %d\n", fd);
		exit(100);
	}
}

/**
 * copy_content - copies from one fd to another
 * @fd_from: source fd
 * @fd_to: destination fd
 * @argv: argument vector for error messages
 */
void copy_content(int fd_from, int fd_to, char **argv)
{
	ssize_t rd, wr;
	char buf[1024];

	while ((rd = read(fd_from, buf, 1024)) > 0)
	{
		wr = write(fd_to, buf, rd);
		if (wr != rd)
		{
			safe_close(fd_from);
			safe_close(fd_to);
			error_and_exit(99,
				"Error: Can't write to %s\n", argv[2]);
		}
	}
	if (rd == -1)
	{
		safe_close(fd_from);
		safe_close(fd_to);
		error_and_exit(98,
			"Error: Can't read from file %s\n", argv[1]);
	}
}

/**
 * main - copies content of a file to another
 * @argc: argument count
 * @argv: argument vector
 *
 * Return: 0 on success
 */
int main(int argc, char *argv[])
{
	int fd_from, fd_to;

	if (argc != 3)
		error_and_exit(97, "Usage: cp file_from file_to\n", "");

	fd_from = open(argv[1], O_RDONLY);
	if (fd_from == -1)
		error_and_exit(98,
			"Error: Can't read from file %s\n", argv[1]);

	fd_to = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0664);
	if (fd_to == -1)
	{
		safe_close(fd_from);
		error_and_exit(99, "Error: Can't write to %s\n", argv[2]);
	}

	copy_content(fd_from, fd_to, argv);
	safe_close(fd_from);
	safe_close(fd_to);

	return (0);
}

