#ifndef ENTITIES_H
#define ENTITIES_H

#include "buildings.h"

#include <raylib.h>

extern Vector2 shipSizes[];

typedef enum {
    KILLED = 0,
    MINION,
    GREMLIN,
    CRAB,
    CENTIPEDE,
    MACHINE_GUN,
    SPACE_GUN,
    M2
} ENTITIES_EntityType;

typedef struct {
    ENTITIES_EntityType type;
    Rectangle loc;
    float orientation; // radians
    float hp;

    int usedAnimFrame;

    float timer;
    float animationTimer;

    int xTarg, yTarg;
} ENTITIES_Entity;

typedef enum {
    DESTROYED = 0,
    CIRCLE,
    JET,
    DOUBLE,
} ENTITIES_ShipType;

typedef struct {
    ENTITIES_ShipType type;

    ENTITIES_Entity* entities;
    int entitiesLen;

    float orientation;
    int dir;
    int usedAnimFrame;
    float timer;

    Rectangle loc;
} ENTITIES_Ship;

void ENTITIES_InitEntity(ENTITIES_Entity* entity, ENTITIES_EntityType type, Vector2 pos);
void ENTITIES_CreateEntity(ENTITIES_Entity* entity, ENTITIES_Entity** entitiesArr, int* entitiesLen);
void ENTITIES_DrawEntity(ENTITIES_Entity* entity, Vector2 worldShake, float cameraScale);

void ENTITIES_UpdateMinionOrGremlin(ENTITIES_Entity* entity, BUILDINGS_Building** grid, Rectangle gridLoc, int gridWidth, int gridHeight, int* elLeft, BUILDINGS_Person* people, int peopleLen);
void ENTITIES_UpdateCrabOrCentipede(ENTITIES_Entity* entity, BUILDINGS_Building** grid, Rectangle gridLoc, int gridWidth, int gridHeight, int* elLeft, BUILDINGS_Person* people, int peopleLen, PROJECTILES_Projectile** projectiles, int* projectilesLen);
void ENTITIES_UpdateMachineOrSpaceOrM2Gun(ENTITIES_Entity* entity, Vector2 shipPos, BUILDINGS_Building** grid, Rectangle gridLoc, int gridWidth, int gridHeight, int* elLeft, BUILDINGS_Person* people, int peopleLen, PROJECTILES_Projectile** projectiles, int* projectilesLen);

void ENTITIES_SelectNewTarget(ENTITIES_Entity* entity, BUILDINGS_Building** grid, Rectangle gridLoc, int gridWidth, int gridHeight, int ts);
void ENTITY_DealContactDamage(ENTITIES_Entity* entity, BUILDINGS_Building** grid, int* elLeft, BUILDINGS_Person* people, int peopleLen);

// ship
void ENTITY_InitShip(ENTITIES_Ship* ship, ENTITIES_ShipType type);
void ENTITY_CreateShip(ENTITIES_Ship* ship, ENTITIES_Ship** shipsArr, int* shipsArrLen);
void ENITITIES_DrawShip(ENTITIES_Ship* ship, float cameraScale);

void ENTITIES_UpdateShip(ENTITIES_Ship* ship, BUILDINGS_Building** grid, Rectangle gridLoc, int gridWidth, int gridHeight, int* elLeft, BUILDINGS_Person* people, int peopleLen, PROJECTILES_Projectile** projectiles, int* projectilesLen);

#endif