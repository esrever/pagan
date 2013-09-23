#include <curses.h>
#include <algorithm>
#include <vector>
#include <typeinfo>

enum class eTile : char 
{
	EMPTY = 46,
	CHAR = 64,
	WALL = 35
};


struct cMap
{
	std::vector<char> mMap;
	int mRows;
	int mCols;

	cMap(int rows=0, int cols=0):mRows(rows),mCols(cols)
	{ 
		int total = rows * cols;
		if(total)
		{
			mMap.resize(total+1);
			mMap.back() = '\0';
		}

		memset(&mMap.front(),46,total);
		for(int i=0;i<(total>>2);++i)
		{
			mMap.at(rand() % total) = 35;
		}
	}

	eTile Get(int xpos, int ypos) const
	{
		int tot = xpos 	+ypos*mCols;
		return eTile(mMap.at(tot));
	}
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
	cMap mymap(maxy,maxx);

	addstr(&mymap.mMap.front());
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
			case '1': posx--;posy++; break;
			case '2': posx  ;posy++; break;
			case '3': posx++;posy++; break;
			case '7': posx--;posy--; break;
			case '8': posx  ;posy--; break;
			case '9': posx++;posy--; break;
			case '4': posx--;posy  ; break;
			case '5': posx  ;posy  ; break;
			case '6': posx++;posy  ; break;
		}
		if( (posx>=0) && (posy>=0) && (posx<maxx) && (posy<maxy) && (mymap.Get(posx,posy) == eTile::EMPTY))
		{
			mvaddch(prevposy,prevposx,chtype(mymap.Get(prevposx,prevposy)));
			mvaddch(posy,posx,chtype(eTile::CHAR));
			move(posy,posx);
			prevposx=posx;
			prevposy=posy;
		}
		else
		{
			posx = prevposx;
			posy = prevposy;
		}
		refresh();
		ch = getch();
	}
	endwin();			/* End curses mode		  */
	return 0;
}
