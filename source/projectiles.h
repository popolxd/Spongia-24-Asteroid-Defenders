#ifndef PROJECTILES_H
#define PROJECTILES_H

#include "raylib.h"

typedef enum {
    DEAD = 0,
    BULLET,
    LASER_SHOT,
    RED_SPIT,
    WHITE_SPIT
} PROJECTILES_ProjectileType;

typedef struct {
    PROJECTILES_ProjectileType type;
    Vector2 pos;
    Vector2 vel;
    int isFriendly;
    int dmg;
} PROJECTILES_Projectile;

void PROJECTILES_CreateProjectile(PROJECTILES_Projectile* projectile, PROJECTILES_Projectile** projectileArr, int* projectileArrLen);
void PROJECTILES_DrawProjectile(PROJECTILES_Projectile* projectile, float cameraScale);

#endif