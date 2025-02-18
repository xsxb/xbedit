#include "signatures.h"

void get_signature(FILE *fptr)
{
	char *format;
	format = "Unrecognized binary";
	unsigned char s_buffer[8];
	int recognized = 0;

	fgets(s_buffer, 8, fptr);
	//rewind(fptr);

	// Is DOS
	if (!recognized)
	{
		for (int c = 0; c < 2; c++)
		{
			if (s_buffer[c] == SIG_DOS[c])
			{
				if (c == 1) { format = "DOS"; recognized = 1;}
			}
		}
	}

	// Is ELF
	if (!recognized)
	{
		for (int c = 0; c < 4; c++)
		{
			if (s_buffer[c] == SIG_ELF[c])
			{
				if (c == 3) { format = "ELF"; recognized = 1; }
			}
		}
	}

	printf("Format: %s\n", format);
}

