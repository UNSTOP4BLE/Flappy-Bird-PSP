#include <math.h>   
#include <stdlib.h>   
#include "common.h" 

AnimFrames sparkles[5] = 
{
	{118, 146, 5, 5,  0, 0},
	{118, 146, 5, 5,  0, 0},
	{118, 146+5, 5, 5,  0, 0},
	{118, 146+10, 5, 5,  0, 0},
	{118, 146+10, 5, 5,  0, 0},
};

int sparkleanim, sparklespeed;
Rect sparkle;

void animatesparkle(int x, int y)
{
	if (sparkleanim == 0)
	{
		sparkleanim = 1;
		sparkle.x = x+2 + rand() % 32;
		sparkle.y = y+2 + rand() % 32;
	}

    PlayAnim(game.tex, sparkles, sparkle.x, sparkle.y, 6, 4, 1, false, &sparkleanim, &sparklespeed, sparkles[sparkleanim].w * 2, sparkles[sparkleanim].h * 2, false, true, 0);
}