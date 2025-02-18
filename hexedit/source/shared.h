#ifndef SHARED_H
#define SHARED_H

#include <stdio.h>
#include <stdlib.h>


int load_file(FILE *fptr)
{
	long fsize;
	char *file_buffer;
	size_t return_code;

	// Get file size:
	fseek(fptr , 0 , SEEK_END);
	fsize = ftell(fptr);
	rewind (fptr);

	// Allocate memory for file buffer
	file_buffer = (char *) malloc (sizeof(char)*fsize);

	/*
	printf("Size of file: %d Bytes\n", fsize);
	printf("Number of full lines: %d\n", (fsize/16));
	printf("Last line size: %d Bytes\n", (fsize % 16));
	*/

	// Load file into buffer
	return_code = fread(file_buffer, 1, fsize, fptr);

	return return_code;
}

#endif
