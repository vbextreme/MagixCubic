#include "main.h"

int main(int argc,char* argv[])
{	
	con_sigsize();
	
	UINT32 smx,smy;
	presentat(argv[0],&smy,&smx);
	
	MXC g;
	initmxc(&g);
	
	INT32 cmd = 0;
	
	while( cmd != 1 )
	{
		setcolor(0,0);
		con_cls();
		if ( !menumain(&g,smy,smx) ) break;
		
		do{resetgame(&g);}while(isgameover(&g));
		draw(&g,TRUE);
		
		while(1)
		{
			while(!con_kbhit())
			{
				if ( con_haveresize() ) con_getmaxrc(&g.scrh,&g.scrw);
				thr_sleep(TIME_I);
			}
			
			cmd = con_getchex();
			cmd = gkey(&g,cmd);
			if ( cmd == 1 ) {cmd = 0;break;}
			
			if (cmd == 2 )
			{
				INT32 r = g.points += destroycubic(&g);
				if ( r )
				{
					while(pushdown(&g));
					while(pushright(&g));
					while(pushdown(&g) ||	pushright(&g));
				}
				
				while(inscubic(&g))
				{
					g.points += BONUS;
					draw(&g,FALSE);
					while(pushdown(&g));
					while(pushright(&g));
					while(pushdown(&g) ||	pushright(&g));
				}
				if ( isgameover(&g) ) {cmd = 3;break;}
			}
			
			draw(&g,FALSE);
		}
		
		if ( cmd == 3 )
		{
			gameoverat(&g);
			g.lastpoints = g.points;
			saveopz(&g);
			while ( cmd != 'z' ){	while(!con_kbhit()) thr_sleep(TIME_I); cmd = con_getchex();}
			
			cmd = 0;
			setcolor(0,0);
			con_cls();
		}
	}
	
	con_async(0);
	setcolor(0,0);
	con_cls();
	
    return 0;   
}
