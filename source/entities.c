#include "entities.h"
#include "textures.h"
#include "mymath.h"
#include "main.h"
#include "buildings.h"

#include <raylib.h>
#include <stdlib.h>
#include <math.h>

float entityTimers[] = {0.0f, 1.0f, 0.9f, 2.0f, 2.0f, 0.5f, 1.0f, 2.0f};
float entityAnimTimers[] = {0.0f, 0.3f, 0.3f, 0.3f, 0.3f, 1.0f, 0.0f, 0.0};

float entitySpeeds[] = {0.0f, 0.05f, 0.08f, 0.04f, 0.05f, 0.0f, 0.0f, 0.0f};
int entityDmg[] = {0, 10, 10, 15, 15, 3, 6, 15};

float entitySizes[] = {0.0f, 0.065f, 0.065f, 0.065f, 0.065f, 0.13f, 0.13f, 0.13f};
int entityHps[] = {0, 110, 100, 100, 90, 110, 110, 110};

Vector2 shipSizes[] = {(Vector2){0.0f, 0.0f}, (Vector2){0.13f, 0.13f}, (Vector2){0.13f, 0.13f}, (Vector2){0.13f, 0.26f}};
float shipAnimTimers[] = {0.0f, 0.8f, 0.4f, 0.4f};

void ENTITIES_InitEntity(ENTITIES_Entity* entity, ENTITIES_EntityType type, Vector2 pos)
{
    entity->type = type;
    entity->loc = (Rectangle){pos.x - entitySizes[type]/2, pos.y - entitySizes[type]/2, entitySizes[type]*SCREEN_WIDTH, entitySizes[type]*SCREEN_WIDTH};
    entity->orientation = 0.0f;
    entity->hp = entityHps[type];
    entity->usedAnimFrame = 0;
    entity->timer = 0.0f;
    entity->animationTimer = 0.0f;
    entity->xTarg = -1; entity->yTarg = -1;
}

void ENTITIES_CreateEntity(ENTITIES_Entity* entity, ENTITIES_Entity** entitiesArr, int* entitiesLen)
{
    int found = 0;

    for (int i = 0; i < *entitiesLen; i++) {
        if ((*entitiesArr)[i].type == KILLED) {
            found = 1;
            (*entitiesArr)[i] = *entity;
            break;
        }
    }

    if (!found) {
        *entitiesArr = (ENTITIES_Entity*)realloc(*entitiesArr, sizeof(ENTITIES_Entity)*(*entitiesLen + 1));
        (*entitiesArr)[*entitiesLen] = *entity;
        (*entitiesLen)++;
    }
}

void ENTITIES_DrawEntity(ENTITIES_Entity* entity, Vector2 worldShake, float cameraScale)
{
    Rectangle locForDrawing = (Rectangle){entity->loc.x + entity->loc.width/2, entity->loc.y + entity->loc.height/2, entity->loc.width, entity->loc.height};
    Rectangle screenLoc = GetScreenLoc(locForDrawing, cameraScale); // for now, later need to split

    switch (entity->type) {
        case MINION:
            DrawTexturePro(TEXTURES_Tileset, TEXTURES_Minion[entity->usedAnimFrame], screenLoc, (Vector2){screenLoc.width/2, screenLoc.height/2}, entity->orientation * 180.0f / PI, WHITE);
            break;
        case GREMLIN:
            DrawTexturePro(TEXTURES_Tileset, TEXTURES_Gremlin[entity->usedAnimFrame], screenLoc, (Vector2){screenLoc.width/2, screenLoc.height/2}, entity->orientation * 180.0f / PI, WHITE);
            break;
        case CRAB:
            DrawTexturePro(TEXTURES_Tileset, TEXTURES_Crab[entity->usedAnimFrame], screenLoc, (Vector2){screenLoc.width/2, screenLoc.height/2}, entity->orientation * 180.0f / PI, WHITE);
            break;
        case CENTIPEDE:
            DrawTexturePro(TEXTURES_Tileset, TEXTURES_Centipede[entity->usedAnimFrame], screenLoc, (Vector2){screenLoc.width/2, screenLoc.height/2}, entity->orientation * 180.0f / PI, WHITE);
            break;
        case MACHINE_GUN:
            DrawTexturePro(TEXTURES_Tileset, TEXTURES_MachineGun[entity->usedAnimFrame], screenLoc, (Vector2){screenLoc.width/2, screenLoc.height/2}, entity->orientation * 180.0f / PI, WHITE);
            break;
        case SPACE_GUN:
            DrawTexturePro(TEXTURES_Tileset, TEXTURES_SpaceGun[entity->usedAnimFrame], screenLoc, (Vector2){screenLoc.width/2, screenLoc.height/2}, entity->orientation * 180.0f / PI, WHITE);
            break;
        case M2:
            DrawTexturePro(TEXTURES_Tileset, TEXTURES_M2Gun[entity->usedAnimFrame], screenLoc, (Vector2){screenLoc.width/2, screenLoc.height/2}, entity->orientation * 180.0f / PI, WHITE);
            break;
        default:
            break;
    }
}

void ENTITIES_UpdateMinionOrGremlin(ENTITIES_Entity* entity, BUILDINGS_Building** grid, Rectangle gridLoc, int gridWidth, int gridHeight, int* elLeft, BUILDINGS_Person* people, int peopleLen)
{
    int ts = gridLoc.width / gridWidth;

    // movement
    if (entity->xTarg == -1 || grid[entity->yTarg][entity->xTarg].hp == 0) { // musim selectnut novy target
        ENTITIES_SelectNewTarget(entity, grid, gridLoc, gridWidth, gridHeight, ts);
    } else {

        Vector2 targetPos = (Vector2){gridLoc.x + entity->xTarg*ts + ts/2, gridLoc.y + entity->yTarg*ts + ts/2};
        Vector2 dirVec = (Vector2){targetPos.x - entity->loc.x - entity->loc.width/2, targetPos.y - entity->loc.y - entity->loc.height/2};
        Vector2 dir = MYMATH_NormalizeVec(dirVec);

        entity->timer += ELAPSED;

        if (dirVec.x*dirVec.x + dirVec.y*dirVec.y > entity->loc.width*entity->loc.width/4) {
            entity->loc.x += dir.x * entitySpeeds[entity->type] * SCREEN_WIDTH * ELAPSED;
            entity->loc.y += dir.y * entitySpeeds[entity->type] * SCREEN_WIDTH * ELAPSED;

        } else if (entity->timer >= entityTimers[entity->type]) { // damaging
            entity->timer = 0.0f;

            ENTITY_DealContactDamage(entity, grid, elLeft, people, peopleLen);
        }
    }
}

void ENTITIES_UpdateCrabOrCentipede(ENTITIES_Entity* entity, BUILDINGS_Building** grid, Rectangle gridLoc, int gridWidth, int gridHeight, int* elLeft, BUILDINGS_Person* people, int peopleLen, PROJECTILES_Projectile** projectiles, int* projectilesLen)
{
    int ts = gridLoc.x / gridWidth;

    if (entity->animationTimer < entityAnimTimers[entity->type]) {
        entity->animationTimer += ELAPSED;
    } else {
        entity->animationTimer = 0.0f;
        entity->usedAnimFrame = !entity->usedAnimFrame;
    }

    // movement
    if (entity->xTarg == -1 || grid[entity->yTarg][entity->xTarg].hp == 0) { // musim selectnut novy target
        ENTITIES_SelectNewTarget(entity, grid, gridLoc, gridWidth, gridHeight, ts);
    } else {

        Vector2 targetPos = (Vector2){gridLoc.x + entity->xTarg*ts + ts/2, gridLoc.y + entity->yTarg*ts + ts/2};
        Vector2 dirVec = (Vector2){targetPos.x - entity->loc.x - entity->loc.width/2, targetPos.y - entity->loc.y - entity->loc.height/2};
        Vector2 dir = MYMATH_NormalizeVec(dirVec);

        entity->timer += ELAPSED;

        if (dirVec.x*dirVec.x + dirVec.y*dirVec.y > SCREEN_WIDTH*SCREEN_WIDTH * ((entity->type == CRAB) ? 0.08 : 0.05)) {
            entity->loc.x += dir.x * entitySpeeds[entity->type] * SCREEN_WIDTH * ELAPSED;
            entity->loc.y += dir.y * entitySpeeds[entity->type] * SCREEN_WIDTH * ELAPSED;

        } else if (entity->timer >= entityTimers[entity->type]) { // damaging
            entity->timer = 0.0f;

            Vector2 addition = (entity->type == CENTIPEDE) ? (Vector2){sinf(entity->orientation) * entity->loc.width/2, -cosf(entity->orientation) * entity->loc.width/2} : (Vector2){0, 0};

            PROJECTILES_Projectile newProj = (PROJECTILES_Projectile){
                .type = (entity->type == CENTIPEDE) ? WHITE_SPIT : RED_SPIT,
                .pos = (Vector2){entity->loc.x + entity->loc.width/2 + addition.x, entity->loc.y + entity->loc.height/2 + addition.y},
                .vel = (Vector2){sinf(entity->orientation) * 0.35f * SCREEN_WIDTH, -cosf(entity->orientation) * 0.35f * SCREEN_WIDTH},
                .isFriendly = 0,
                .dmg = entityDmg[entity->type]
            };

            PROJECTILES_CreateProjectile(&newProj, projectiles, projectilesLen);
        }
    }
}

void ENTITIES_UpdateMachineOrSpaceOrM2Gun(ENTITIES_Entity* entity, Vector2 shipPos, BUILDINGS_Building** grid, Rectangle gridLoc, int gridWidth, int gridHeight, int* elLeft, BUILDINGS_Person* people, int peopleLen, PROJECTILES_Projectile** projectiles, int* projectilesLen)
{
    int ts = gridLoc.x / gridWidth;

    if (entity->xTarg == -1 || grid[entity->yTarg][entity->xTarg].hp == 0) { // musim selectnut novy target
        entity->loc.x += shipPos.x; entity->loc.y += shipPos.y;
        ENTITIES_SelectNewTarget(entity, grid, gridLoc, gridWidth, gridHeight, ts);
        entity->loc.x -= shipPos.x; entity->loc.y -= shipPos.y;

    } else {

        Vector2 targetPos = (Vector2){gridLoc.x + entity->xTarg*ts + ts/2, gridLoc.y + entity->yTarg*ts + ts/2};
        Vector2 dirVec = (Vector2){targetPos.x - entity->loc.x - shipPos.x - entity->loc.width/2, targetPos.y - entity->loc.y - shipPos.y - entity->loc.height/2};
        float distSq = dirVec.x*dirVec.x + dirVec.y*dirVec.y;
        
        entity->orientation = MYMATH_RotationFromVec(dirVec);

        if (entity->timer < entityTimers[entity->type]) {
            entity->timer += ELAPSED;

            if (entity->timer < 0.05f || entity->timer > entityTimers[entity->type] - 0.05f) entity->usedAnimFrame = 1;
            else entity->usedAnimFrame = 0;
        }
        if (entity->timer > entityTimers[entity->type] && distSq < SCREEN_WIDTH * SCREEN_WIDTH * 0.16) { // shooting
            entity->timer = 0.0f;

            PROJECTILES_Projectile newProj = (PROJECTILES_Projectile){
                .type = (entity->type == SPACE_GUN) ? BULLET : RED_SPIT,
                .pos = (Vector2){entity->loc.x + shipPos.x + entity->loc.width/2 + sinf(entity->orientation)*entitySizes[entity->type]/3*SCREEN_WIDTH, entity->loc.y + shipPos.y + entity->loc.height/2 - cosf(entity->orientation)*entitySizes[entity->type]/3*SCREEN_WIDTH},
                .vel = (Vector2){0.4f * SCREEN_WIDTH * sinf(entity->orientation), 0.4f * SCREEN_WIDTH * -cosf(entity->orientation)},
                .isFriendly = 0,
                .dmg = entityDmg[entity->type],
            };
            PROJECTILES_CreateProjectile(&newProj, projectiles, projectilesLen);
            PlaySound(SOUND_Chaingun);
        }
    }

}

void ENTITIES_SelectNewTarget(ENTITIES_Entity* entity, BUILDINGS_Building** grid, Rectangle gridLoc, int gridWidth, int gridHeight, int ts)
{
    float minSqDist = 100000000000.0f;
    int curXTarg = -1, curYTarg = -1;

    for (int i = 0; i < gridHeight; i++) {
        for (int j = 0; j < gridWidth; j++) {
            if (grid[i][j].type == NOTHING || grid[i][j].type == UNPLACABLE || grid[i][j].hp == 0) continue;

            Vector2 potTargetPos = (Vector2){gridLoc.x + j*ts + ts/2, gridLoc.y + i*ts + ts/2};
            Vector2 dirVec = (Vector2){potTargetPos.x - entity->loc.x - entity->loc.width/2, potTargetPos.y - entity->loc.y - entity->loc.height/2};

            float sqDist = dirVec.x*dirVec.x + dirVec.y*dirVec.y;
            if (sqDist < minSqDist) {
                minSqDist = sqDist;
                curXTarg = j;
                curYTarg = i;

                entity->orientation = MYMATH_RotationFromVec(dirVec);
            }
        }
    }

    entity->xTarg = curXTarg;
    entity->yTarg = curYTarg;
}

void ENTITY_DealContactDamage(ENTITIES_Entity* entity, BUILDINGS_Building** grid, int* elLeft, BUILDINGS_Person* people, int peopleLen)
{
    grid[entity->yTarg][entity->xTarg].hp -= entityDmg[entity->type];

    if (grid[entity->yTarg][entity->xTarg].hp <= 0) {
        grid[entity->yTarg][entity->xTarg].hp = 0;

        if (!grid[entity->yTarg][entity->xTarg].isDestroyed) {
            grid[entity->yTarg][entity->xTarg].isDestroyed = 1;

            if (grid[entity->yTarg][entity->xTarg].type == GENERATOR) *elLeft -= 3; // special case
            else *elLeft += buildingElUsed[grid[entity->yTarg][entity->xTarg].type];

            PlaySound(SOUND_Destruction);
        }
    }

    if (!grid[entity->yTarg][entity->xTarg].isDestroyed) PlaySound(SOUND_Hit);

    for (int i = 0; i < peopleLen; i++) {
        if (people[i].hp <= 0) continue;

        if (people[i].xPos == entity->xTarg && people[i].yPos == entity->yTarg) { // musi dostat damage
            people[i].hp -= entityDmg[entity->type];

            if (people[i].hp < 0) {
                people[i].hp = 0;
                PlaySound(SOUND_Kill);
            }
        }
    }
}

void ENTITY_InitShip(ENTITIES_Ship* ship, ENTITIES_ShipType type)
{
    ship->type = type;
    ship->dir = 1;
    ship->usedAnimFrame = 0;
    ship->timer = 0.0f;

    ENTITIES_Entity newEnt; int entType;

    switch (ship->type) {
        case CIRCLE:
            ship->entitiesLen = 1;
            ship->entities = (ENTITIES_Entity*)malloc(sizeof(ENTITIES_Entity)*ship->entitiesLen);
            ship->entities[0].type = KILLED;

            ship->orientation = 0.0f;
            ship->loc = (Rectangle){-SCREEN_WIDTH*0.29 - shipSizes[CIRCLE].x*SCREEN_WIDTH, 0.15*SCREEN_HEIGHT - shipSizes[CIRCLE].y*SCREEN_WIDTH/2, shipSizes[CIRCLE].x*SCREEN_WIDTH, shipSizes[CIRCLE].y*SCREEN_WIDTH};

            entType = MYMATH_RandInt(5, 8);
            ENTITIES_InitEntity(&newEnt, entType, (Vector2){0, 0});
            ENTITIES_CreateEntity(&newEnt, &ship->entities, &ship->entitiesLen);
            break;

        case JET:
            ship->entitiesLen = 1;
            ship->entities = (ENTITIES_Entity*)malloc(sizeof(ENTITIES_Entity)*ship->entitiesLen);
            ship->entities[0].type = KILLED;

            ship->orientation = 0.0f;
            ship->loc = (Rectangle){SCREEN_WIDTH*0.2 - shipSizes[JET].x*SCREEN_WIDTH/2, SCREEN_HEIGHT*1.29, shipSizes[JET].x*SCREEN_WIDTH, shipSizes[JET].y*SCREEN_WIDTH};

            entType = MYMATH_RandInt(5, 8);
            ENTITIES_InitEntity(&newEnt, entType, (Vector2){0, -SCREEN_WIDTH*0.014});
            ENTITIES_CreateEntity(&newEnt, &ship->entities, &ship->entitiesLen);
            break;

        case DOUBLE:
            ship->entitiesLen = 2;
            ship->entities = (ENTITIES_Entity*)malloc(sizeof(ENTITIES_Entity)*ship->entitiesLen);
            ship->entities[0].type = KILLED; ship->entities[1].type = KILLED;

            ship->orientation = PI / 2;
            ship->loc = (Rectangle){-SCREEN_WIDTH*0.29 - shipSizes[DOUBLE].x*SCREEN_WIDTH, 0.15*SCREEN_HEIGHT - shipSizes[DOUBLE].y*SCREEN_WIDTH/2, shipSizes[DOUBLE].x*SCREEN_WIDTH, shipSizes[DOUBLE].y*SCREEN_WIDTH};

            entType = MYMATH_RandInt(5, 8);
            ENTITIES_InitEntity(&newEnt, entType, (Vector2){SCREEN_WIDTH*0.065, ship->loc.width/2});
            ENTITIES_CreateEntity(&newEnt, &ship->entities, &ship->entitiesLen);

            entType = MYMATH_RandInt(5, 8);
            ENTITIES_InitEntity(&newEnt, entType, (Vector2){0, ship->loc.width/2});
            ENTITIES_CreateEntity(&newEnt, &ship->entities, &ship->entitiesLen);
        default:
            break;
    }
}

void ENTITY_CreateShip(ENTITIES_Ship* ship, ENTITIES_Ship** shipsArr, int* shipsArrLen)
{
    int found = 0;

    for (int i = 0; i < *shipsArrLen; i++) {
        if ((*shipsArr)[i].type == DESTROYED) {
            found = 1;
            free((*shipsArr)[i].entities);

            (*shipsArr)[i] = *ship;
            break;
        }
    }

    if (!found) {
        *shipsArr = (ENTITIES_Ship*)realloc(*shipsArr, sizeof(ENTITIES_Ship)*(*shipsArrLen + 1));
        (*shipsArr)[*shipsArrLen] = *ship;
        (*shipsArrLen)++;
    }
}

void ENITITIES_DrawShip(ENTITIES_Ship* ship, float cameraScale)
{
    Rectangle locForDrawing = (Rectangle){ship->loc.x + ship->loc.width/2, ship->loc.y + ship->loc.height/2, ship->loc.width, ship->loc.height};
    Rectangle screenLoc = GetScreenLoc(locForDrawing, cameraScale);
    switch (ship->type) {
        case CIRCLE:
            DrawTexturePro(TEXTURES_Tileset, TEXTURES_CircleShip[ship->usedAnimFrame], screenLoc, (Vector2){screenLoc.width/2, screenLoc.height/2}, ship->orientation * 180 / PI, WHITE);
            break;
        case JET:
            DrawTexturePro(TEXTURES_Tileset, TEXTURES_JetShip[ship->usedAnimFrame], screenLoc, (Vector2){screenLoc.width/2, screenLoc.height/2}, ship->orientation * 180 / PI, WHITE);
            break;
        case DOUBLE:
            DrawTexturePro(TEXTURES_Tileset, TEXTURES_DoubleShip[ship->usedAnimFrame], screenLoc, (Vector2){screenLoc.width/2, screenLoc.height/2}, ship->orientation * 180 / PI, WHITE);
            break;
        default:
            break;
    }

    if (ship->type != DESTROYED) {
        for (int i = 0; i < ship->entitiesLen; i++) {
            Rectangle temp = ship->entities[i].loc;
            ship->entities[i].loc = (Rectangle){ship->loc.x + temp.x, ship->loc.y + temp.y, temp.width, temp.height};
            ENTITIES_DrawEntity(&ship->entities[i], (Vector2){0, 0}, cameraScale);
            ship->entities[i].loc = temp;
        }
    }
}

void ENTITIES_UpdateShip(ENTITIES_Ship* ship, BUILDINGS_Building** grid, Rectangle gridLoc, int gridWidth, int gridHeight, int* elLeft, BUILDINGS_Person* people, int peopleLen, PROJECTILES_Projectile** projectiles, int* projectilesLen)
{
    switch (ship->type) {
        case CIRCLE:
            ship->timer += ELAPSED;
            if (ship->timer > shipAnimTimers[CIRCLE]) {
                ship->usedAnimFrame = !ship->usedAnimFrame;
                ship->timer = 0.0f;
            }

            ship->loc.x += 0.13f * SCREEN_WIDTH * ELAPSED * ship->dir;
            if (ship->loc.x > SCREEN_WIDTH*1.3) {
                ship->dir = -1;
                ship->loc.y = 0.85 * SCREEN_HEIGHT - ship->loc.height/2;

                // int rand = MYMATH_RandInt(0, 5); // cleaning mechanism
                // if (rand == 0) {
                //     ship->type = DESTROYED;
                //     break;
                // }
            } else if (ship->loc.x < -SCREEN_WIDTH*0.3 - ship->loc.width) {
                ship->dir = 1;
                ship->loc.y = 0.15 * SCREEN_HEIGHT  - ship->loc.height/2;

                // int rand = MYMATH_RandInt(0, 5); // cleaning mechanism
                // if (rand == 0) {
                //     ship->type = DESTROYED;
                //     break;
                // }
            }
            break;

        case JET:
            ship->timer += ELAPSED;
            if (ship->timer > shipAnimTimers[CIRCLE]) {
                ship->usedAnimFrame = !ship->usedAnimFrame;
                ship->timer = 0.0f;
            }

            ship->loc.y -= 0.12f * SCREEN_WIDTH * ELAPSED * ship->dir;
            if (ship->loc.y > SCREEN_HEIGHT*1.3) {
                ship->dir = 1;
                ship->loc.x = 0.2 * SCREEN_WIDTH - ship->loc.width/2;
                ship->orientation = 0.0f;
                ship->entities[0].loc.y -= SCREEN_WIDTH * 0.028f;

                // int rand = MYMATH_RandInt(0, 5); // cleaning mechanism
                // if (rand == 0) {
                //     ship->type = DESTROYED;
                //     break;
                // }
            } else if (ship->loc.y < -SCREEN_HEIGHT*0.3 - ship->loc.height) {
                ship->dir = -1;
                ship->loc.x = 0.8 * SCREEN_WIDTH - ship->loc.width/2;
                ship->orientation = PI;
                ship->entities[0].loc.y += SCREEN_WIDTH * 0.028f;

                // int rand = MYMATH_RandInt(0, 5); // cleaning mechanism
                // if (rand == 0) {
                //     ship->type = DESTROYED;
                //     break;
                // }
            }
            break;

        case DOUBLE:
            ship->timer += ELAPSED;

            if (ship->timer > shipAnimTimers[DOUBLE]) {
                ship->usedAnimFrame = !ship->usedAnimFrame;
                ship->timer = 0.0f;
            }

            ship->loc.x += 0.13f * SCREEN_WIDTH * ELAPSED * ship->dir;
            if (ship->loc.x > SCREEN_WIDTH*1.3) {
                ship->dir = -1;
                ship->loc.y = 0.85 * SCREEN_HEIGHT - ship->loc.height/2;
                ship->orientation = -PI / 2;
                ship->entities[0].loc.x -= SCREEN_WIDTH * 0.13f;

                // int rand = MYMATH_RandInt(0, 5); // cleaning mechanism
                // if (rand == 0) {
                //     ship->type = DESTROYED;
                //     break;
                // }
            } else if (ship->loc.x < -SCREEN_WIDTH*0.3 - ship->loc.width) {
                ship->dir = 1;
                ship->loc.y = 0.15 * SCREEN_HEIGHT  - ship->loc.height/2;
                ship->orientation = PI / 2;
                ship->entities[0].loc.x += SCREEN_WIDTH * 0.13f;

                // int rand = MYMATH_RandInt(0, 5); // cleaning mechanism
                // if (rand == 0) {
                //     ship->type = DESTROYED;
                //     break;
                // }
            }
            break;

        default:
            break;
    }

    if (ship->type != DESTROYED) {
        for (int i = 0; i < ship->entitiesLen; i++) {
            switch (ship->entities[i].type) {
                case M2:
                case SPACE_GUN:
                case MACHINE_GUN:
                    ENTITIES_UpdateMachineOrSpaceOrM2Gun(&ship->entities[i], (Vector2){ship->loc.x, ship->loc.y}, grid, gridLoc, gridWidth, gridHeight, elLeft, people, peopleLen, projectiles, projectilesLen);
                    break;
                default:
                    break;
            }
        }
    }
}