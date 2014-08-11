#include <curses.h>
#include <stdlib.h>
#define CELLSIZE 4 

int x,y; // start position
int BOX[4][4]={  //the number displayed
    {0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0}};

void initial() // init the curses.h
 {
     initscr();
	 start_color();
	 init_pair(1, COLOR_RED, COLOR_CYAN);
     cbreak();
     nonl();
     noecho();
     intrflush(stdscr,FALSE);
     keypad(stdscr,TRUE);
	 curs_set(FALSE);
     refresh();
 }

void drawGameBox()
{
	x=(LINES-CELLSIZE*4)/2;
	y=(COLS-CELLSIZE*8)/2;
	int i,j;
	for(i=0;i<5;i++)
	{
		for(j=0;j<CELLSIZE*4;j++)
		{
			if(j%CELLSIZE==0)continue;
			move(x+i*CELLSIZE,y+j*2);
			printw("o");
			move(x+j,y+i*CELLSIZE*2);
			printw("o");
		}
	}
}

int printBoxNumber()
{
	int i,j;
	int off=(CELLSIZE + 1)/2;
	attron(COLOR_PAIR(1));
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
		{
			if(BOX[i][j]==0)continue;
			move(x+i*CELLSIZE+off,y+j*2*CELLSIZE+off);
			printw("%d",BOX[i][j]);
		}
	attroff(COLOR_PAIR(1));
}

int _random()
{
	srand((unsigned)time(NULL));
	return rand()%4;
}
int alloc_number() //input number in the empty cell
{
	int i,j,_index=0;
	int rand_arr[16];
	for(i=0;i<16;i++)
		rand_arr[i]=0;

	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
		{
			if(BOX[i][j]==0)
				rand_arr[_index++]=(0xff & i <<4) | (0xf & j);
		}
	if (_index == 0) return -1;

	int n=_random()% _index;
	i= 0xf & rand_arr[n] >> 4;
	j= 0xf & rand_arr[n];

	n=_random();
	if(n%2)
		BOX[i][j]=2;
	else
		BOX[i][j]=4;
}

/* 
 *  0:running
 *  1:other key is pressed.
 *  function:merge the same cell
 */
int handle_keydown()
{
	int key=getch();
	int i,j,k;
	switch(key)
	{
		case 259: //up
		case 119:
			for(j=0;j<4;j++)
				for(i=0;i<4;i++)
				{
					if(BOX[i][j]==0)
					{
						for(k=1;i+k<=3;k++)
							if(BOX[i+k][j]!=0)
							{
								BOX[i][j]=BOX[i+k][j];
								BOX[i+k][j]=0;
								i--;
								break;
							}
					}
					else
					{
						for(k=1;i+k<=3;k++)
							if(BOX[i+k][j]!=0)
							{
								if(BOX[i][j]==BOX[i+k][j])
								{
									BOX[i][j]*=2;
									BOX[i+k][j]=0;
								}
								break;
							}
					}
				}
			break;
		case 258: //down
		case 115:
			for(j=0;j<4;j++)
				for(i=3;i>=0;i--)
				{
					if(BOX[i][j]==0)
					{
						for(k=1;i-k>=0;k++)
							if(BOX[i-k][j]!=0)
							{
								BOX[i][j]=BOX[i-k][j];
								BOX[i-k][j]=0;
								i++;
								break;
							}
					}
					else
					{
						for(k=1;i-k>=0;k++)
							if(BOX[i-k][j]!=0)
							{
								if(BOX[i][j]==BOX[i-k][j])
								{
									BOX[i][j]*=2;
									BOX[i-k][j]=0;
								}
								break;
							}
					}
				}
			break;
		case 260: //left
		case 97:
			for(i=0;i<4;i++)
				for(j=0;j<4;j++)
				{
					if(BOX[i][j]==0)
					{
						for(k=1;j+k<=3;k++)
							if(BOX[i][j+k]!=0)
							{
								BOX[i][j]=BOX[i][j+k];
								BOX[i][j+k]=0;
								j--;
								break;
							}
					}
					else
					{
						for(k=1;j+k<=3;k++)
							if(BOX[i][j+k]!=0)
							{
								if(BOX[i][j]==BOX[i][j+k])
								{
									BOX[i][j]*=2;
									BOX[i][j+k]=0;
								}
								break;
							}
					}
				}
			break;
		case 261: //right
		case 100:
			for(i=0;i<4;i++)
				for(j=3;j>=0;j--)
				{
					if(BOX[i][j]==0)
					{
						for(k=1;j-k>=0;k++)
							if(BOX[i][j-k]!=0)
							{
								BOX[i][j]=BOX[i][j-k];
								BOX[i][j-k]=0;
								j++;
								break;
							}
					}
					else
					{
						for(k=1;j-k>=0;k++)
							if(BOX[i][j-k]!=0)
							{
								if(BOX[i][j]==BOX[i][j-k])
								{
									BOX[i][j]*=2;
									BOX[i][j-k]=0;
								}
								break;
							}
					}
				}
			break;
		default:
			return 1;
	}
	return 0;
}

int isGameOver()
{
	int i,j;
	int flag=1;
	for(i=0;i<4;i++)
		for(j=0;j<4;i++)
			if(BOX[i][j]==0)
			{
				flag=0;
				break;
			}
	return flag;
}
int main()
{
	initial();
	do
	{
		clear();
		alloc_number();
		drawGameBox();
		printBoxNumber();
		refresh();
		while(handle_keydown()==1);
		if(isGameOver())  //game over
		{
			move(x-1,y+CELLSIZE);
			printw("GAME OVER");
			getch();
			break;
		}
	}while(1);
	endwin();
}