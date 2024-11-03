#ifndef BUILDINGS_H
#define BUILDINGS_H

#include "projectiles.h"

#include <raylib.h>

extern Vector2 buildingCosts[];
extern float peopleMaxHp;
extern int buildingElUsed[];

typedef struct {
    float hp;
    Vector2 worldPos;
    int xPos, yPos;

} BUILDINGS_Person; // co to robi tu kurva neviem

typedef enum {
    NOTHING = 0,
    CHAINGUN,
    LASER,
    TREBUCHET,
    GENERATOR,
    HEAL,
    MINE,
    PRINTER,
    MAIN_BUILDING,
    UNPLACABLE,
} BUILDINGS_BuildingType;

typedef struct {
    BUILDINGS_BuildingType type;
    int numOfPeopleOn;
    float hp;
    int isDestroyed;

    // animation
    int usedAnimFrame;

    // gun stuff
    float orientation;
    float rotSpeed;
    float timer;

    int isTargetingShip;
    int shipTarget;
    int target;
} BUILDINGS_Building;

void BUILDINGS_InitBuilding(BUILDINGS_Building* building, BUILDINGS_BuildingType type);
void BUILDINGS_DrawBuilding(BUILDINGS_Building* building, Rectangle target);

void BUILDINGS_DrawLaser(Vector2 pos, float rot, float cameraScale);

void BUILDINGS_UpdateChaingun(BUILDINGS_Building* building, Rectangle worldLoc, PROJECTILES_Projectile** projectiles, int* projectilesLen, int* elLeft);
void BUILDINGS_UpdateLaser(BUILDINGS_Building* building, Rectangle worldLoc, PROJECTILES_Projectile** projectiles, int* projectilesLen, int* elLeft);
void BUILDINGS_UpdateTrebuchet(BUILDINGS_Building* building, Rectangle worldLoc, PROJECTILES_Projectile** projectiles, int* projectilesLen, int* elLeft);

void BUILDINGS_UpdateGenerator(BUILDINGS_Building* building, int* elLeft);
void BUILDINGS_UpdateHeal(BUILDINGS_Building* building, BUILDINGS_Person* people, int peopleLen, int xPos, int yPos, int* elLeft);
void BUILDINGS_UpdateMine(BUILDINGS_Building* building, float* stone, int* elLeft);
void BUILDINGS_UpdatePrinter(BUILDINGS_Building* building, float* iron, int* elLeft);
void BUILDINGS_UpdateMainBuilding(BUILDINGS_Building* building, int* elLeft);

int BUILDING_Repairing(BUILDINGS_Building* building, int* elLeft);

#endif