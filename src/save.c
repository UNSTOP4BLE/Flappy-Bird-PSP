#include <string.h>
#include <stdio.h>
#include "common.h"
#include "save.h"

void writeSave()
{
    FILE *saveFile = fopen("assets/savefile.save", "wb");
    fwrite(&prefs, sizeof(prefs), sizeof(prefs), saveFile);
    fclose(saveFile);   
}

void readSave()
{
    FILE *saveFile = fopen("assets/savefile.save", "rb");
    if (saveFile != NULL)
        fread(&prefs, sizeof(prefs), sizeof(prefs), saveFile);
    fclose(saveFile);
}