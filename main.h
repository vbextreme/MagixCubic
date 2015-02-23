#ifndef MAIN_H_INCLUDE
#define MAIN_H_INCLUDE

//SUONI
//THEMA NON 256 color

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <easytype.h>
#include <easyconsole.h>
#include <easythread.h>
#include <easyfile.h>

#define TITLEAPP "MagixCubic 0.2"
#define DIVAPP   "--------------"
#define OVERAPP "G a m e  O v e r"
#define TIME_I 0.03
#define TIME_K 0.03
#define TIME_MY 0.003
#define TIME_MX 0.0008
#define TIME_AT1 0.001
#define TIME_AT2 0.05
#define TIME_AT3 0.8
#define TIME_AT4 0.5
#define TIME_ATG 0.01

#define BONUS 100

#define MAPW 17
#define MAPH 16
#define MAPL 9
#define EMPTYL 8
#define FONTCOLOR 16

typedef struct _MXC
{
	INT32 lmm;
	BYTE map[MAPH][MAPW];
	BYTE spmap[MAPH][MAPW];
	BYTE lcom[MAPL];
	BYTE lchm[MAPL];
	BYTE fcol;
	UINT32 y;
	UINT32 x;
	UINT32 scrw;
	UINT32 scrh;
	UINT32 stx;
	UINT32 sty;
	UINT32 points;
	UINT32 pointx;
	UINT32 pointy;
	UINT32 titlex;
	UINT32 titley;
}MXC;

VOID presentat(CHAR* app, UINT32* py, UINT32* px);
VOID gameoverat(MXC* g);
INT32 menumain(MXC* g, UINT32 py, UINT32 px);

VOID loadthema(MXC* g, CHAR* name);
VOID initmxc(MXC* g);
VOID resetgame(MXC* g);
VOID draw(MXC* g, BOOL drwi);
INT32 gkey(MXC* g, BYTE cmd);
BOOL isgameover(MXC* g);
INT32 destroycubic(MXC* g);
INT32 pushdown(MXC* g);
INT32 pushright(MXC* g);
INT32 inscubic(MXC* g);

VOID (*setcolor)(BYTE,BYTE);

#endif

