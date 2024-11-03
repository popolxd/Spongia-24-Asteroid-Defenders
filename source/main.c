#include "main.h"
#include "textures.h"
#include "world.h"
#include "ui.h"

#include <raylib.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int SCREEN_WIDTH;
int SCREEN_HEIGHT;
int DIAGONAL;

float ELAPSED;

int main(void)
{
	srand(time(NULL));

	SCREEN_WIDTH = 1200;
	SCREEN_HEIGHT = 1000;
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Asteroid Defenders!");
	InitAudioDevice();

	// int flags = FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE;
	// SetWindowState(flags);

	// fullscreen
	int display = GetCurrentMonitor();
	int monitorW = GetMonitorWidth(display), monitorH = GetMonitorHeight(display);
	SCREEN_WIDTH = monitorW;
	SCREEN_HEIGHT = monitorH;
	DIAGONAL = sqrtf(SCREEN_WIDTH*SCREEN_WIDTH + SCREEN_HEIGHT*SCREEN_HEIGHT);
	SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	ToggleFullscreen();

	SetTargetFPS(60);
	ELAPSED = 1;

	SetExitKey(KEY_DELETE);

	TEXTURES_LoadTextures();

	WORLD_World world;

	int currentScreen = 0;

  int run = 1;

  UI_MenuBackground menuBackground;
  menuBackground.starLen = SCREEN_WIDTH * SCREEN_HEIGHT / 1050;
  menuBackground.starPos = (Vector2*)malloc(sizeof(Vector2)*menuBackground.starLen);

  for (int i = 0; i < menuBackground.starLen; i++) {
      menuBackground.starPos[i] = (Vector2){
          MYMATH_RandInt(-SCREEN_WIDTH*0.22, SCREEN_WIDTH*1.22),
          MYMATH_RandInt(-SCREEN_HEIGHT*0.22, SCREEN_HEIGHT*1.22)
      };
  }

      
  while (run) {

    run = !WindowShouldClose();
    ELAPSED = GetFrameTime();

    
    BeginDrawing();
		ClearBackground(BLACK);

		switch (currentScreen) {
			case 0:
				UI_UpdateMainMenu(&currentScreen, &world, &menuBackground);
				// UI_DrawMainMenu();
				break;
			case 1:
				WORLD_UpdateWorld(&world);
				WORLD_DrawWorld(&world, &currentScreen);
				break;
			default:
        run = 0;
				break;
		}

		DrawText(TextFormat("%d", GetFPS()), 5, 5, 20, WHITE);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
