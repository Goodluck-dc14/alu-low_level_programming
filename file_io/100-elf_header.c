#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>

/**
 * print_magic - prints ELF magic bytes
 * @e: pointer to ELF header ident
 */
void print_magic(unsigned char *e)
{
	int i;

	printf("  Magic:   ");
	for (i = 0; i < EI_NIDENT; i++)
	{
		printf("%02x", e[i]);
		if (i < EI_NIDENT - 1)
			printf(" ");
	}
	printf("\n");
}

/**
 * print_class - prints ELF class
 * @e: pointer to ELF header ident
 */
void print_class(unsigned char *e)
{
	printf("  Class:                             ");
	if (e[EI_CLASS] == ELFCLASS32)
		printf("ELF32\n");
	else if (e[EI_CLASS] == ELFCLASS64)
		printf("ELF64\n");
	else
		printf("<unknown: %x>\n", e[EI_CLASS]);
}

/**
 * print_data - prints ELF data encoding
 * @e: pointer to ELF header ident
 */
void print_data(unsigned char *e)
{
	printf("  Data:                              ");
	if (e[EI_DATA] == ELFDATA2LSB)
		printf("2's complement, little endian\n");
	else if (e[EI_DATA] == ELFDATA2MSB)
		printf("2's complement, big endian\n");
	else
		printf("<unknown: %x>\n", e[EI_DATA]);
}

/**
 * print_version - prints ELF version
 * @e: pointer to ELF header ident
 */
void print_version(unsigned char *e)
{
	printf("  Version:                           %d", e[EI_VERSION]);
	if (e[EI_VERSION] == EV_CURRENT)
		printf(" (current)\n");
	else
		printf("\n");
}

/**
 * print_osabi - prints ELF OS/ABI
 * @e: pointer to ELF header ident
 */
void print_osabi(unsigned char *e)
{
	char *os[] = {
		"UNIX - System V",
		"UNIX - HP-UX",
		"UNIX - NetBSD",
		"UNIX - GNU",
		"",
		"",
		"UNIX - Solaris",
		"UNIX - AIX",
		"UNIX - IRIX",
		"UNIX - FreeBSD",
		"UNIX - Tru64",
		"Novell - Modesto",
		"UNIX - OpenBSD"
	};

	printf("  OS/ABI:                            ");
	if (e[EI_OSABI] < 13)
		printf("%s\n", os[(int)e[EI_OSABI]]);
	else
		printf("<unknown: %x>\n", e[EI_OSABI]);
}

/**
 * main - displays ELF header information
 * @argc: argument count
 * @argv: argument vector
 *
 * Return: 0 on success
 */
int main(int argc, char *argv[])
{
	int fd;
	unsigned char e[64];
	ssize_t rd;
	unsigned int type;
	unsigned long entry;

	if (argc != 2)
	{
		dprintf(STDERR_FILENO, "Usage: elf_header elf_filename\n");
		exit(98);
	}
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		dprintf(STDERR_FILENO, "Error: Can't read file %s\n", argv[1]);
		exit(98);
	}
	rd = read(fd, e, 64);
	if (rd < 16 || e[0] != 0x7f || e[1] != 'E' ||
	    e[2] != 'L' || e[3] != 'F')
	{
		dprintf(STDERR_FILENO, "Error: Not an ELF file - %s\n", argv[1]);
		close(fd);
		exit(98);
	}

	printf("ELF Header:\n");
	print_magic(e);
	print_class(e);
	print_data(e);
	print_version(e);
	print_osabi(e);
	printf("  ABI Version:                       %d\n", e[EI_ABIVERSION]);

	if (e[EI_CLASS] == ELFCLASS64)
	{
		type = e[16] | (e[17] << 8);
		if (e[EI_DATA] == ELFDATA2MSB)
			type = (e[16] << 8) | e[17];
	}
	else
	{
		type = e[16] | (e[17] << 8);
		if (e[EI_DATA] == ELFDATA2MSB)
			type = (e[16] << 8) | e[17];
	}

	printf("  Type:                              ");
	if (type == ET_NONE)
		printf("NONE (No file type)\n");
	else if (type == ET_REL)
		printf("REL (Relocatable file)\n");
	else if (type == ET_EXEC)
		printf("EXEC (Executable file)\n");
	else if (type == ET_DYN)
		printf("DYN (Shared object file)\n");
	else if (type == ET_CORE)
		printf("CORE (Core file)\n");
	else
		printf("<unknown: %x>\n", type);

	if (e[EI_CLASS] == ELFCLASS64)
	{
		if (e[EI_DATA] == ELFDATA2MSB)
		{
			entry = ((unsigned long)e[24] << 56) |
				((unsigned long)e[25] << 48) |
				((unsigned long)e[26] << 40) |
				((unsigned long)e[27] << 32) |
				((unsigned long)e[28] << 24) |
				((unsigned long)e[29] << 16) |
				((unsigned long)e[30] << 8) |
				(unsigned long)e[31];
		}
		else
		{
			entry = ((unsigned long)e[31] << 56) |
				((unsigned long)e[30] << 48) |
				((unsigned long)e[29] << 40) |
				((unsigned long)e[28] << 32) |
				((unsigned long)e[27] << 24) |
				((unsigned long)e[26] << 16) |
				((unsigned long)e[25] << 8) |
				(unsigned long)e[24];
		}
		printf("  Entry point address:               0x%lx\n", entry);
	}
	else
	{
		unsigned int entry32;

		if (e[EI_DATA] == ELFDATA2MSB)
		{
			entry32 = ((unsigned int)e[24] << 24) |
				((unsigned int)e[25] << 16) |
				((unsigned int)e[26] << 8) |
				(unsigned int)e[27];
		}
		else
		{
			entry32 = ((unsigned int)e[27] << 24) |
				((unsigned int)e[26] << 16) |
				((unsigned int)e[25] << 8) |
				(unsigned int)e[24];
		}
		printf("  Entry point address:               0x%x\n", entry32);
	}

	close(fd);
	return (0);
}

