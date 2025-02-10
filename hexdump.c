#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

FILE *fptr;
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
int hex_win_size_x = 51;

void quit()
{
    	endwin();
	exit(0);
}

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
}

// Classic hexdump with ncurses
void tui_hexdump()
{
    //mvprintw(hex_
    for (int n = 0; n < tui_size_y - 2; n++)
    {
	//Read from file into buffer
	fgets(buffer, sizeof buffer, fptr);

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


int main(int argc, char *argv[])
{

    if (argc < 2)
    {
	printf("No arguments were passed.\n");
	exit(1);
    }

    fptr = fopen(argv[1], "r");

    //Init ncurses TUI
    initscr();
    noecho();
    start_color();

    getmaxyx(stdscr, max_y, max_x);
    tui_size_y = max_y;

    addr_win = newwin(tui_size_y,12,0,0);
    hex_win = newwin(tui_size_y,hex_win_size_x,0,12);
    ascii_win = newwin(tui_size_y,20,0,63);
    refresh();

    box(addr_win, 0, 0);
    box(hex_win, 0, 0);
    box(ascii_win, 0, 0);

    mvwprintw(addr_win, 0, 1, "ADDR");
    mvwprintw(hex_win, 0, 1, "VALUES");
    mvwprintw(ascii_win, 0, 1, "ASCII");

    move(1, 14);
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
	    case 113:  quit();	//q 
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
	//Highlight corresponding ASCII
	//mvwchgat(ascii_win, cpos[0], cpos[1]/2, 1, A_STANDOUT, 0, NULL);
	wrefresh(hex_win);
	wrefresh(ascii_win);
    }

    endwin();

    fclose(fptr);
    return 0;
}
