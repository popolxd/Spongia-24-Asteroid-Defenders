#ifndef WORLD_H
#define WORLD_H

#include "main.h"
#include "buildings.h"
#include "projectiles.h"
#include "entities.h"
#include "mymath.h"

#include <raylib.h>

#define MAX_SCALE 2.0f
#define MIN_SCALE 0.7f

typedef struct {
    // inner stuff
    int roidChosen;
    BUILDINGS_Building** grid;

    int gridWidth;
    int gridHeight;

    PROJECTILES_Projectile* projectiles;
    int projectilesLen;

    ENTITIES_Entity* entities;
    int entitiesLen;

    ENTITIES_Ship* ships;
    int shipsLen;

    float stone;
    float iron;
    int elLeft;

    BUILDINGS_Person* people;
    int peopleLen;

    Vector2* starPos;
    int starLen;

    // outer stuff
    Rectangle gridWorldLoc;
    Vector2 worldShake;
    float cameraScale;
    int ts;

    BUILDINGS_BuildingType buildingToPlace;
    int sidePanelXPos;

    int useXRayView;
    int selectedPerson;

    // odrb
    int xTarg, yTarg;
    int curEvent;

    float timer;
    float timer2;
    float timer3;

    float difficultyScale;

} WORLD_World;

void WORLD_InitWorld(WORLD_World* world);
void WORLD_DrawWorld(WORLD_World* world, int* currentScreen);
void WORLD_UpdateWorld(WORLD_World* world);

void WORLD_DefaultBulletCheck(WORLD_World* world, PROJECTILES_Projectile* projectile);
void WORLD_UpdateRotationOfBuilding(WORLD_World* world, int xPos, int yPos);

void WORLD_ShootLaser(WORLD_World* world, int xPos, int yPos);
void WORLD_InitRoids(WORLD_World* world);

void WORLD_UninitWorld(WORLD_World* world, int* currentScreen);

#define BuildingRect(ind) (Rectangle){world->sidePanelXPos + SCREEN_WIDTH*0.02, SCREEN_WIDTH*0.04 + ind*SCREEN_WIDTH*0.065, SCREEN_WIDTH*0.05, SCREEN_WIDTH*0.05}
#define BuildingHeadRect(ind) (Rectangle){world->sidePanelXPos + SCREEN_WIDTH*0.025, -SCREEN_WIDTH*0.015 + ind*SCREEN_WIDTH*0.07, SCREEN_WIDTH*0.04, SCREEN_WIDTH*0.04}

#define SwitchViewRect (Rectangle){world->sidePanelXPos + SCREEN_WIDTH*0.02, SCREEN_WIDTH*0.02, SCREEN_WIDTH*0.05, SCREEN_WIDTH*0.05}

#endif
