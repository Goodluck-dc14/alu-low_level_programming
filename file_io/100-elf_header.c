#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>

/**
 * print_magic - prints ELF magic bytes
 * @e: pointer to ELF header
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
 * print_class_data - prints class and data encoding
 * @e: pointer to ELF header
 */
void print_class_data(unsigned char *e)
{
	printf("  Class:                             ");
	if (e[EI_CLASS] == ELFCLASS32)
		printf("ELF32\n");
	else if (e[EI_CLASS] == ELFCLASS64)
		printf("ELF64\n");
	else
		printf("<unknown: %x>\n", e[EI_CLASS]);

	printf("  Data:                              ");
	if (e[EI_DATA] == ELFDATA2LSB)
		printf("2's complement, little endian\n");
	else if (e[EI_DATA] == ELFDATA2MSB)
		printf("2's complement, big endian\n");
	else
		printf("<unknown: %x>\n", e[EI_DATA]);
}

/**
 * print_version_osabi - prints version, OS/ABI and ABI version
 * @e: pointer to ELF header
 */
void print_version_osabi(unsigned char *e)
{
	char *os[] = {
		"UNIX - System V", "UNIX - HP-UX",
		"UNIX - NetBSD", "UNIX - GNU",
		"", "", "UNIX - Solaris", "UNIX - AIX",
		"UNIX - IRIX", "UNIX - FreeBSD",
		"UNIX - Tru64", "Novell - Modesto",
		"UNIX - OpenBSD"
	};

	printf("  Version:                           %d", e[EI_VERSION]);
	if (e[EI_VERSION] == EV_CURRENT)
		printf(" (current)\n");
	else
		printf("\n");

	printf("  OS/ABI:                            ");
	if (e[EI_OSABI] < 13)
		printf("%s\n", os[(int)e[EI_OSABI]]);
	else
		printf("<unknown: %x>\n", e[EI_OSABI]);

	printf("  ABI Version:                       %d\n", e[EI_ABIVERSION]);
}

/**
 * get_type - gets the ELF type value
 * @e: pointer to ELF header
 *
 * Return: type value
 */
unsigned int get_type(unsigned char *e)
{
	if (e[EI_DATA] == ELFDATA2MSB)
		return ((e[16] << 8) | e[17]);
	return (e[16] | (e[17] << 8));
}

/**
 * print_type - prints ELF type
 * @e: pointer to ELF header
 */
void print_type(unsigned char *e)
{
	unsigned int type;

	type = get_type(e);
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
}

/**
 * print_entry64 - prints 64-bit entry point
 * @e: pointer to ELF header
 */
void print_entry64(unsigned char *e)
{
	unsigned long entry;
	int i;

	entry = 0;
	if (e[EI_DATA] == ELFDATA2MSB)
	{
		for (i = 24; i < 32; i++)
			entry = (entry << 8) | e[i];
	}
	else
	{
		for (i = 31; i >= 24; i--)
			entry = (entry << 8) | e[i];
	}
	printf("  Entry point address:               0x%lx\n", entry);
}

/**
 * print_entry32 - prints 32-bit entry point
 * @e: pointer to ELF header
 */
void print_entry32(unsigned char *e)
{
	unsigned int entry;
	int i;

	entry = 0;
	if (e[EI_DATA] == ELFDATA2MSB)
	{
		for (i = 24; i < 28; i++)
			entry = (entry << 8) | e[i];
	}
	else
	{
		for (i = 27; i >= 24; i--)
			entry = (entry << 8) | e[i];
	}
	printf("  Entry point address:               0x%x\n", entry);
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
	if (rd < 16 || e[0] != 0x7f ||
	    e[1] != 'E' || e[2] != 'L' || e[3] != 'F')
	{
		dprintf(STDERR_FILENO,
			"Error: Not an ELF file - %s\n", argv[1]);
		close(fd);
		exit(98);
	}
	printf("ELF Header:\n");
	print_magic(e);
	print_class_data(e);
	print_version_osabi(e);
	print_type(e);
	if (e[EI_CLASS] == ELFCLASS64)
		print_entry64(e);
	else
		print_entry32(e);
	close(fd);
	return (0);
}

