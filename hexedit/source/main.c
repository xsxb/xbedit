// ISSUES:
// load_file() function needs rewriting
// byte order 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "signatures.c"
#include "tui.c"

unsigned char buffer[16];
char dec_ascii[16];

FILE *fptr;
char *fpath;
long fsize;
char *file_buffer;

void print_usage()
{
	printf("\nUsage:\
	\nhexdump [options] -f <file>...\
	\n\n-h \tDisplay help\
	\n-f \tFile \
	\n-t \tOpen in TUI mode\
	\n\n --- TUI Mode ---\
	\n\n\tq - quit\
	\n\nNavigation:\
	\n\th - Move cursor left\
	\n\tj - Move cursor up\
	\n\tk - Move cursor downn\
	\n\tl - Move cursor right\n");

}

// Print values decoded as ASCII
void print_ascii()
{
    printf("  | "); 
    for (int i = 0; i < sizeof buffer; i++)
    {
	(buffer[i] > 32 && buffer[i] < 127) ? printf("%c", buffer[i]) : printf(".");
    }
    printf("  |"); 
}

// Canonical hexdump to CLI 
void hexdump()
{
	load_file(fptr);

	// Output to console
	printf("00000000  ");
	for (int c = 0; c < fsize; c++)
	{
		if (c % 16 == 0 && c != 0)
		{
			print_ascii();
			printf("\n%08X  ", c);
		}
		unsigned char *byte = (char *) file_buffer + c;
		buffer[c % 16] = *byte;				//byte array for string conversion
		printf("%02X ", *byte);
	}
	printf("\n");
}

int main(int argc, char *argv[])
{
	// Option and argument handlin
	int opt;
	int t_flag;
	int f_flag;
	while ((opt = getopt(argc, argv, "f:ht::")) != -1)
	{
		switch (opt)
		{
			case 'h':
				print_usage();
				exit(0);
			case 'f':
				f_flag = 1;
				fpath = optarg;
				break;
			case 't':
				t_flag = 1;
				break;
			case '?':
				if (optopt == 'f')
				{
					fprintf(stderr, "Option -f requires a file as argument.\n");
				}
				else
				{
					fprintf(stderr, "Unknown option: -%c\n", optopt);
				}
				exit(1);
		}
	}

	if (f_flag != 1) { fprintf(stderr, "No file given in arguments\n"); exit(0); }

	fptr = fopen(fpath, "rb");
	if (fptr == NULL) { perror ("Error opening file"); exit(2); }

	if (t_flag != 1)
	{ 
		get_signature(fptr);
		hexdump();
	}
	else { tui_run(fptr); }

	fclose(fptr);
    return 0;
}
