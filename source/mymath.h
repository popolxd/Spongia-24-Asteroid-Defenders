#ifndef MYMATH_H
#define MYMATH_H

#include <raylib.h>

#define GetScreenLocWShake(rec, wsh, cs) (Rectangle){(rec.x - SCREEN_WIDTH/2 + wsh.x)*cs + SCREEN_WIDTH/2, (rec.y - SCREEN_HEIGHT/2 + wsh.y)*cs + SCREEN_HEIGHT/2, rec.width*cs, rec.height*cs}
#define GetScreenLoc(rec, cs) (Rectangle){(rec.x - SCREEN_WIDTH/2)*cs + SCREEN_WIDTH/2, (rec.y - SCREEN_HEIGHT/2)*cs + SCREEN_HEIGHT/2, rec.width*cs, rec.height*cs}
#define GetScreenPosWShake(vec, wsh, cs) (Vector2){(vec.x - SCREEN_WIDTH/2 + wsh.x)*cs + SCREEN_WIDTH/2, (vec.y - SCREEN_HEIGHT/2 + wsh.y)*cs + SCREEN_HEIGHT/2}
#define GetScreenPos(vec, cs) (Vector2){(vec.x - SCREEN_WIDTH/2)*cs + SCREEN_WIDTH/2, (vec.y - SCREEN_HEIGHT/2)*cs + SCREEN_HEIGHT/2}

#define GetWorldPosWShake(vec, wsh, cs) (Vector2){(vec.x - SCREEN_WIDTH/2) / cs + SCREEN_WIDTH/2 - wsh.x, (vec.y - SCREEN_HEIGHT/2) / cs + SCREEN_HEIGHT/2 - wsh.y}

float MYMATH_RotationFromVec(Vector2 vec);
int MYMATH_Colliderect(Rectangle rect1, Rectangle rect2);
Vector2 MYMATH_NormalizeVec(Vector2 vec);
int MYMATH_PointInsideRect(Vector2 point, Rectangle rect);
int MYMATH_RandInt(int lowerInclusive, int upperExclusive);
// float MYMATH_MovingPointRectCollision(Vector2 point, Vector2 vel, Rectangle rect); // maybe later

int MYMATH_IsRayRectCollision(Vector2 pos, Vector2 dir, Rectangle rect);

#endif