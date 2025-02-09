#include <stdio.h>
#include <stdlib.h>

FILE *fptr;
unsigned char buffer[16];
char dec_ascii[16];
int lines = 0;

void print_ascii()
{
    printf("  | "); 
    for (int i = 0; i < sizeof buffer; i++)
    {
	(buffer[i] > 32 && buffer[i] < 127) ? printf("%c", buffer[i]) : printf(".");
    }
    printf("  |"); 
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
	printf("No arguments were passed.\n");
	exit(1);
    }

    fptr = fopen(argv[1], "r");

    while (fgets(buffer, sizeof buffer, fptr) != NULL)
    {
	printf("%08X  : ", lines);
	for (int i = 0; i < sizeof buffer; i++)
	{
	    printf("%02X ", buffer[i]);
	}
	print_ascii();
	printf("\n");
	lines++;
    }
    
    fclose(fptr);
    return 0;
}
