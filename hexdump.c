// ISSUES:
// fgets function stops parsing on newline character
// tui_hexdump() discards first line

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

FILE *fptr;
char *fpath;
long fsize;
char *file_buffer;

size_t return_code;
unsigned char buffer[16];
char dec_ascii[16];
int lines = 0;

int cpos[2];
int max_x;
int max_y;
int input;

WINDOW *addr_win;
WINDOW *hex_win;
WINDOW *ascii_win;

int tui_size_y = 20;
int tui_size_x = 20;
int hex_win_size_x = 51;

void print_usage()
{
	printf("Usage: \n");
}

void init_tui()
{
    initscr();
    noecho();
    start_color();

    getmaxyx(stdscr, max_y, max_x);
    tui_size_y = max_y;
    tui_size_x = max_x;

    addr_win = newwin(tui_size_y, 12, 1, 0);
    hex_win = newwin(tui_size_y, hex_win_size_x, 1, 12);
    ascii_win = newwin(tui_size_y, 20, 1, 63);
    refresh();

    box(addr_win, 0, 0);
    box(hex_win, 0, 0);
    box(ascii_win, 0, 0);

    mvwprintw(addr_win, 0, 1, "ADDR");
    mvwprintw(hex_win, 0, 1, "VALUES");
    mvwprintw(ascii_win, 0, 1, "ASCII");

    move(1, 14);

    wrefresh(addr_win);
    wrefresh(hex_win);
    wrefresh(ascii_win);

}

void tui_quit()
{
	endwin();
	exit(0);
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

// Classic hexdump to CLI without ncurses
void hexdump()
{
	// Get file size:
	fseek(fptr , 0 , SEEK_END);
	fsize = ftell(fptr);
	rewind (fptr);

	// Allocate memory for file buffer
	file_buffer = (char *) malloc (sizeof(char)*fsize);
	printf("Size of file: %d Bytes\n", fsize);
	printf("Number of full lines: %d\n", (fsize/16));
	printf("Last line size: %d Bytes\n", (fsize % 16));

	// Load file into buffer
	return_code = fread(file_buffer, 1, fsize, fptr);

	printf("00000000  ");
	for (int c = 0; c < fsize; c++)
	{
		char *pos = (char*) file_buffer + c;
		buffer[c%16] = *pos;
		printf("%02X ", *pos);
		if (c % 16 == 0 && c != 0) { printf("\n%08X  ", c); }
	}
	printf("\n");

	/*
	while (fread(buffer, 16, 16, fptr))
	{
		return_code = fread(buffer, 16, 16, fptr);
		for (int i = 0; i < sizeof buffer; i++)
		{ printf("%02X ", buffer[i]); }
		printf("\n");
	}
	printf("Return code: %d\n", return_code);
	*/	

	/*
    while (fgets(buffer, sizeof buffer, fptr) != NULL)
    {
	printf("%08X  ", lines);
	for (int i = 0; i < sizeof buffer; i++)
	{
	    printf("%02X ", buffer[i]);
	}
	print_ascii();
	printf("\n");
	lines++;
    }
	*/
}

// Hexdump in textual user interface with ncurses
void tui_hexdump()
{
    for (int n = 0; n < tui_size_y - 2; n++)
    {
	//Read from file into buffer
	if (fgets(buffer, sizeof buffer, fptr) != NULL)
	{
		fgets(buffer, sizeof buffer, fptr);
	} else { break; }

	//Print memory offset
	mvwprintw(addr_win, lines+1, 2, "%08X", lines);

	//Print hex values from buffer
	int xpos = 0;
	for (int i = 0; i < sizeof buffer; i++)
	{
	    if (xpos % 3 == 0)
	    {
		mvwprintw(hex_win, n+1, xpos+1, " ");
		xpos++;
	    }
	    mvwprintw(hex_win, n+1, xpos+1, "%02X", buffer[i]);
	xpos += 2;
	}

	//Print decoded as ASCII
	for (int i = 0; i < sizeof buffer; i++)
	{
	    if (buffer[i] > 32 && buffer[i] < 127)
	    {
		mvwaddch(ascii_win, lines+1, i+2, buffer[i]);
	    } else
	    {
		mvwaddch(ascii_win, lines+1, i+2, '.');
	    }
	}

	lines++;
    }
}

int tui_run()
{
    //Textual User Interface mode
    init_tui();
    getyx(hex_win, cpos[0],cpos[1]);

    tui_hexdump();

    wrefresh(addr_win);
    wrefresh(hex_win);
    wrefresh(ascii_win);

    //Handle navigation input
    while (1)
    {
	input = getch();
	switch(input)
	{
	//Menu
	    case 113:  tui_quit();	//q 
	//Vertical movement
	    case 106:  cpos[0]++; break;  //j
	    case 107:  cpos[0]--; break;  //k
	//Horizontal movement in hex_win
	    case 104:  if ((cpos[1]+1) % 3 == 0) { cpos[1]--;}	//skip space
		       cpos[1]--;    	 //h
		       break;
	    case 108:  if ((cpos[1]) % 3 == 0) { cpos[1]++;}	//skip space
		       cpos[1]++;    	 //l
		       break;
	}
	//Confine cursor to hex_win
	if (cpos[0] >= tui_size_y-1) { cpos[0]--; }
	else if (cpos[0] < 1) { cpos[0]++; }
	else if (cpos[1] >= hex_win_size_x-2) { cpos[1]-=2; }
	else if (cpos[1] <= 1) { cpos[1]+=2; }
	wmove(hex_win, cpos[0], cpos[1]);
	wrefresh(hex_win);
	//Highlight corresponding ASCII
	//mvwchgat(ascii_win, cpos[0], cpos[1]/2, 1, A_STANDOUT, 0, NULL);
    }

    endwin();
	return 0;
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
				printf("Usage: ");
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

	if (t_flag != 1) { hexdump(); }
	else { tui_run(); }

	fclose(fptr);
    return 0;
}
