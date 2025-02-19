#ifndef SHARED_H
#define SHARED_H

#include <stdio.h>
#include <stdlib.h>

long fsize;

// Returns file buffer 
char * load_file(FILE *fptr)
{
	char *file_buffer;
	size_t return_code;

	// Get file size:
	fseek(fptr , 0 , SEEK_END);
	fsize = ftell(fptr);
	rewind (fptr);

	// Allocate memory for file buffer
	file_buffer = (char *) malloc (sizeof(char)*fsize);

	// Load file into buffer
	return_code = fread(file_buffer, 1, fsize, fptr);

	//TODO: Error handling
	

	return file_buffer;
}

#endif
