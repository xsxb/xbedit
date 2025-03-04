#include "signatures.h"
#include "elf.h"

// arg1 = buffer from file read; arg2 = pointer to signature; arg3 = length of signature
// TODO: Error handling
int sigcmp(unsigned char *sig_buffer, const int signature[], int sig_size)
{
	int is_equal = 0;

	//if (sizeof signature > sig_size) {}

	for (int i = 0; i < sig_size; i++)
	{
		//printf("Comparing: %02X | %02X\n", sig_buffer[i], signature[i]);
		if (sig_buffer[i] == signature[i])
		{
			if (i == sig_size -1)
			{
				is_equal = 1;
			}
		}
	}

	return is_equal;
}

void get_signature(FILE *fptr)
{
	const char *format;
	unsigned char s_buffer[8];

	format = "Unrecognized binary";

	fgets(s_buffer, 8, fptr);
	
	//printf("Size of pointer array: %d\n", sizeof SIGNATURE_P_ARRAY);
	for (int s = 0; s < (sizeof SIG_P_ARR / 8); s++)
	{
		int n = sigcmp(s_buffer, SIG_P_ARR[s], (sizeof SIG_P_ARR[s] / 4));
		
		if (n == 1) { format = SIG_P_STR[s]; }
	}

	printf("Format: %s\n", format);
}

//TODO: from file buffer:
void print_elf_header(FILE *fptr)
{
   for (int i = 0; i < 2; i++)
   {
       int field_value;
       printf("Field offset: %08X\n", ELF_FIELDS[i] & 0x00FFFFFF);
       printf("Field size: %d Byte\n", ELF_FIELDS[i] >> 24);
       printf("Field value: %d\n", field_value);
       printf("=============================\n");
   }
}
