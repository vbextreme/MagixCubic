#include "main.h"

VOID presentat(CHAR* app, UINT32* py, UINT32* px)
{
	CHAR* e = strlen(app) + app;
	while ( e > app && *e !='/') --e;
	++e;
	
	UINT32 x,y,sx,sy;
	UINT32 w,h;
	
	con_getmaxrc(&h,&w);
	con_getrc(&sy,&sx);
	
	y = sy - 1;
	for (; y > 0; --y)
	{
		for (x = w; x > 0; --x)
		{
			con_gotorc(y,x);
			putchar(' ');
			con_flush();
			thr_sleep(TIME_AT1);
		}
	}
	
	INT32 lt = strlen(TITLEAPP);
	
	for (x = 1; x < w / 2 - lt / 2; ++x)
	{
		con_gotorc(y,x);
		con_flush();
		thr_sleep(TIME_AT2);
	}
	
	for (y = 1; y < h / 2 ; ++y)
	{
		con_gotorc(y,x);
		con_flush();
		thr_sleep(TIME_AT2);
	}
	
	printf("%s",TITLEAPP);
	con_flush();
	thr_sleep(TIME_AT3);
	for ( --y; y > h / 4 ; --y)
	{
		con_clsline(CON_CLLS_ALL);
		con_gotorc(y,x);
		printf("%s",TITLEAPP);
		con_flush();
		thr_sleep(TIME_AT4);
	}
	y+=2;
	con_gotorc(y,x);
	printf("%s",DIVAPP);
	
	*px = x;
	*py = y;
}

VOID gameoverat(MXC* g)
{
	INT32 x,y;
	for ( y = 0; y < MAPH; ++y)
	{
		for ( x = 1; x < MAPW; ++x)
		{
			g->map[y][x] = EMPTYL;
			con_gotorc(y + g->sty + 1, x + g->stx + 2);
			con_setcolor256(g->lcom[EMPTYL],g->fcol);
			putchar(g->lchm[EMPTYL]);
			con_flush();
			thr_sleep(TIME_ATG);
		}
	}
	
	con_gotorc(g->sty + 1 + MAPH / 2,g->stx + 3 + MAPW / 2 - strlen(OVERAPP)/2);
	setcolor(0,0);
	printf("%s",OVERAPP);
	con_flush();
	
}
