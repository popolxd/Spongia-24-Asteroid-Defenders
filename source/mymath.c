#include "mymath.h"

#include <raylib.h>
#include <math.h>
#include <stdlib.h>

float MYMATH_RotationFromVec(Vector2 vec)
{
    float rotation;

    if (vec.x > 0) {
        rotation = atanf(vec.y / vec.x) + PI/2;
    } else if (vec.x == 0) {
        rotation = (vec.y > 0) ? PI : 0.0f;
    } else {
        rotation = atanf(vec.y / vec.x) + 3*PI/2;
    }

    return rotation;
}

int MYMATH_Colliderect(Rectangle rect1, Rectangle rect2)
{
    return !(rect1.x + rect1.width < rect2.x || rect1.x > rect2.x + rect2.width || rect1.y + rect1.height < rect2.y || rect1.y > rect2.y + rect2.height);
}

int MYMATH_PointInsideRect(Vector2 point, Rectangle rect)
{
    return !(point.x > rect.x + rect.width || point.x < rect.x || point.y > rect.y + rect.height || point.y < rect.y);
}

Vector2 MYMATH_NormalizeVec(Vector2 vec) 
{
    float denom = sqrtf(vec.x*vec.x + vec.y*vec.y);
    return (Vector2){vec.x / denom, vec.y / denom};
}

int MYMATH_RandInt(int lowerInclusive, int upperExclusive)
{
    return lowerInclusive + (upperExclusive - lowerInclusive)*((float)rand() / RAND_MAX);
}

int MYMATH_IsRayRectCollision(Vector2 pos, Vector2 dir, Rectangle rect)
{
    float txmax, txmin, tymax, tymin;
    if (dir.x == 0) {
        if (pos.x < rect.x || pos.x < rect.x + rect.width) return 0;
        txmin = 0.0f; txmax = INFINITY;
    } else if (dir.x > 0) {
        txmin = (rect.x - pos.x) / dir.x;
        txmax = (rect.x + rect.width - pos.x) / dir.x;
    } else {
        txmin = (rect.x + rect.width - pos.x) / dir.x;
        txmax = (rect.x - pos.x) / dir.x;
    }
    if (txmax < 0) return 0;

    if (dir.y == 0) {
        if (pos.y < rect.y || pos.y < rect.y + rect.height) return 0;
        tymin = 0.0f; tymax = INFINITY;
    } else if (dir.y > 0) {
        tymin = (rect.y - pos.y) / dir.y;
        tymax = (rect.y + rect.height - pos.y) / dir.y;
    } else {
        tymin = (rect.y + rect.height - pos.y) / dir.y;
        tymax = (rect.y - pos.y) / dir.y;
    }
    if (tymax < 0) return 0;

    if (txmin > tymax || tymin > txmax) return 0;
    return 1;
}

// float MYMATH_MovingPointRectCollision(Vector2 point, Vector2 vel, Rectangle rect) // (0 - 1) means collision
// {
//     Rectangle broadphase;
//     if (vel.x > 0) {
//         broadphase.x = point.x;
//         broadphase.width = vel.x;
//     } else {
//         broadphase.x = point.x + vel.x;
//         broadphase.width = -vel.x;
//     }
//     if (vel.y > 0) {
//         broadphase.y = point.y;
//         broadphase.height = vel.y;
//     } else {
//         broadphase.y = point.y + vel.y;
//         broadphase.height = -vel.y;
//     }
//     if (!MYMATH_Colliderect(broadphase, rect)) return 2.0f;

//     float xEntryTime;
//     if (vel.x == 0) {
//         xEntryTime = 2.0f;
//     } else if (vel.x > 0){
//         xEntryTime = (rect.x - point.x) / vel.x;
//     } else {
//         xEntryTime = (rect.x + rect.width - point.x) / vel.x;
//     }
//     if (xEntryTime < 0) xEntryTime = 2.0f;

//     float yEntryTime;
//     if (vel.y == 0) {
//         yEntryTime = 2.0f;
//     } else if (vel.y > 0) {
//         yEntryTime = (rect.y - point.y) / vel.y; 
//     } else {
//         yEntryTime = (rect.y + rect.height - point.y) / vel.y;
//     }
//     if (yEntryTime < 0) yEntryTime = 2.0f;

//     return fminf(xEntryTime, yEntryTime);
// }