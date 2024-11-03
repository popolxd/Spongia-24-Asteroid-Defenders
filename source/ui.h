#ifndef UI_H
#define UI_H

#include "world.h"

typedef struct{
    Rectangle rect;
    char* text;
    Color color;
    Color textColor;
}UI_Button;

typedef struct{
    Vector2* starPos;
    int starLen;
}UI_MenuBackground;

void UI_DrawMainMenu();
void UI_UpdateMainMenu(int* currentScreen, WORLD_World* world, UI_MenuBackground* background);

#endif
