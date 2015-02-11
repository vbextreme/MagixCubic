#include "main.h"

int main(int argc,char* argv[])
{	
	UINT32 smx,smy;
	presentat(argv[0],&smy,&smx);
	
	MXC g;
	initmxc(&g);
	
	BYTE cmd = 0;
	
	while( cmd != 1 )
	{
		con_cls();
		if ( !menumain(&g,smy,smx) ) break;
		
		do{resetgame(&g);}while(isgameover(&g));
		draw(&g,TRUE);
		
		while(1)
		{
			while(!con_kbhit()) thr_sleep(TIME_I);
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
			while ( cmd != CON_KEY_CTRL ){	while(!con_kbhit()) thr_sleep(TIME_I); cmd = con_getchex();}
			
			cmd = 0;
			con_cls();
		}
	}
	
	con_async(0,NULL);
	setcolor(0,0);
	con_cls();
	
    return 0;   
}
