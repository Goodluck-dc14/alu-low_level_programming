#include "main.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * error_exit - prints error and exits
 * @code: exit code
 * @msg: error message format
 * @name: file or fd info
 */
void error_exit(int code, char *msg, char *name)
{
	dprintf(STDERR_FILENO, msg, name);
	exit(code);
}

/**
 * close_or_exit - closes fd or exits with error
 * @fd: file descriptor to close
 */
void close_or_exit(int fd)
{
	char buf[16];
	int i = 0, n, tmp;

	if (close(fd) == -1)
	{
		n = fd;
		if (n == 0)
			buf[i++] = '0';
		else
		{
			tmp = n;
			while (tmp > 0)
			{
				tmp /= 10;
				i++;
			}
			buf[i] = '\0';
			tmp = i;
			while (n > 0)
			{
				buf[--tmp] = (n % 10) + '0';
				n /= 10;
			}
		}
		buf[i] = '\0';
		dprintf(STDERR_FILENO, "Error: Can't close fd %s\n", buf);
		exit(100);
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
	ssize_t rd, wr;
	char buf[1024];

	if (argc != 3)
	{
		dprintf(STDERR_FILENO, "Usage: cp file_from file_to\n");
		exit(97);
	}

	fd_from = open(argv[1], O_RDONLY);
	if (fd_from == -1)
		error_exit(98, "Error: Can't read from file %s\n", argv[1]);

	fd_to = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0664);
	if (fd_to == -1)
	{
		close_or_exit(fd_from);
		error_exit(99, "Error: Can't write to %s\n", argv[2]);
	}

	while ((rd = read(fd_from, buf, 1024)) > 0)
	{
		wr = write(fd_to, buf, rd);
		if (wr == -1 || wr != rd)
		{
			close_or_exit(fd_from);
			close_or_exit(fd_to);
			error_exit(99, "Error: Can't write to %s\n", argv[2]);
		}
	}

	if (rd == -1)
	{
		close_or_exit(fd_from);
		close_or_exit(fd_to);
		error_exit(98, "Error: Can't read from file %s\n", argv[1]);
	}

	close_or_exit(fd_from);
	close_or_exit(fd_to);

	return (0);
}

