#ifndef __COMMON_H__
#define __COMMON_H__

#include "psp/wav.h"
#include "psp/pad.h"
#include "psp/glib2d.h"
#include "psp/animation.h"
#include "psp/font.h"

typedef struct 
{
	Rect screen, black, white;
	int gameMode;
	int fadealpha;
    g2dTexture* tex;
    Wav* sfx[5]; //1 flap, 2 hit, 3 point, 4 swoosh, 5 die
} Common;

typedef struct 
{
	int score, maxscore, lastmax;
	bool darkmode;
} Prefs;

extern Common game;
extern Prefs prefs;

#endif
