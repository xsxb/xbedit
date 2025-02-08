#include <stdio.h>
#include <stdlib.h>

FILE *fptr;
unsigned char buffer[16];
char dec_ascii[16];
int lines = 0;

void print_ascii()
{
    printf("\t| "); 
    for (int i = 0; i < sizeof buffer; i++)
    {
	(buffer[i] > 32) ? printf("%c", buffer[i]) : printf(".");
    }
    printf("\t|"); 
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
	printf("%d\t: ", lines);
	for (int i = 0; i < sizeof buffer; i++)
	{
	    if (buffer[i] < 16) { printf("0"); }
	    printf("%X ", buffer[i]);
	}
	print_ascii();
	printf("\n");
	lines++;
    }
    
    fclose(fptr);
    return 0;
}
