#include <stdlib.h>

#include <ncurses.h>

#include "shared.h"

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

// Canonical hexdump in textual user interface
void tui_hexdump(FILE *fptr)
{
	load_file(fptr);



    for (int n = 0; n < tui_size_y - 2; n++)
	{
		if ( n * 16 > fsize) { break; }		//EOF

		// Print memory offset
		mvwprintw(addr_win, n+1, 2, "%08X", n * 16);

		// Print hex values
		// +2 x-offset from window and *3 for spacing 
		int file_pos;
		for (int c = 0; c < 16; c++)
		{
			file_pos = n * 16 + c;
			if (file_pos > fsize) { break; }	//EOF

			mvwprintw(hex_win, n+1, c*3+2, "%02X", (unsigned char) file_buffer[file_pos]);
			// Print as ASCII
			if (file_buffer[file_pos] > 32 && file_buffer[file_pos] < 127)
			{ mvwprintw(ascii_win, n+1, c+2, "%c", file_buffer[file_pos]); }
			else
			{ mvwprintw(ascii_win, n+1, c+2, "."); }
		}
	}
}

void tui_quit()
{
	endwin();
	exit(0);
}

int tui_run(FILE *fptr)
{
    //Textual User Interface mode
    init_tui();
    getyx(hex_win, cpos[0],cpos[1]);

    tui_hexdump(fptr);

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
