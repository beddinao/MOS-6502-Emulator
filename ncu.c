#include <curses.h>

int	main()
{
	initscr();
	mvaddstr(LINES / 2, COLS / 2, "hello there");
	refresh();

	getch();

	endwin();
}


