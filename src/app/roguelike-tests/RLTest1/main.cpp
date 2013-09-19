#include <curses.h>
#include <algorithm>

enum class eTile : char 
{
	EMPTY = 46,
	CHAR = 64,
	WALL = 35
};

struct cWin
{
	WINDOW * mWin;

	cWin():mWin(nullptr){}
	~cWin() {Destroy();}

	void Destroy()
	{
		if(mWin)
		{
			wborder(mWin, ' ', ' ', ' ',' ',' ',' ',' ',' ');
			wrefresh(mWin);
			delwin(mWin);
			mWin = nullptr;
		}
	}
	void Init(int x, int y, int w, int h)
	{
		Destroy();	
		mWin = newwin(h, w, y, x);
		box(mWin, 0 , 0);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
		wrefresh(mWin);		/* Show that box 		*/
	}
};
	
int main()
{	int ch;
	initscr();			/* Start curses mode 		*/
	raw();				/* Line buffering disabled	*/
	keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
	noecho();			/* Don't echo() while we do getch */

	int maxx,maxy;
	getmaxyx(stdscr,maxy,maxx);
	int posx=maxx/2,posy=maxy/2;
	for(int i=0;i<maxy;++i)
		for(int j=0;j<maxx;++j)
			mvaddch(i,j,chtype(eTile::EMPTY));

	mvaddch(posy,posx,chtype(eTile::CHAR));
	move(posy,posx);

	cWin win_map;
	win_map.Init(3,4,5,6);

	ch = getch();
	int prevposx=posx,prevposy=posy;
	while( ch != 27 )
	{
		switch(ch)
		{
			case '1': posx = std::max(posx-1,0); posy=std::min(posy+1,maxy-1); break;
			case '2': posy=std::min(posy+1,maxy-1); break;
			case '3': posx = std::min(posx+1,maxx-1); posy=std::min(posy+1,maxy-1); break;
			case '7': posx = std::max(posx-1,0); posy=std::max(posy-1,0); break;
			case '8': posy=std::max(posy-1,0); break;
			case '9': posx = std::min(posx+1,maxx); posy=std::max(posy-1,0); break;
			case '4': posx = std::max(posx-1,0); break;
			case '5': break;
			case '6': posx = std::min(posx+1,maxx-1); break;
		}
		mvaddch(prevposy,prevposx,chtype(eTile::EMPTY));
		mvaddch(posy,posx,chtype(eTile::CHAR));
		
		move(posy,posx);
		prevposx=posx;
		prevposy=posy;
		refresh();
		ch = getch();
	}
	endwin();			/* End curses mode		  */
	return 0;
}
