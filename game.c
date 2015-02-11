#include "main.h"

#define resetspmap(G,I) for ( I = 0; I < MAPH; ++I ) memset(&G->spmap[I][0],0,sizeof(BYTE) * MAPW)

VOID initrandom()
{
	srand(time(NULL));
}

INT32 nrandom(INT32 max)
{
	return rand() % (max+1);
}

INT32 randomrange(INT32 min,INT32 max)
{
    return min + (rand() % (max-min+1));
}

VOID loadthema(MXC* g, CHAR* name)
{
	CHAR ph[1024];
	
	sprintf(ph,"%s/.config/magixcubic",pht_homedir());
	if ( !ino_exist(ph) )
	{
		prv_maskreset();
		if ( !dir_new(ph,0755) ) return;
	}
	
	sprintf(ph,"%s/.config/magixcubic/%s",pht_homedir(),name);
	CHAR n[80];
	CHAR v[80];
	if ( !ino_exist(ph) )
	{
		FILE* f = fopen(ph,"w");
			if ( !f ) { con_async(1,NULL); return;}
		INT32 i;
		
		for ( i = 0; i < 9; ++i )
		{
			sprintf(n,"color%d",i);
			sprintf(v,"%d",g->lcom[i]);
			cfg_write(n,v,f);
			
			sprintf(n,"char%d",i);
			sprintf(v,"%c",g->lchm[i]);
			cfg_write(n,v,f);
		}
		cfg_write("forecolor","16",f);
		cfg_write("mode","256",f);
		fclose(f);
	}
	
	FILE* f = fopen(ph,"r");
		if ( !f ) { con_async(1,NULL); return;}
	
	INT32 i;
	while ( cfg_read(n,v,f) )
	{
		if ( !strncmp(n,"color",5) )
		{
			i = atoi(&n[5]);
			if ( i < 0 || i > 8 ) continue;
			g->lcom[i] = atoi(v);
		}
		else if ( !strncmp(n,"char",4) )
		{
			i = atoi(&n[4]);
			if ( i < 0 || i > 8 ) continue;
			g->lchm[i] = (v[0]) ? v[0] : ' ';
		}
		else if ( !strncmp(n,"mode",4) )
		{
			setcolor = (!strncmp(v,"256",3)) ? con_setcolor256 : con_setcolor;
		}
		else if ( !strncmp(n,"forecolor",9) )
		{
			i = atoi(v);
			if ( i < 0 || i > 255 ) continue;
			g->fcol = i;
		}
	}
	
	
	fclose(f);
}

VOID initmxc(MXC* g)
{
	initrandom();
	
	setcolor = con_setcolor256;
	
	con_getmaxrc(&g->scrh,&g->scrw);
	g->lmm = 5;
	g->sty = (g->scrh > 30) ? 8 : 4;
	g->stx = (g->scrw > 80) ? 16 : 2;
	g->pointy = (g->scrh > 30) ? 4 : 2;
	g->pointx = (g->scrw > 80) ? 16 : 2;
	g->titley = (g->scrh > 30) ? 2 : 1;
	g->titlex = (g->scrw > 80) ? 16 : 2;
	
	g->fcol = FONTCOLOR;	
	
	g->lcom[0] = 1;
	g->lcom[1] = 2;
	g->lcom[2] = 3;
	g->lcom[3] = 4;
	g->lcom[4] = 5;
	g->lcom[5] = 6;
	g->lcom[6] = 11;
	g->lcom[7] = 14;
	g->lcom[8] = 16;
	
	g->lchm[0] = '%';
	g->lchm[1] = '?';
	g->lchm[2] = '@';
	g->lchm[3] = '&';
	g->lchm[4] = '-';
	g->lchm[5] = '^';
	g->lchm[6] = '#';
	g->lchm[7] = 'o';
	g->lchm[8] = ' ';
	
	
	loadthema(g,"default.thm");
	
	CHAR ph[1024],d[1024];
	sprintf(ph,"%s/.config/magixcubic",pht_homedir());
	
	FILETYPE ft = dir_list(d,TRUE,FT_REG,ph);
	while ( ft >= FT_UNKNOWN && strcmp(d,"default.thm") )
		ft = dir_list(d,TRUE,FT_REG,NULL);
	
	con_async(1,NULL);
}

VOID resetgame(MXC* g)
{
	con_cls();
	con_gotorc(g->titley,g->titlex);
	printf("%s",TITLEAPP);
	
	g->points = 0;
	INT32 y,x;
	for ( y = 0; y < MAPH; ++y)
		for ( x = 0; x < MAPW; ++x)
			g->map[y][x] = nrandom(g->lmm-1);
			
	g->x = 1;
	g->y = 0;
}

VOID draw(MXC* g, BOOL drwi)
{
	con_gotorc(g->pointy,g->pointx);
	setcolor(0,0);
	con_clsline(CON_CLLS_RIGHT);
	printf("Points:%d",g->points);
	
	if ( drwi )
	{
		con_rect(g->sty,g->stx,MAPH + 2,MAPW + 3);
		con_line(g->sty + 1, g->stx + 2, g->sty + MAPH, g->stx + 2, '|');
	}
	
	INT32 y,x;
	for ( y = 0; y < MAPH; ++y)
	{
		con_gotorc(y + g->sty + 1, g->stx + 1);
		setcolor(g->lcom[g->map[y][0]],g->fcol);
		putchar(g->lchm[g->map[y][0]]);
	}
	
	for ( y = 0; y < MAPH; ++y)
	{
		for ( x = 1; x < MAPW; ++x)
		{
			con_gotorc(y + g->sty + 1, x + g->stx + 2);
			setcolor(g->lcom[g->map[y][x]],g->fcol);
			putchar(g->lchm[g->map[y][x]]);
		}
	}
	
	con_gotorc(g->y + g->sty + 1, g->x + g->stx + 2);
	con_flush();
}

INT32 gkey(MXC* g, BYTE cmd)
{
	#ifdef _DEBUG
		setcolor(0,0);
		con_gotorc(5,25);
		printf("key:%d",cmd);
	#endif
	
	switch(cmd)
	{
		case CON_KEY_LEFT:
			if ( g->x > 1 ) --g->x;
		break;
		
		case CON_KEY_RIGHT:
			if ( g->x < MAPW - 1 ) ++g->x;
		break;
		
		case CON_KEY_UP:
			if ( g->y > 0 ) --g->y;
		break;
		
		case CON_KEY_DOWN:
			if ( g->y < MAPH - 1) ++g->y;
		break;
		
		case CON_KEY_CTRL:
			return 2;
		
		case 27:
			return 1;
		
		default:
			return 0;
	}
	
	#ifdef _DEBUG
	setcolor(0,0);
	con_gotorc(6,25);
	printf("y:%2d x%2d",g->y,g->x);
	#endif
	
	return 0;
}

INT32 ncubic(MXC* g, INT32 y, INT32 x, BYTE m, INT32* n)
{
	if ( g->spmap[y][x] ) return 0;
	if ( g->map[y][x] != m ) return 0;
	if ( g->map[y][x] == EMPTYL ) return 0;
	
	g->spmap[y][x] = 1;
	++(*n);
	if ( y > 0 && g->map[y-1][x] == m && g->map[y-1][x] != EMPTYL ) ncubic(g,y - 1,x,m,n);
	if ( x > 1 && g->map[y][x-1] == m && g->map[y][x-1] != EMPTYL) ncubic(g,y,x - 1,m,n);
	if ( y < MAPH - 1 && g->map[y+1][x] == m && g->map[y+1][x] != EMPTYL) ncubic(g,y + 1,x,m,n);
	if ( x < MAPW - 1 && g->map[y][x+1] == m && g->map[y][x+1] != EMPTYL) ncubic(g,y,x + 1,m,n);
	
	return 0;
}

VOID kcubic(MXC* g, INT32 y, INT32 x, BYTE m)
{
	if ( g->spmap[y][x] ) return;
	if ( g->map[y][x] != m ) return;
	if ( g->map[y][x] == EMPTYL ) return;
	
	g->spmap[y][x] = 1;
	g->map[y][x] = EMPTYL;
	
	con_gotorc(g->sty + y + 1,g->stx + x + 2);
	putchar(' ');
	con_flush();
	thr_sleep(TIME_K);
	
	if ( y > 0 && g->map[y-1][x] == m && g->map[y-1][x] != EMPTYL) kcubic(g,y - 1,x,m);
	if ( x > 1 && g->map[y][x-1] == m && g->map[y][x-1] != EMPTYL) kcubic(g,y,x - 1,m);
	if ( y < MAPH - 1 && g->map[y+1][x] == m && g->map[y+1][x] != EMPTYL ) kcubic(g,y + 1,x,m);
	if ( x < MAPW - 1 && g->map[y][x+1] == m && g->map[y][x+1] != EMPTYL) kcubic(g,y,x + 1,m);
}

BOOL isgameover(MXC* g)
{
	INT32 i;
	INT32 y,x;
	INT32 nc;
	  
	for ( y = 0; y < MAPH; ++y)
	{
		for ( x = 1; x < MAPW; ++x)
		{
			if ( g->map[y][x] == EMPTYL ) continue;
			
			nc = 0;
			resetspmap(g,i);
			ncubic(g,y,x,g->map[y][x],&nc);
			
			#ifdef _DEBUG
			setcolor(0,0);
			con_gotorc(8,25);
			printf("help y:%2d x:%2d nc:%2d",y,x,nc);
			con_flush();
			#endif
			if (  nc > 2) return FALSE;
		}
	} 
	return TRUE;
}

INT32 destroycubic(MXC* g)
{
	INT32 i;
	INT32 nc = 0;
	
	resetspmap(g,i);
	ncubic(g,g->y,g->x,g->map[g->y][g->x],&nc);
	
	#ifdef _DEBUG
	//con_setcolor256(0,0);
	//con_gotorc(7,25);
	//printf("ncubuc:%10d",nc);
	//con_flush();
	#endif
	
	if ( nc < 3 ) return 0;
	
	resetspmap(g,i);
	setcolor(g->lcom[g->map[g->y][g->x]],g->fcol);
	kcubic(g,g->y,g->x,g->map[g->y][g->x]);
	
	setcolor(0,0);
	
	return (nc + g->lmm - 3)  * (nc / 3);
}

INT32 spaceup(MXC* g,INT32 sty, INT32 x)
{
	INT32 y;
	for (y = sty; y > 0 ; --y)
	{
		if ( g->map[y][x] == EMPTYL ) 
		{
			INT32 k;
			for ( k = y -1; k >= 0; --k )
				if ( g->map[k][x] < EMPTYL ) return y;
			return -1;
		}
	}
	return -1;
}

VOID copydown(MXC* g,INT32 sty, INT32 x)
{
	INT32 y;
	for (y = sty; y > 0 ; --y)
	{
		g->map[y][x] = g->map[y-1][x];
		con_gotorc(y + g->sty + 1, x + g->stx + 2);
		setcolor(g->lcom[g->map[y][x]],g->fcol);
		putchar(g->lchm[g->map[y][x]]);
		con_gotorc( g->y + g->sty + 1, g->x + g->stx + 2);
		con_flush();
		thr_sleep(TIME_MY);
	}
	g->map[0][x] = EMPTYL;
	con_gotorc(g->sty + 1, x + g->stx + 2);
	setcolor(g->lcom[g->map[0][x]],g->fcol);
	putchar(g->lchm[g->map[0][x]]);
	con_gotorc( g->y + g->sty + 1, g->x + g->stx + 2);
	con_flush();
}

INT32 pushdown(MXC* g)
{
	INT32 x;
	INT32 sts;
	INT32 nc = 0;
	
	#ifdef _DEBUG
	int di=0;
	#endif
	
	for ( x = 1; x < MAPW; ++x)
	{
		sts = MAPH -1;
		while( (sts = spaceup(g,sts,x)) > -1 )
		{
			#ifdef _DEBUG
			//con_setcolor256(0,0);
			//con_gotorc(9+di++,25);
			//printf("pushdown: sts:%2d x:%2d",sts,x);
			//con_flush();
			#endif
	
			++nc;
			copydown(g,sts,x);
		}
	}
	return nc;
}

INT32 spaceleft(MXC* g,INT32 stx, INT32 y)
{
	INT32 x;
	
	for (x = stx; x > 1 ; --x)
	{
			#ifdef _DEBUG
			//con_setcolor256(0,0);
			//con_gotorc(9+y,25+x);
			//con_clsline(CON_CLLS_RIGHT);
			//printf("%1d",g->map[y][x]);
			//con_flush();
			#endif
		
		if ( g->map[y][x] == EMPTYL ) 
		{
			INT32 k;
			for ( k = x -1; k >= 1; --k )
				if ( g->map[y][k] < EMPTYL ) return x;
			return -1;
		}
	}
	return -1;
}

VOID copyright(MXC* g,INT32 stx, INT32 y)
{
	INT32 x;
	for (x = stx; x > 1 ; --x)
	{
		g->map[y][x] = g->map[y][x-1];
		con_gotorc(y + g->sty + 1, x + g->stx + 2);
		setcolor(g->lcom[g->map[y][x]],g->fcol);
		putchar(g->lchm[g->map[y][x]]);
		con_gotorc( g->y + g->sty + 1, g->x + g->stx + 2);
		con_flush();
		thr_sleep(TIME_MX);
	}
	g->map[y][1] = EMPTYL;
	con_gotorc(y + g->sty + 1, g->stx + 3);
	setcolor(g->lcom[g->map[0][x]],g->fcol);
	putchar(g->lchm[g->map[y][1]]);
	con_gotorc( g->y + g->sty + 1, g->x + g->stx + 2);
	con_flush();
}

INT32 pushright(MXC* g)
{
	INT32 y;
	INT32 sts;
	INT32 nc = 0;
	
	#ifdef _DEBUG
	int di=0;
	#endif
	
	for ( y = 0; y < MAPH; ++y)
	{
		sts = MAPW -1;
		while( (sts = spaceleft(g,sts,y)) > -1 )
		{
			#ifdef _DEBUG
			//con_setcolor256(0,0);
			//con_gotorc(9+di++,25);
			//printf("pushright: sts:%2d y:%2d",sts,y);
			//con_flush();
			#endif
			
			++nc;
			copyright(g,sts,y);
		}
	}
	return nc;
}

INT32 inscubic(MXC* g)
{
	INT32 y;
	for (y = 0; y < MAPH; ++y)
		if ( g->map[y][1] != EMPTYL ) return 0;
		
	for (y = 0; y < MAPH; ++y)
	{
		g->map[y][1] = g->map[y][0];
		g->map[y][0] = nrandom(g->lmm-1);
	}
	return 1;
}
