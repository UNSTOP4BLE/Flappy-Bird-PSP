#include <string.h>                                                                                                                                                                                            
#include "../common.h"
#include "../game.h"

static struct
{   // X pos  Y pos  width  height 
	int charX, charY, charW, charH;
} fontmap[0x60] = {
	{128, 30, 4, 9,},  //space
	{80, 20, 3, 9}, //!
	{104, 30, 5, 9}, //"
	{88, 20, 7, 9}, //#
	{32, 30, 7, 9}, //$
	{96, 20, 7, 9}, //%
	{56, 30, 7, 9}, //&
	{112, 30, 3, 9}, //'
	{120, 20, 4, 9}, //(
	{128, 20, 4, 9}, //)
	{80, 30, 7, 9}, //*
	{168, 20, 5, 9}, //+
	{192, 20, 4, 9}, //,
	{168, 20, 5, 9}, //-
	{200, 20, 3, 9}, //.
	{8, 30, 7, 9}, // /
	{0, 20, 7, 9}, //0
	{8, 20, 7, 9}, //1
	{16, 20, 7, 9}, //2
	{24, 20, 7, 9}, //3
	{32, 20, 7, 9}, //4
	{40, 20, 7, 9}, //5
	{48, 20, 7, 9}, //6
	{56, 20, 7, 9}, //7
	{64, 20, 7, 9}, //8
	{72, 20, 7, 9}, //9
	{104, 20, 3, 9}, //:
	{120, 30, 3, 9}, //;
	{88, 30, 6, 9}, //<
	{184, 20, 6, 9}, //=
	{96, 30, 6, 9}, //>
	{0, 30, 7, 9}, //?
	{72, 30, 7, 9}, //@
	{0, 0, 7, 9}, //A
	{8, 0, 7, 9}, //B
	{16, 0, 7, 9}, //C
	{24, 0, 7, 9}, //D
	{32, 0, 7, 9}, //E
	{40, 0, 7, 9}, //F
	{48, 0, 7, 9}, //G
	{56, 0, 7, 9}, //H
	{64, 0, 5, 9}, //I
	{72, 0, 7, 9}, //J
	{80, 0, 7, 9}, //K
	{88, 0, 7, 9}, //L
	{96, 0, 7, 9}, //M
	{104, 0, 7, 9}, //N
	{112, 0, 7, 9}, //O
	{120, 0, 7, 9}, //P
	{128, 0, 7, 9}, //Q
	{136, 0, 7, 9}, //R
	{144, 0, 7, 9}, //S
	{152, 0, 7, 9}, //T
	{160, 0, 7, 9}, //U
	{168, 0, 7, 9}, //V
	{176, 0, 7, 9}, //W
	{184, 0, 7, 9}, //X
	{192, 0, 7, 9}, //Y
	{200, 0, 7, 9}, //Z
	{152, 20, 4, 9}, //[
	{16, 30, 7, 9}, // backslash
	{160, 20, 4, 9}, //]
	{64, 30, 5, 9}, //^
	{24, 30, 7, 9}, //_
	{40, 30, 4, 9}, //`
	{0, 10, 7, 9}, //a
	{8, 10, 7, 9}, //b
	{16, 10, 7, 9}, //c
	{24, 10, 7, 9}, //d
	{32, 10, 7, 9}, //e
	{40, 10, 6, 9}, //f
	{48, 10, 7, 9}, //g
	{56, 10, 7, 9}, //h
	{64, 10, 3, 9}, //i
	{72, 10, 5, 9}, //j
	{80, 10, 7, 9}, //k
	{88, 10, 3, 9}, //l
	{96, 10, 7, 9}, //m
	{104, 10, 7, 9}, //n
	{112, 10, 7, 9}, //o
	{120, 10, 7, 9}, //p
	{128, 10, 7, 9}, //q
	{136, 10, 7, 9}, //r
	{144, 10, 7, 9}, //s
	{152, 10, 5, 9}, //t
	{160, 10, 7, 9}, //u
	{168, 10, 7, 9}, //v
	{176, 10, 7, 9}, //w
	{184, 10, 7, 9}, //x
	{192, 10, 6, 9}, //y
	{200, 10, 7, 9}, //z
	{136, 20, 5, 9}, //{
	{112, 20, 3, 9}, //|
	{144, 20, 5, 9}, //}
	{48, 30, 7, 9}, //~
};

typedef struct 
{
    g2dTexture* fnttex; //1 tex, 2 font
} Tex;

Tex tex;

void FntInit(char *toload)
{	
	tex.fnttex = g2dTexLoad(toload, G2D_SWIZZLE);
}

void FntFree()
{
	g2dTexFree(&tex.fnttex);
}

static void changerect(int array, int x, int y, int w, int h)
{
	fontmap[array].charX = x;
	fontmap[array].charY = y;
	fontmap[array].charW = w;
	fontmap[array].charH = h;
}

void PrintMSG(const char *text, int x, int y, int font)
{	
	//Draw string character by character
	int c;
	int xhold = x;

	switch (font)
	{
		case 1: //big numbers 
			x -= strlen(text) * 13 >> 1; //center font

			changerect(16,         266,   0,  12,  18); //0
			changerect(17,  266+(12*1),   0,  12,  18); //1
			changerect(18,  266+(12*2),   0,  12,  18); //2
			changerect(19,         266,  19,  12,  18); //3
			changerect(20,  266+(12*1),  19,  12,  18); //4
			changerect(21,  266+(12*2),  19,  12,  18); //5
			changerect(22,         266,  38,  12,  18); //6
			changerect(23,  266+(12*1),  38,  12,  18); //7
			changerect(24,  266+(12*2),  38,  12,  18); //8
			changerect(25,         266,  57,  12,  18); //9
		break;
		case 2: //small numbers 
			changerect(16,        97,  90,   6,  7); //0
			changerect(17,  97+(6*1),  90,   6,  7); //1
			changerect(18,  97+(6*2),  90,   6,  7); //2
			changerect(19,  97+(6*3),  90,   6,  7); //3
			changerect(20,  97+(6*4),  90,   6,  7); //4
			changerect(21,  97+(6*5),  90,   6,  7); //5
			changerect(22,  97+(6*6),  90,   6,  7); //6
			changerect(23,  97+(6*7),  90,   6,  7); //7
			changerect(24,  97+(6*8),  90,   6,  7); //8
			changerect(25,  97+(6*9),  90,   6,  7); //9
		break;
		case 3: //big numbers, allign right 
			x -= strlen(text) * 13; //allign font

			changerect(16,        97,  79,   7,  10); //0
			changerect(17,  97+(8*1),  79,   7,  10); //1
			changerect(18,  97+(8*2),  79,   7,  10); //2
			changerect(19,  97+(8*3),  79,   7,  10); //3
			changerect(20,  97+(8*4),  79,   7,  10); //4
			changerect(21,  97+(8*5),  79,   7,  10); //5
			changerect(22,  97+(8*6),  79,   7,  10); //6
			changerect(23,  97+(8*7),  79,   7,  10); //7
			changerect(24,  97+(8*8),  79,   7,  10); //8
			changerect(25,  97+(8*9),  79,   7,  10); //9
		break;
		default:
			changerect(16,   0, 20, 7, 9); //0
			changerect(17,   8, 20, 7, 9); //1
			changerect(18,  16, 20, 7, 9); //2
			changerect(19,  24, 20, 7, 9); //3
			changerect(20,  32, 20, 7, 9); //4
			changerect(21,  40, 20, 7, 9); //5
			changerect(22,  48, 20, 7, 9); //6
			changerect(23,  56, 20, 7, 9); //7
			changerect(24,  64, 20, 7, 9); //8
			changerect(25,  72, 20, 7, 9); //9
		break;
	}

	while ((c = *text++) != '\0')
	{
		if (c == '\n')
		{
			x = xhold;
			y += 11;
		}
		//Shift and validate character
		if ((c -= 0x20) >= 0x60)
			continue;
		
		//Draw character
		Rect font_Img = {fontmap[c].charX, fontmap[c].charY, fontmap[c].charW, fontmap[c].charH};
		Rect font_Disp = {x, y, fontmap[c].charW, fontmap[c].charH};
		if (font > 0)
		{
			font_Disp.w = fontmap[c].charW * 2;
			font_Disp.h = fontmap[c].charH * 2;
			DrawG2DTex(game.tex, &font_Img, &font_Disp, false, 0, 255);
		}
		else
			DrawG2DTex(tex.fnttex, &font_Img, &font_Disp, false, 0, 255);

		//Increment X
		if (font == 1)
			x += fontmap[c].charW*2 - 1;
		else if (font == 2)
			x += fontmap[c].charW*2 - 1;
		else if (font == 3)
			x += fontmap[c].charW*2 - 1;
		else
			x += fontmap[c].charW - 1;
	}
}
