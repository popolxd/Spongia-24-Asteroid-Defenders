#include "projectiles.h"
#include "textures.h"
#include "mymath.h"
#include "main.h"

#include <stdlib.h>

float bulletSizes[] = {0.0f, 0.015f, 0.035f, 0.02f, 0.02f}; // all of bullets are square for now, and purely cosmetic

void PROJECTILES_CreateProjectile(PROJECTILES_Projectile* projectile, PROJECTILES_Projectile** projectileArr, int* projectileArrLen)
{
    int found = 0;
    
    for (int i = 0; i < *projectileArrLen; i++) {
        if ((*projectileArr)[i].type == DEAD) { // mozem replacenut
            found = 1;
            (*projectileArr)[i] = *projectile;
            break;
        }
    }

    if (!found) {
        *projectileArr = (PROJECTILES_Projectile*)realloc(*projectileArr, sizeof(PROJECTILES_Projectile)*(*projectileArrLen + 1));
        (*projectileArr)[*projectileArrLen] = *projectile;
        (*projectileArrLen)++;
    }
}

void PROJECTILES_DrawProjectile(PROJECTILES_Projectile* projectile, float cameraScale)
{
    Rectangle worldLoc = (Rectangle){projectile->pos.x, projectile->pos.y, bulletSizes[projectile->type]*SCREEN_WIDTH, bulletSizes[projectile->type]*SCREEN_WIDTH};
    Rectangle screenLoc = GetScreenLoc(worldLoc, cameraScale);

    switch (projectile->type) {
        case BULLET:
            DrawTexturePro(TEXTURES_Tileset, TEXTURES_Bullet, screenLoc, (Vector2){screenLoc.width/2, screenLoc.height/2}, MYMATH_RotationFromVec(projectile->vel)*180.0f/PI, WHITE);
            break;
        case LASER_SHOT:
            DrawTexturePro(TEXTURES_Tileset, TEXTURES_LaserShot, screenLoc, (Vector2){screenLoc.width/2, screenLoc.width/2}, MYMATH_RotationFromVec(projectile->vel)*180.0f/PI, WHITE);
            break;
        case RED_SPIT:
            DrawTexturePro(TEXTURES_Tileset, TEXTURES_RedSpit, screenLoc, (Vector2){screenLoc.width/2, screenLoc.width/2}, MYMATH_RotationFromVec(projectile->vel)*180.0f/PI, WHITE);
            break;
        case WHITE_SPIT:
            DrawTexturePro(TEXTURES_Tileset, TEXTURES_WhiteSpit, screenLoc, (Vector2){screenLoc.width/2, screenLoc.width/2}, MYMATH_RotationFromVec(projectile->vel)*180.0f/PI, WHITE);
            break;
        default:
            break;
    }
}