#include "buildings.h"
#include "textures.h"
#include "main.h"
#include "projectiles.h"

#include <raylib.h>
#include <math.h>
#include <stdio.h>

float peopleRepairingSpeed = 5.0f;
float peopleMaxHp = 100.0f;

float maxHps[] = {0, 70, 70, 70, 50, 50, 50, 50, 100};
float timers[] = {0.0f, 0.4f, 3.0f, 5.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};

Vector2 buildingCosts[] = {(Vector2){0, 0}, (Vector2){20, 0}, (Vector2){15, 5}, (Vector2){15, 15}, (Vector2){15, 0}, (Vector2){20, 0}, (Vector2){20, 0}, (Vector2){25, 0}, (Vector2){0, 0}};
int buildingElUsed[] = {0, 1, 1, 1, 0, 1, 1, 1, 0};

void BUILDINGS_InitBuilding(BUILDINGS_Building* building, BUILDINGS_BuildingType type)
{
    building->type = type;
    building->numOfPeopleOn = 0;
    building->hp = maxHps[type];
    building->orientation = 0.0f; // in radians
    building->timer = timers[building->type] / 3.0f;
    building->rotSpeed = 2.5f;
    building->isDestroyed = 0;
    building->usedAnimFrame = 0;

    building->isTargetingShip = 0;
    building->target = -1;
    building->shipTarget = -1;
}

void BUILDINGS_DrawBuilding(BUILDINGS_Building* building, Rectangle target)
{
    if (!building->isDestroyed) {
        switch (building->type) {
            case CHAINGUN: // 2 textury: podvozok a turreta, zalezi od orientation
                DrawTexturePro(TEXTURES_Tileset, TEXTURES_GunBase, target, (Vector2){0, 0}, 0, WHITE);
                DrawTexturePro(TEXTURES_Tileset, TEXTURES_ChaingunHead[building->usedAnimFrame], (Rectangle){target.x + target.width/2, target.y + target.height/2, target.width, target.height}, (Vector2){target.width*0.5, target.height*0.5}, building->orientation * 180.0f / PI, WHITE);
                break;
            case LASER:
                DrawTexturePro(TEXTURES_Tileset, TEXTURES_GunBase, target, (Vector2){0, 0}, 0, WHITE);
                DrawTexturePro(TEXTURES_Tileset, TEXTURES_LaserHead[building->usedAnimFrame], (Rectangle){target.x + target.width/2, target.y + target.height/2, target.width, target.height}, (Vector2){target.width*0.5, target.height*0.5}, building->orientation * 180.0f / PI, WHITE);
                break;
            case TREBUCHET:
                DrawTexturePro(TEXTURES_Tileset, TEXTURES_GunBase, target, (Vector2){0, 0}, 0, WHITE);
                DrawTexturePro(TEXTURES_Tileset, TEXTURES_TrebuchetHead[building->usedAnimFrame], (Rectangle){target.x + target.width/2, target.y + target.height/2, target.width, target.height}, (Vector2){target.width*0.5, target.height*0.5}, building->orientation * 180.0f / PI, WHITE);
                break;
            case GENERATOR:
                DrawTexturePro(TEXTURES_Tileset, TEXTURES_Generator[building->usedAnimFrame], target, (Vector2){0, 0}, 0.0f, WHITE);
                break;
            case HEAL:
                DrawTexturePro(TEXTURES_Tileset, TEXTURES_Heal[building->usedAnimFrame], target, (Vector2){0, 0}, 0.0f, WHITE);
                break;
            case MINE:
                DrawTexturePro(TEXTURES_Tileset, TEXTURES_Mine[building->usedAnimFrame], target, (Vector2){0, 0}, 0.0f, WHITE);
                break;
            case PRINTER:
                DrawTexturePro(TEXTURES_Tileset, TEXTURES_Printer[building->usedAnimFrame], target, (Vector2){0, 0}, 0.0f, WHITE);
                break;
            case MAIN_BUILDING:
                DrawTexturePro(TEXTURES_Tileset, TEXTURES_MainBuilding[building->usedAnimFrame], target, (Vector2){0, 0}, 0.0f, WHITE);
                break;
            default:
                break;
        }
    } else {
        if (building->type != NOTHING && building->type != UNPLACABLE) {
            DrawTexturePro(TEXTURES_Tileset, TEXTURES_DestroyedBuilding, target, (Vector2){0, 0}, 0.0f, WHITE);
        }
    }

    if (building->type != NOTHING && building->type != UNPLACABLE) {
        float ratio = building->hp / maxHps[building->type];
        DrawText(TextFormat("%d%%", (int)(ratio * 100)), target.x, target.y + target.height*0.8, target.height*0.2, (Color){(1 - ratio) * 255, ratio * 255, 0, 255});
    }
}

void BUILDINGS_DrawLaser(Vector2 pos, float rot, float cameraScale)
{
    float laserWidth = SCREEN_WIDTH * 0.01 * cameraScale;
    Rectangle rect = (Rectangle){pos.x, pos.y, laserWidth, DIAGONAL};
    DrawRectanglePro(rect, (Vector2){laserWidth/2, 0}, rot * 180.0f / PI + 180.0f, WHITE);
}

void BUILDINGS_UpdateChaingun(BUILDINGS_Building* building, Rectangle worldLoc, PROJECTILES_Projectile** projectiles, int* projectilesLen, int* elLeft)
{
    int neededRepair = BUILDING_Repairing(building, elLeft);
    if (building->isDestroyed) return;

    building->usedAnimFrame = 0;

    float boost = 1.0f;
    if (!neededRepair) boost += 0.25f * building->numOfPeopleOn;
    if (*elLeft < 0) boost *= 0.5;

    if (building->timer < timers[CHAINGUN] / boost) {
        building->timer += ELAPSED;

        if (building->timer < 0.1f || building->timer > timers[CHAINGUN] / boost - 0.1f) building->usedAnimFrame = 1;

    } else if (building->target != -1) {
        building->timer = 0.0f;

        PROJECTILES_Projectile newProj = (PROJECTILES_Projectile){
        .type = BULLET,
        .pos = (Vector2){worldLoc.x + worldLoc.width/2, worldLoc.y + worldLoc.height/2},
        .vel = (Vector2){sinf(building->orientation) * 0.4f * SCREEN_WIDTH, -cosf(building->orientation) * 0.4f * SCREEN_WIDTH},
        .isFriendly = 1,
        .dmg = 4,
        };

        PROJECTILES_CreateProjectile(&newProj, projectiles, projectilesLen);

        newProj = (PROJECTILES_Projectile){
            .type = BULLET,
            .pos = newProj.pos,
            .vel = (Vector2){-newProj.vel.x, -newProj.vel.y},
            .isFriendly = 1,
            .dmg = 4,
        };

        PROJECTILES_CreateProjectile(&newProj, projectiles, projectilesLen);

        PlaySound(SOUND_Chaingun);
    }
}

void BUILDINGS_UpdateLaser(BUILDINGS_Building* building, Rectangle worldLoc, PROJECTILES_Projectile** projectiles, int* projectilesLen, int* elLeft)
{
    int neededRepair = BUILDING_Repairing(building, elLeft);
    if (building->isDestroyed) return;

    building->usedAnimFrame = 0;

    float boost = 1.0f;
    if (!neededRepair) boost += 0.25f * building->numOfPeopleOn;
    if (*elLeft < 0) boost *= 0.5;

    if (building->timer < timers[LASER] / boost + 1.0f) {
        building->timer += ELAPSED * boost;

        if (building->timer < 1.0f) building->usedAnimFrame = 1;

    } else if (building->target != -1) {
        building->timer = 0;
        PlaySound(SOUND_Laser);
    }
}

void BUILDINGS_UpdateTrebuchet(BUILDINGS_Building* building, Rectangle worldLoc, PROJECTILES_Projectile** projectiles, int* projectilesLen, int* elLeft)
{
    int neededRepair = BUILDING_Repairing(building, elLeft);
    if (building->isDestroyed) return;

    building->usedAnimFrame = 0;

    float boost = 1.0f;
    if (!neededRepair) boost += 0.25f * building->numOfPeopleOn;
    if (*elLeft < 0) boost *= 0.5;

    if (building->timer < timers[TREBUCHET]) {
        building->timer += ELAPSED * boost;

        if (building->timer < 0.2f || building->timer > timers[TREBUCHET] - 0.2f) building->usedAnimFrame = 1;

    } else if (building->target != -1) {
        building->timer = 0.0f;

        PROJECTILES_Projectile newProj = (PROJECTILES_Projectile){
            .type = LASER_SHOT,
            .pos = (Vector2){worldLoc.x + worldLoc.width/2, worldLoc.y + worldLoc.width/2},
            .vel = {sinf(building->orientation) * 0.3f * SCREEN_WIDTH, -cosf(building->orientation) * 0.3f * SCREEN_WIDTH},
            .isFriendly = 1,
            .dmg = 60,
        };

        PROJECTILES_CreateProjectile(&newProj, projectiles, projectilesLen);
        PlaySound(SOUND_Trebuchet);
    }
}

void BUILDINGS_UpdateGenerator(BUILDINGS_Building* building, int* elLeft)
{
    BUILDING_Repairing(building, elLeft);
    if (building->isDestroyed) return;

    if (building->timer < timers[GENERATOR]) {
        building->timer += ELAPSED;
    } else {
        building->timer = 0.0f;
        building->usedAnimFrame = !building->usedAnimFrame;
    }
}

void BUILDINGS_UpdateHeal(BUILDINGS_Building* building, BUILDINGS_Person* people, int peopleLen, int xPos, int yPos, int* elLeft)
{
    int neededRepair = BUILDING_Repairing(building, elLeft);
    if (building->isDestroyed) return;

    if (building->timer < timers[HEAL]) {
        building->timer += ELAPSED;
    } else {
        building->timer = 0.0f;
        building->usedAnimFrame = !building->usedAnimFrame;
    }

    if (neededRepair) return;

    float boost = 1.0f;
    if (*elLeft < 0) boost = 0.5f;

    for (int i = 0; i < peopleLen; i++) {
        if (people[i].hp <= 0) continue;

        if (people[i].xPos == xPos && people[i].yPos == yPos) {
            people[i].hp += 10.0f * ELAPSED * boost;
            if (people[i].hp > peopleMaxHp) people[i].hp = peopleMaxHp;
        }
    }
}

void BUILDINGS_UpdateMine(BUILDINGS_Building* building, float* stone, int* elLeft)
{
    int neededRepair = BUILDING_Repairing(building, elLeft);
    if (building->isDestroyed) return;

    if (building->timer < timers[MINE]) {
        building->timer += ELAPSED;
    } else {
        building->timer = 0.0f;
        building->usedAnimFrame = !building->usedAnimFrame;
    }

    float boost = 1.0f;
    if (!neededRepair) boost += 0.15f * building->numOfPeopleOn;
    if (*elLeft < 0) boost *= 0.5f;

    *stone += (1.0f * boost) / (0.015f * (*stone) + 1) * ELAPSED;
}

void BUILDINGS_UpdatePrinter(BUILDINGS_Building* building, float* iron, int* elLeft)
{
    int neededRepair = BUILDING_Repairing(building, elLeft);
    if (building->isDestroyed) return;

    if (building->timer < timers[PRINTER]) {
        building->timer += ELAPSED;
    } else {
        building->timer = 0.0f;
        building->usedAnimFrame = !building->usedAnimFrame;
    }

    float boost = 1.0f;
    if (!neededRepair) boost += 0.15f * building->numOfPeopleOn;
    if (*elLeft < 0) boost *= 0.5f;

    *iron += (0.5f * boost) / (0.02f * (*iron) + 1) * ELAPSED;
}

void BUILDINGS_UpdateMainBuilding(BUILDINGS_Building* building, int* elLeft)
{
    BUILDING_Repairing(building, elLeft);
    if (building->isDestroyed) return; // tuto bude koniec hry :(

    if (building->timer < timers[PRINTER]) {
        building->timer += ELAPSED;
    } else {
        building->timer = 0.0f;
        building->usedAnimFrame = !building->usedAnimFrame;
    }
}

int BUILDING_Repairing(BUILDINGS_Building* building, int* elLeft)
{
    if (building->hp < maxHps[building->type]) {
        
        building->hp += peopleRepairingSpeed * building->numOfPeopleOn * ELAPSED;
        if (building->hp > maxHps[building->type]) {
            building->hp = maxHps[building->type];

            if (building->isDestroyed) {
                building->isDestroyed = 0;
                if (building->type == GENERATOR) *elLeft += 3;
                else *elLeft -= buildingElUsed[building->type];
            }
        }

        return 1;
    }
    return 0;
}