#ifndef TEXTURES_H
#define TEXTURES_H

#include <raylib.h>

extern Texture2D TEXTURES_Tileset;
extern Rectangle TEXTURES_Person;

extern Rectangle TEXTURES_Roids[4];
extern Rectangle TEXTURES_GunBase;
extern Rectangle TEXTURES_DestroyedBuilding;

extern Rectangle TEXTURES_ChaingunHead[2];
extern Rectangle TEXTURES_LaserHead[2];
extern Rectangle TEXTURES_TrebuchetHead[2];

extern Rectangle TEXTURES_Generator[2];
extern Rectangle TEXTURES_Heal[2];
extern Rectangle TEXTURES_Mine[2];
extern Rectangle TEXTURES_Printer[2];
extern Rectangle TEXTURES_MainBuilding[2];

extern Rectangle TEXTURES_Minion[2];
extern Rectangle TEXTURES_Gremlin[2];
extern Rectangle TEXTURES_Crab[2];
extern Rectangle TEXTURES_Centipede[2];
extern Rectangle TEXTURES_MachineGun[2];
extern Rectangle TEXTURES_SpaceGun[2];
extern Rectangle TEXTURES_M2Gun[2];

extern Rectangle TEXTURES_CircleShip[2];
extern Rectangle TEXTURES_JetShip[2];
extern Rectangle TEXTURES_DoubleShip[2];

extern Rectangle TEXTURES_Bullet;
extern Rectangle TEXTURES_LaserShot;
extern Rectangle TEXTURES_RedSpit;
extern Rectangle TEXTURES_WhiteSpit;

extern Rectangle TEXTURES_StoneIcon;
extern Rectangle TEXTURES_IronIcon;
extern Rectangle TEXTURES_ElectricityIcon;

extern Rectangle TEXTURES_Name;
extern Rectangle TEXTURES_BuildIcons[7];
extern Rectangle TEXTURES_Xray;

#define TextureRect(x, y) (Rectangle){128*x, 128*y, 128, 128}

extern Sound SOUND_GameMusic;
extern Sound SOUND_Chaingun;
extern Sound SOUND_Laser;
extern Sound SOUND_Trebuchet;
extern Sound SOUND_Destruction;
extern Sound SOUND_Hit;
extern Sound SOUND_Kill;
extern Sound SOUND_Interact;
extern Sound SOUND_Wrong;

void TEXTURES_LoadTextures();

#endif
