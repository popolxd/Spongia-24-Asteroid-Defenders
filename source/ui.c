#include "ui.h"
#include "world.h"
#include "main.h"
#include "textures.h"

#include <raylib.h>


int UI_IsButtonPressed(UI_Button* button)
{
  return (
    IsMouseButtonPressed(MOUSE_BUTTON_LEFT) 
    && GetMouseX()>button->rect.x && GetMouseX()<button->rect.x+button->rect.width 
    && GetMouseY()>button->rect.y && GetMouseY()<button->rect.y+button->rect.height
  );
}

void UI_CreateButtonRect(UI_Button* button, int x, int y, int size, char* text, Color color)
{
  button->rect=(Rectangle){
    .x = x,
    .y = y,
    .height = size,
    .width = MeasureText(text, size)
  };
  button->text = text;
  button->color = color;
  button->textColor = WHITE;
}

void UI_DrawButton(UI_Button* button)
{
  DrawRectangleRec(button->rect, button->color);
  DrawText(button->text, button->rect.x, button->rect.y, button->rect.height, button->textColor);
}

void UI_DrawMainMenu()
{

}

void UI_UpdateMainMenu(int* currentScreen, WORLD_World* world, UI_MenuBackground* background)
{
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);

    for (int i = 0; i < background->starLen; i++) {
        DrawCircleV(GetScreenPos(background->starPos[i], 1), 1, WHITE);
        background->starPos[i].y -= 0.05f * SCREEN_WIDTH * ELAPSED;
        if (background->starPos[i].y < -SCREEN_HEIGHT*0.22) background->starPos[i].y += SCREEN_HEIGHT*1.44;
    }

    DrawTexturePro(TEXTURES_Tileset, TEXTURES_Name, (Rectangle){SCREEN_WIDTH/2-(SCREEN_WIDTH*0.05*11)/2, 100, SCREEN_WIDTH*0.05*11, SCREEN_WIDTH*0.05*6}, (Vector2){0, 0}, 0.0f, WHITE);

    UI_Button playButton;
    UI_CreateButtonRect(&playButton, SCREEN_WIDTH/2-SCREEN_WIDTH*0.05, SCREEN_HEIGHT/2+SCREEN_WIDTH*0.1, SCREEN_WIDTH*0.04, " play ", PINK);
    UI_DrawButton(&playButton);
    
    if (UI_IsButtonPressed(&playButton)) {
        *currentScreen = 1;
        WORLD_InitWorld(world);

        PlaySound(SOUND_GameMusic);
    }

    UI_Button exitButton;
    UI_CreateButtonRect(&exitButton, SCREEN_WIDTH/2-SCREEN_WIDTH*0.05, SCREEN_HEIGHT/2+SCREEN_WIDTH*0.2, SCREEN_WIDTH*0.04, " exit ", PINK);
    UI_DrawButton(&exitButton);
    
    if (UI_IsButtonPressed(&exitButton)) {
        *currentScreen = 1000;
    }
}
