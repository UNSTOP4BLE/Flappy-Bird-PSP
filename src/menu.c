#include "menu.h"
#include "game.h"

static int floorx;
static float birdysin;
static int anim, animspeed;

static AnimFrames bird[4] = 
{
	{ 55,   0,  17,  12,   0,0},
	{ 83,   0,  17,  12,   0,0},
	{111,   0,  17,  12,   0,0},
	{111,   0,  17,  12,   0,0},
};

void MenuState()
{   
    floorx --;
  
    if (floorx <= -G2D_SCR_W)
        floorx = 0;

    birdysin += 0.5;

    //draw background
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

    //draw floor
    Rect floor_img = {0, 162, 168, 56};
    Rect floor_disp = {floorx, G2D_SCR_H - 56, G2D_SCR_W, G2D_SCR_H / 3};
    DrawG2DTex(game.tex, &floor_img, &floor_disp, false, 0, 255);
    floor_disp.x = floorx + G2D_SCR_W;
	DrawG2DTex(game.tex, &floor_img, &floor_disp, false, 0, 255);
    
    //draw bird
    PlayAnim(game.tex, bird, G2D_SCR_W / 2 - 15, sin(birdysin / 8) * 10 + 100, 10, 3, 1, false, &anim, &animspeed, 30, 20, false, true, 0);

    //draw logo
    Rect logo_img = {58, 13, 89, 24};
    Rect logo_disp = {G2D_SCR_W / 2 - 89, 40, 89 * 2, 24 * 2};
	DrawG2DTex(game.tex, &logo_img, &logo_disp, false, 0, 255);

	//draw button promts
	Rect start_img = {177, 71, 52, 29};
	Rect start_disp = {G2D_SCR_W / 2 - 52,  G2D_SCR_H - 56 - 40, 52 * 2, 29 * 2};
	DrawG2DTex(game.tex, &start_img, &start_disp, false, 0, 255);

	Rect cross_img = {143, 59, 10, 10};
	Rect cross_disp = {G2D_SCR_W / 2 - 10, G2D_SCR_H - 30, 10 * 2, 10 * 2};
	DrawG2DTex(game.tex, &cross_img, &cross_disp, false, 0, 255);

    //load the Playstate
    if (Pad_Pressed(PSP_CTRL_CROSS))
    {
		game.fadealpha = 255;
		Wav_Play(game.sfx[3]);
        PlayStateInit();   
 		game.gameMode = 1;
    }
	//black fade
	DrawG2DTex(game.tex, &game.black, &game.screen, false, 0, game.fadealpha);


}