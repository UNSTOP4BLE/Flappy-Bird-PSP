#include <pspdebug.h>
#include <pspkernel.h>
#include <psputility.h>
#include <time.h>
#include "psp/callbacks.h"
#include "common.h"
#include "menu.h"
#include "game.h"
#include "save.h"

PSP_MODULE_INFO("Flappy Bird PSP", 0, 1, 0);

Common game;
Prefs prefs;

int main() 
{
    setupcallbacks();
	Wav_Init();
	Pad_Init();
  	g2dInit();
  	FntInit("assets/font.png");

  	game.tex = g2dTexLoad("assets/tex.png", G2D_SWIZZLE);

    game.sfx[0] = Wav_Load("assets/sfx_wing.wav"); 
    game.sfx[1] = Wav_Load("assets/sfx_hit.wav"); 
   	game.sfx[2] = Wav_Load("assets/sfx_point.wav"); 
   	game.sfx[3] = Wav_Load("assets/sfx_swooshing.wav"); 
   	game.sfx[4] = Wav_Load("assets/sfx_die.wav"); 
  	
    srand(time(NULL));
    
	readSave();

    game.white.x = 101;
    game.white.y = 145;
    game.white.w = 16;
    game.white.h = 16;

    game.black.x = 84;
    game.black.y = 145;
    game.black.w = 16;
    game.black.h = 16;

    game.screen.x = 0;
    game.screen.y = 0;
    game.screen.w = G2D_SCR_W;
    game.screen.h = G2D_SCR_H;

  	while(1)
  	{
       	g2dClear(G2D_RGBA(84, 192, 201, 0));

       	Pad_Update();

       	if (Pad_Pressed(PSP_CTRL_SELECT))
       		prefs.darkmode = !prefs.darkmode;

       	if (game.fadealpha > 0)
			game.fadealpha -= 20;

       	switch (game.gameMode)
       	{
       		case 0:
       			MenuState();
       		break;
       		case 1:
       			PlayState();
       		break;
       	}

		g2dFlip(G2D_VSYNC);
  	}

	g2dTexFree(&game.tex);
	Wav_Free(game.sfx[0]);
	Wav_Free(game.sfx[1]);
	Wav_Free(game.sfx[2]);
	Wav_Free(game.sfx[3]);
	Wav_Free(game.sfx[4]);
  	Pad_Shutdown();
  	FntFree();

  	return 0;
}