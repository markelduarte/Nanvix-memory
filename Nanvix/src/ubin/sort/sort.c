/*
 * Copyright(C) 2021 
 * 
 * Danilo Bizarria 
 * Markel Macedo
 * Matheus Ferreira
 * Rafael Lino
 * 
 * 
 * Nanvix is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Nanvix is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Nanvix. If not, see <http://www.gnu.org/licenses/>.
 */
 
#include <sys/stat.h>
#include <sys/types.h>

#include <assert.h>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Software versioning. */
#define VERSION_MAJOR 1 /* Major version. */
#define VERSION_MINOR 0 /* Minor version. */

/* Program arguments. */
static char *const *filenames; /* Files to sort.           */
static int nfiles = 0;         /* Number of files to sort. */

/* Bubble sort starts here*/

static void swap(char* *xp, char* *yp)
{
    char* temp = *xp;
    *xp = *yp;
    *yp = temp;
}
  
// A function to implement bubble sort
static void bubble_sort(char* lines[], int nlines)
{
//puts("bubble sorting...");
	int i, j;
	for (i = 0; i < nlines - 1; i++)
	{
		// Last i elements are already in place   
		for (j = 0; j < nlines - i - 1; j++)
		{ 
			if (strcmp(lines[j], lines[j + 1]) > 0)
              			swap(lines + j, lines + j + 1);
		}
	}
}

/*Bubble sort ends here*/

/* for test purposes */
//static void print_lines(char* lines[], int nlines)
//{
//printf("printing %d lines...\n", nlines);
//assert(lines != NULL);
//	for (int i = 0; i < nlines; i++)
//	{
//		printf("line %d: %s\n", i + 1, lines[i]);
//	}
//}

static int split_lines(char* *output[], char* input, int n){
//puts("spliting lines...");
//printf("input:\n%s\n", input);
	char* *lines;
	int nlines = 0;
	for (int i = 0; i < n; i++){
		if(input[i] == '\n')
			nlines++;
	}
//printf("detected %d lines\n", nlines);
	*output = lines = malloc(sizeof(char*)*nlines);
	assert(lines != NULL);
	int j = 0;
	int line = 0;
	for(int i = 0; i < n; i++)
	{
		/* here ends a line */
		if(input[i] == '\n')
		{
//printf("copying line %d\n", line + 1);
assert(line < nlines);
			/* ignores '\n' */
			int count = i - j;

			/* +1 for terminating '\0'  */
			lines[line] = malloc(sizeof(char)*(count + 1));
			assert(lines[line] != NULL);

			strncpy(lines[line], input + j, count);
			/* as strncpy() does not null-terminate strings */
                        strcpy(lines[line] + count, "");
//printf("line %d: %s", line + 1, lines[line]);	
			
			line++;
			j = i + 1;
		}
	}

//print_lines(lines, nlines);
	return nlines;
}

static int join_lines(char *buffer,  char* lines[], int nlines){
//puts("joining lines...");
	int len = 0;
//	buf[0] = '\0'; // for strcat()
	for (int i = 0; i < nlines; i++)
	{
		/* 
                 * strcat() would be less efficient, 
                 * as it always reads entire destination string
 		 * sprintf() is not available in nanvix 
                 */
		ssize_t line_len = strlen(lines[i]);
		strcpy(buffer + len, lines[i]);
		len += line_len;
		strcpy(buffer + len, "\n");
		len += 1;
	}

//puts(output);
	return len;
}

/*
 * sort a file.
 */
static void sort(char *filename)
{
	int fd;           /* File descriptor.        */
	int off;          /* Buffer offset.          */
	char buf[BUFSIZ]; /* Buffer.                 */
	ssize_t n, nread; /* Bytes actually read.    */
	ssize_t nwritten; /* Bytes actually written. */
	
	fd = open(filename, O_RDONLY);
	
	/* Failed to open file. */
	if (fd < 0)
	{
		fprintf(stderr, "sort: cannot open %s\n", filename);
		return;
	}
	
	/* sort file. */
	do
	{	
		/* Error while reading. */
		if ((nread = read(fd, buf, BUFSIZ)) < 0)
			fprintf(stderr, "sort: cannot read %s\n", filename);	
//printf("\nBUFSIZ: %d\tnread: %ld\n", BUFSIZ, nread);
		n = nread;
		char* *lines;
		int nlines = split_lines(&lines, buf, n);
//puts("input lines:");
//print_lines(lines, nlines);
		
		/*Bubble Sort*/
		bubble_sort(lines, nlines);
//puts("ordered lines:");
//print_lines(lines, nlines);

		join_lines(buf, lines, nlines);
//puts("joined lines:");
//puts(buf);

		// can we sort the buffer directly?
		//bubbleSort(buf, n);
		
		off = 0;
	      	do
		{
			nwritten = write(fileno(stdout), &buf[off], nread);
			
			/* Failed to write. */
			if (nwritten < 0)
			{
				fprintf(stderr, "sort: write error\n");
				exit(errno);
			}
			off += nwritten;
		} while ((nread -= nwritten) > 0);
	} while (n == BUFSIZ);	
	
	close(fd);
}

/*
 * Prints program version and exits.
 */
static void version(void)
{
	printf("sort %d.%d\n\n", VERSION_MAJOR, VERSION_MINOR);
//	printf("Copyright(C) 2011-2014 Pedro H. Penna\n");
	printf("This is free software under the "); 
	printf("GNU General Public License Version 3.\n");
	printf("There is NO WARRANTY, to the extent permitted by law.\n\n");
	
	exit(EXIT_SUCCESS);
}

/*
 * Prints program usage and exits.
 */
static void usage(void)
{
	printf("Usage: sort [options] [files]\n\n");
	printf("Brief: Sorts files.\n\n");
	printf("Options:\n");
	printf("  --help    Display this information and exit\n");
	printf("  --version Display program version and exit\n");
	
	exit(EXIT_SUCCESS);
}

/*
 * Gets program arguments.
 */
static void getargs(int argc, char *const argv[])
{
	int i;     /* Loop index.       */
	char *arg; /* Working argument. */
	
	/* Get program arguments. */
	for (i = 1; i < argc; i++)
	{
		arg = argv[i];
		
		/* Display help information. */
		if (!strcmp(arg, "--help"))
			usage();
		
		/* Display program version. */
		else if (!strcmp(arg, "--version"))
			version();
		
		/* Get file names. */
		else
		{
			if (nfiles++ == 0)
				filenames = &argv[i];
		}
	}
}

/*
 * Sorts files. 
 */
int main(int argc, char *const argv[])
{	
	int i;          /* Loop index.               */
	char *filename; /* Name of the working file. */
	struct stat st; /* Working file's status.    */
	
	getargs(argc, argv);
	
	/* Sort standard input. */
	if (nfiles == 0)
		sort("/dev/tty");
	
	/* Sort files. */
	else
	{
		for (i = 0; i < nfiles; i++)
		{
			filename = filenames[i];
			
			/* Sort standard input. */
			if (!strcmp(filename, "-"))
				filename = "/dev/tty";
			
			/* Check if file is not a directory. */
			else
			{
				if (stat(filename, &st) == -1)
				{
					fprintf(stderr, "sort: cannot stat %s\n", filename);
					continue;
				}
				
				/* File is directory. */
				if (S_ISDIR(st.st_mode))
				{
					fprintf(stderr, "sort: %s is a directory\n", filename);
					continue;
				}
			}
				
			sort(filename);
		}
	}
	
	return(EXIT_SUCCESS);
}
