#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "game.h"
#include "save.h"
#include "sparkle.h"

static char scoretext[9999];
static int anim, animspeed, soundtimer, fadestate;
static float birdy, birdspeed, floorx, birdl, birdr, birdt, birdb, birdangle, gamespeed, scrollspeed;
static bool start, paused, isdead, cangetcoin;
void drawBG();
void drawFloor();
void drawPipe(int thepipe);
void checkColission();
void deadState();
Rect gameover_disp = {G2D_SCR_W / 2 - 94, 80, 94*2, 19*2};
Rect box_disp = {G2D_SCR_W / 2 - 113, G2D_SCR_H, 113*2, 58*2};
Rect medal_img = {0, 0, 22, 22};
Rect medal_disp = {0, 0, 22 * 2, 22 * 2};
Rect new_disp = {0, 0, 16 * 2, 7 * 2};
	
typedef struct
{
    float x, y;
    float t, l, r;
    float tb;
} Pipe;

Pipe pipes[2];

static AnimFrames bird[4] = 
{
	{ 55,   0,  17,  12,   0,0},
	{ 83,   0,  17,  12,   0,0},
	{111,   0,  17,  12,   0,0},
	{111,   0,  17,  12,   0,0},
};

void PlayStateInit()
{
    gameover_disp.y = 80;
    box_disp.y = G2D_SCR_H;
    prefs.score = 0;
    fadestate = 0;
    start = 1; 
    isdead = false;
    cangetcoin = true;
    scrollspeed = 0;
    birdy = 30;
    birdspeed = 0;
    birdangle = 0;
    gamespeed = 2;
    soundtimer = 0;
    pipes[0].x = -50;
    pipes[1].x = -50 + G2D_SCR_W / 2;
    pipes[0].y = rand() % (G2D_SCR_H - 56);
    pipes[1].y = rand() % (G2D_SCR_H - 56);        
}

void PlayState()
{   
    pipes[0].t = pipes[0].y;
    pipes[0].l = pipes[0].x;
    pipes[0].r = pipes[0].x + 50;
    pipes[0].tb = pipes[0].y - 100;
    
    pipes[1].t = pipes[1].y;
    pipes[1].l = pipes[1].x;
    pipes[1].r = pipes[1].x + 50;
    pipes[1].tb = pipes[1].y - 100;
    
    birdt = birdy;
    birdb = birdy + 20;
    birdl = 30;
    birdr = 30 + 30;
        
    drawBG();
    
    if (isdead) //back to menu
    {
		if (Pad_Pressed(PSP_CTRL_CROSS))
		{
			game.gameMode = 0;
			game.fadealpha = 255;
			Wav_Play(game.sfx[3]);
		}
    }
    //is the game paused?
    if (!start && !isdead && Pad_Pressed(PSP_CTRL_START))
    {
        paused = !paused;
		Wav_Play(game.sfx[3]);
    }
        
    //control the bird
    if (!start && !paused && !isdead)
    {
        birdy += birdspeed;
        birdspeed += 0.180;
    }
   	birdangle = (birdspeed/5);

   	if (birdangle >= 1.55) //dont let the bird do a 360
   		birdangle = 1.55;

    if (!start && !paused && Pad_Pressed(PSP_CTRL_CROSS) && !isdead)
    {
        Wav_Play(game.sfx[0]);
        birdspeed = -3;
    }

    drawPipe(0);
    drawPipe(1);

    drawFloor();
    
    //draw bird
    if (paused || isdead)
        PlayAnim(game.tex, bird, 30, birdy, 10, 3, 1, false, &anim, &animspeed, 30, 20, false, false, birdangle);
    else 
        PlayAnim(game.tex, bird, 30, birdy, 10, 3, 1, false, &anim, &animspeed, 30, 20, false, true, birdangle);
         
    if (start && Pad_Pressed(PSP_CTRL_CROSS) && !isdead)
        start = 0;
    
    if (start)
    {   	
        Rect startscreen_img = {143, 103, 57, 49};
        Rect startscreen_disp = {G2D_SCR_W / 2 - 57, 87 + 10, 57 * 2, 49 * 2};
        DrawG2DTex(game.tex, &startscreen_img, &startscreen_disp, false, 0, 255);

        Rect ready_img = {215, 136, 92, 25};
        Rect ready_disp = {G2D_SCR_W / 2 - 92, 39 + 10, 92 * 2, 25 * 2};
        DrawG2DTex(game.tex, &ready_img, &ready_disp, false, 0, 255);
    }
    
    Rect pause_img = {56, 139, 13, 14};
    Rect pause_disp = {0, 0, 13 * 2, 14 * 2};
        
    if (!isdead)
    {
        if (paused)
        {
            pause_img.x = 56;
            DrawG2DTex(game.tex, &pause_img, &pause_disp, false, 0, 255);
        }
        else
        {
            pause_img.x = 70;
            DrawG2DTex(game.tex, &pause_img, &pause_disp, false, 0, 255);
        }
    }
    
    //draw and increment score
	if (((birdl >= pipes[0].l && birdl <= pipes[0].r) || (birdl >= pipes[1].l && birdl <= pipes[1].r)) && cangetcoin && !isdead)
    {
    	Wav_Play(game.sfx[2]);
    	
        if ((prefs.score % 10) == 9) //increase game speed by 0.3 every 10 points
            gamespeed += 0.3;
        
        prefs.score += 1;
        cangetcoin = false;
    }
    
    if ((birdl > pipes[0].r && birdr < pipes[1].r) || (birdl > pipes[1].r && birdr < pipes[0].r))
        cangetcoin = true;

    sprintf(scoretext, "%d", prefs.score);

    if (!isdead)
 	{
 	    checkColission();
        PrintMSG(scoretext, G2D_SCR_W / 2, 10, 1);
 	}
    else
    {
	   	deadState(); 		
    }

    if (start) //draw black fade
		DrawG2DTex(game.tex, &game.black, &game.screen, false, 0, game.fadealpha);
}

void drawBG()
{ 
    Rect bg_img = {0, 313, 138, 105};
    Rect bg_disp = {0, G2D_SCR_H-110, 138, 110};
    if (prefs.darkmode)
    {
    	bg_img.x = 169;
    	bg_img.y = 162;
    	bg_img.h = 256;

    	bg_disp.y = 0;
    	bg_disp.h = G2D_SCR_H+2;
    }

    DrawG2DTex(game.tex, &bg_img, &bg_disp, false, 0, 255);
    bg_disp.x = 137;
    DrawG2DTex(game.tex, &bg_img, &bg_disp, false, 0, 255);
    bg_disp.x = 137 * 2;
    DrawG2DTex(game.tex, &bg_img, &bg_disp, false, 0, 255);
    bg_disp.x = 137 * 3;
    DrawG2DTex(game.tex, &bg_img, &bg_disp, false, 0, 255);
}

void drawFloor()
{
    if (!start && !paused && !isdead)
        floorx -= gamespeed;
  
    if (floorx <= -G2D_SCR_W)
        floorx = 0;

    Rect floor_img = {0, 162, 168, 56};
    Rect floor_disp = {floorx, G2D_SCR_H - 56, G2D_SCR_W, G2D_SCR_H / 3};
    DrawG2DTex(game.tex, &floor_img, &floor_disp, false, 0, 255);
    floor_disp.x = floorx + G2D_SCR_W;
	DrawG2DTex(game.tex, &floor_img, &floor_disp, false, 0, 255);
}

void drawPipe(int thepipe)
{       
    if (!start && !paused && !isdead)
        pipes[thepipe].x -= gamespeed;
        
    if (pipes[thepipe].x <= -50)
    {
        pipes[thepipe].x = G2D_SCR_W;
        pipes[thepipe].y = rand() % (G2D_SCR_H - 56);
    }
    
    Rect pipebottom_img = {28, 0, 26, 160};
    Rect pipebottom_disp = {pipes[thepipe].x, pipes[thepipe].y, 50, 300};
    DrawG2DTex(game.tex, &pipebottom_img, &pipebottom_disp, false, 0, 255);

    Rect top_img = {0, 0, 26, 160};
    Rect top_disp = {pipes[thepipe].x, pipes[thepipe].y - 400, 50, 300};
    DrawG2DTex(game.tex, &top_img, &top_disp, false, 0, 255);
}

void checkColission()
{
    if ((birdr >= pipes[0].l && birdb >=  pipes[0].t && birdl <= pipes[0].r) || 
        (birdr >= pipes[1].l && birdb >=  pipes[1].t && birdl <= pipes[1].r) || 
        (birdr >= pipes[0].l && birdt <= pipes[0].tb && birdl <= pipes[0].r) ||
        (birdr >= pipes[1].l && birdt <= pipes[1].tb && birdl <= pipes[1].r) || 
        (birdb >= G2D_SCR_H - 56))
    {
    	prefs.lastmax = prefs.maxscore;
    	game.fadealpha = 255;
    	if (prefs.score > prefs.maxscore)
    	    prefs.maxscore = prefs.score;
    	writeSave();
    	Wav_Play(game.sfx[1]);
		isdead = true;
    }
}

void deadState()
{
	soundtimer ++;
	scrollspeed += 0.5;
	Rect gameover_img = {55, 37, 96, 21};
    Rect new_img = {66, 154, 16, 7};
	Rect box_img = {152, 0, 113, 58};
    Rect play_img = {177, 71, 52, 29};
    Rect play_disp = {G2D_SCR_W / 2 - 52, G2D_SCR_H - 60, 52 * 2, 29 * 2};
	Rect cross_img = {143, 59, 10, 10};
    Rect cross_disp = {G2D_SCR_W / 2 + 60, G2D_SCR_H - 56 + 10, 10 * 2, 10 * 2};
			
	if (Wav_Playing() && game.gameMode == 1)	
		DrawG2DTex(game.tex, &game.white, &game.screen, false, 0, game.fadealpha);

	switch (soundtimer)
	{
		case 20:
			scrollspeed = 0;
			Wav_Play(game.sfx[4]);
			fadestate = 1;
		break;
		case 60:
		    scrollspeed = 0;
		    fadestate = 3;
		break;
		case 100:
		    fadestate = 4;
		break;
	}

	switch (fadestate)
	{
		case 1:
			if (gameover_disp.y == 80)
    	        Wav_Play(game.sfx[3]);
    	
			gameover_disp.y -= sin(scrollspeed/2)*5;
			DrawG2DTex(game.tex, &gameover_img, &gameover_disp, false, 0, scrollspeed*10);
		    if (scrollspeed >= 6)
		        fadestate = 2;
		break;
		case 2:
			DrawG2DTex(game.tex, &gameover_img, &gameover_disp, false, 0, 255);
		break;
		case 3:
			medal_disp.x = box_disp.x + (13*2);
			medal_disp.y = box_disp.y + (21*2);
		    if (scrollspeed <= 13.5)
		    {
		        box_disp.y -= scrollspeed;
		    }

			DrawG2DTex(game.tex, &box_img, &box_disp, false, 0, 255);
			DrawG2DTex(game.tex, &gameover_img, &gameover_disp, false, 0, 255);

            PrintMSG(scoretext, G2D_SCR_W / 2 + 90, box_disp.y + 35, 3);
            sprintf(scoretext, "%d", prefs.maxscore);
            PrintMSG(scoretext, G2D_SCR_W / 2 + 90, box_disp.y + 77, 3);

            //draw new label
            new_disp.x = box_disp.x + (68*2);
            new_disp.y = box_disp.y + (29*2);
            if (prefs.maxscore > prefs.lastmax)
            	DrawG2DTex(game.tex, &new_img, &new_disp, false, 0, 255);

            //draw medals
            if (prefs.score >= 40) {
            	//platinum sparkle
            	medal_img.x = 97;
            	medal_img.y = 98;
            } else if (prefs.score >= 30) {
            	//gold sparkle
            	medal_img.x = 120;
            	medal_img.y = 121;
            } else if (prefs.score >= 20) {
            	//silver sparkle
            	medal_img.x = 120;
            	medal_img.y = 98;
            } else if (prefs.score >= 10) {
            	//bronze sparkle
            	medal_img.x = 97;
            	medal_img.y = 121;
            } else {
            	//none 
            	medal_img.x = 279;
            	medal_img.y = 58;
            }

            DrawG2DTex(game.tex, &medal_img, &medal_disp, false, 0, 255);
            if (prefs.score >= 10) //draw sparkle
				animatesparkle(box_disp.x + (13*2), box_disp.y + (21*2));
		break;
		case 4:
        	DrawG2DTex(game.tex, &play_img, &play_disp, false, 0, 255);
        	DrawG2DTex(game.tex, &cross_img, &cross_disp, false, 0, 255);
        	
			DrawG2DTex(game.tex, &box_img, &box_disp, false, 0, 255);
			DrawG2DTex(game.tex, &gameover_img, &gameover_disp, false, 0, 255);
			
            PrintMSG(scoretext, G2D_SCR_W / 2 + 90, box_disp.y + 35, 3);
            sprintf(scoretext, "%d", prefs.maxscore);
            PrintMSG(scoretext, G2D_SCR_W / 2 + 90, box_disp.y + 77, 3);

            //draw new label
            if (prefs.maxscore > prefs.lastmax)
            	DrawG2DTex(game.tex, &new_img, &new_disp, false, 0, 255);

            //draw medals
            if (prefs.score >= 40) {
            	//platinum sparkle
            	medal_img.x = 97;
            	medal_img.y = 98;
            } else if (prefs.score >= 30) {
            	//gold sparkle
            	medal_img.x = 120;
            	medal_img.y = 121;
            } else if (prefs.score >= 20) {
            	//silver sparkle
            	medal_img.x = 120;
            	medal_img.y = 98;
            } else if (prefs.score >= 10) {
            	//bronze sparkle
            	medal_img.x = 97;
            	medal_img.y = 121;
            } else {
            	//none 
            	medal_img.x = 279;
            	medal_img.y = 58;
            }

            DrawG2DTex(game.tex, &medal_img, &medal_disp, false, 0, 255);

            if (prefs.score >= 10) //draw sparkle
				animatesparkle(box_disp.x + (13*2), box_disp.y + (21*2));

        break;
	}

	if (birdb < G2D_SCR_H - 56)
	{
		birdy += birdspeed;
        birdspeed += 0.180;
	}
	else
		birdy = G2D_SCR_H - 56 - 20;
}