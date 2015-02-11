#include "main.h"

VOID menuoptdraw(MXC* g,UINT32 py, UINT32 px, INT32 carretid)
{
	static CHAR* mu[] = {"< Difficulty >","< Thema >","< Back >"};
	static CHAR* mod[] = {"Stupid","Easy","Normal","Hard","Insane"};
	
	UINT32 y = py + 1;
	UINT32 l = strlen(TITLEAPP)/2;
	
	con_gotorc(py-1,px);
	printf("%s",TITLEAPP);
	con_gotorc(py,px);
	printf("%s",DIVAPP);
	
	INT32 i;
	for ( i = 0; i < 3; ++i, ++y)
	{
		setcolor(0,0);
		con_gotorc(y,px +  l - strlen(mu[i]) / 2);
		con_clsline(CON_CLLS_ALL);
		
		if ( carretid == i )
			setcolor(16,0);
		
		printf("%s",mu[i]);
		if ( i == 0 )
		{
			printf("%s",mod[g->lmm-3]);
		}
		else if ( i == 1 )
		{
			INT32 k;
			for ( k = 0; k < g->lmm; ++k)
			{
				setcolor(g->lcom[k],g->fcol);
				putchar(g->lchm[k]);
			}
		}
	}
	
	setcolor(0,0);
	con_gotorc(py + carretid +1,px +  l + strlen(mu[carretid]) / 2);
	con_flush();
}

INT32 menuopt(MXC* g, UINT32 py, UINT32 px)
{
	INT32 cid = 0;
	BYTE c;
	
	CHAR ph[1024];
	sprintf(ph,"%s/.config/magixcubic",pht_homedir());
	
	CHAR d[512];
	FILETYPE ft;
	
	while(1)
	{
		menuoptdraw(g,py,px,cid);
	
		while(!con_kbhit()) thr_sleep(TIME_I);
		c = con_getchex();
		
		switch(c)
	    {
			case CON_KEY_UP: if ( cid > 0 ) --cid;	break;
			case CON_KEY_DOWN: if ( cid < 2 ) ++cid; break;
		
			case CON_KEY_CTRL:
				switch(cid)
				{
					case 0:	++g->lmm; if ( g->lmm == 8 ) g->lmm = 3; break;;
					
					case 1: 
						ft = dir_list(d,TRUE,FT_REG,NULL);
						if ( ft == -1 )
						{
							CHAR ph[1024];
							sprintf(ph,"%s/.config/magixcubic",pht_homedir());
							ft = dir_list(d,TRUE,FT_REG,ph);
						}
						loadthema(g,d);
					break;
					
					case 2: return 0;
				}
			break;
			
			case 27:
				return 0;
				
			default: break;
		}
	}
	return 0;
}

VOID menumaindraw(MXC* g,UINT32 py, UINT32 px, INT32 carretid)
{
	static CHAR* mu[] = {"< Start >","< Option >","< Exit >"};
	
	UINT32 y = py + 1;
	UINT32 l = strlen(TITLEAPP)/2;
	
	con_gotorc(py-1,px);
	printf("%s",TITLEAPP);
	con_gotorc(py,px);
	printf("%s",DIVAPP);
	
	INT32 i;
	for ( i = 0; i < 3; ++i, ++y)
	{
		setcolor(0,0);
		con_gotorc(y,px +  l - strlen(mu[i]) / 2);
		con_clsline(CON_CLLS_ALL);
		
		if ( carretid == i )
			setcolor(16,0);
		
		printf("%s",mu[i]);
	}
	
	setcolor(0,0);
	con_gotorc(py + carretid +1,px +  l + strlen(mu[carretid]) / 2);
	con_flush();
}

INT32 menumain(MXC* g, UINT32 py, UINT32 px)
{
	INT32 cid = 0;
	BYTE c;
	
	while(1)
	{
		menumaindraw(g,py,px,cid);
	
		while(!con_kbhit()) thr_sleep(TIME_I);
		c = con_getchex();
		
		switch(c)
	    {
			case CON_KEY_UP: if ( cid > 0 ) --cid;	break;
			case CON_KEY_DOWN: if ( cid < 2 ) ++cid; break;
		
			case CON_KEY_CTRL:
				switch(cid)
				{
					case 0:	return 1;
					case 1: menuopt(g,py,px); break;
					case 2: return 0;
				}
			break;
			
			case 27:
				return 0;
				
			default: break;
		}
	}
	return 0;
}
