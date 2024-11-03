#include "textures.h"
#include <raylib.h>

Texture2D TEXTURES_Tileset;
Rectangle TEXTURES_Person;

Rectangle TEXTURES_Roids[4];
Rectangle TEXTURES_GunBase;
Rectangle TEXTURES_DestroyedBuilding;

Rectangle TEXTURES_ChaingunHead[2];
Rectangle TEXTURES_LaserHead[2];
Rectangle TEXTURES_TrebuchetHead[2];

Rectangle TEXTURES_Generator[2];
Rectangle TEXTURES_Heal[2];
Rectangle TEXTURES_Mine[2];
Rectangle TEXTURES_Printer[2];
Rectangle TEXTURES_MainBuilding[2];

Rectangle TEXTURES_Minion[2];
Rectangle TEXTURES_Gremlin[2];
Rectangle TEXTURES_Crab[2];
Rectangle TEXTURES_Centipede[2];
Rectangle TEXTURES_MachineGun[2];
Rectangle TEXTURES_SpaceGun[2];
Rectangle TEXTURES_M2Gun[2];

Rectangle TEXTURES_CircleShip[2];
Rectangle TEXTURES_JetShip[2];
Rectangle TEXTURES_DoubleShip[2];

Rectangle TEXTURES_Bullet;
Rectangle TEXTURES_LaserShot;
Rectangle TEXTURES_RedSpit;
Rectangle TEXTURES_WhiteSpit;

Rectangle TEXTURES_StoneIcon;
Rectangle TEXTURES_IronIcon;
Rectangle TEXTURES_ElectricityIcon;

Rectangle TEXTURES_Name;
Rectangle TEXTURES_BuildIcons[7];
Rectangle TEXTURES_Xray;

Sound SOUND_GameMusic;
Sound SOUND_Chaingun;
Sound SOUND_Laser;
Sound SOUND_Trebuchet;
Sound SOUND_Destruction;
Sound SOUND_Hit;
Sound SOUND_Kill;
Sound SOUND_Interact;
Sound SOUND_Wrong;

void TEXTURES_LoadTextures()
{
    TEXTURES_Tileset = LoadTexture("resources/tileset.png");
    TEXTURES_Person = (Rectangle){2.5*128, 5.5*128, 64, 64};

    TEXTURES_Roids[0] = (Rectangle){2*128, 7*128, 7*128, 6*128};
    TEXTURES_Roids[1] = (Rectangle){0, 13*128, 9*128, 6*128};
    TEXTURES_Roids[2] = (Rectangle){0, 19*128, 8*128, 6*128};
    TEXTURES_GunBase = TextureRect(0, 4);
    TEXTURES_DestroyedBuilding = TextureRect(1, 4);

    TEXTURES_ChaingunHead[0] = TextureRect(2, 4);
    TEXTURES_ChaingunHead[1] = TextureRect(3, 4);
    TEXTURES_LaserHead[0] = TextureRect(2, 3);
    TEXTURES_LaserHead[1] = TextureRect(3, 3);
    TEXTURES_TrebuchetHead[0] = TextureRect(2, 2);
    TEXTURES_TrebuchetHead[1] = TextureRect(3, 2);

    TEXTURES_Generator[0] = TextureRect(0, 6);
    TEXTURES_Generator[1] = TextureRect(1, 6);
    TEXTURES_Heal[0] = TextureRect(0, 5);
    TEXTURES_Heal[1] = TextureRect(1, 5);
    TEXTURES_Mine[0] = TextureRect(0, 8);
    TEXTURES_Mine[1] = TextureRect(1, 8);
    TEXTURES_Printer[0] = TextureRect(0, 9);
    TEXTURES_Printer[1] = TextureRect(1, 9);
    TEXTURES_MainBuilding[0] = TextureRect(0, 7);
    TEXTURES_MainBuilding[1] = TextureRect(1, 7);

    TEXTURES_Minion[0] = TextureRect(0, 0);
    TEXTURES_Minion[1] = TextureRect(1, 0);
    TEXTURES_Gremlin[0] = TextureRect(0, 1);
    TEXTURES_Gremlin[1] = TextureRect(1, 1);
    TEXTURES_Crab[0] = TextureRect(0, 2);
    TEXTURES_Crab[1] = TextureRect(1, 2);
    TEXTURES_Centipede[0] = TextureRect(0, 3);
    TEXTURES_Centipede[1] = TextureRect(1, 3);
    TEXTURES_MachineGun[0] = TextureRect(29, 4);
    TEXTURES_MachineGun[1] = TextureRect(29, 5);
    TEXTURES_SpaceGun[0] = TextureRect(30, 4);
    TEXTURES_SpaceGun[1] = TextureRect(30, 5);
    TEXTURES_M2Gun[0] = TextureRect(31, 4);
    TEXTURES_M2Gun[1] = TextureRect(31, 5);

    TEXTURES_CircleShip[0] = TextureRect(30, 0);
    TEXTURES_CircleShip[1] = TextureRect(30, 1);
    TEXTURES_JetShip[0] = TextureRect(31, 2);
    TEXTURES_JetShip[1] = TextureRect(31, 3);
    TEXTURES_DoubleShip[0] = (Rectangle){27*128, 0, 128, 2*128};
    TEXTURES_DoubleShip[1] = (Rectangle){27*128, 2*128, 128, 2*128};

    TEXTURES_Bullet = (Rectangle){2*128, 5*128, 64, 64};
    TEXTURES_LaserShot = TextureRect(3, 5);
    TEXTURES_RedSpit = (Rectangle){2*128, 5.5*128, 64, 64};
    TEXTURES_WhiteSpit = (Rectangle){2.5*128, 5*128, 64, 64};

    TEXTURES_StoneIcon = (Rectangle){3*128, 6*128, 64, 64};
    TEXTURES_IronIcon = (Rectangle){3.5*128, 6*128, 64, 64};
    TEXTURES_ElectricityIcon = (Rectangle){3.5*128, 6.5*128, 64, 64};

    TEXTURES_Name = (Rectangle){4*128, 0, 11*128, 6*128};
    TEXTURES_BuildIcons[0] = (Rectangle){0*128,10*128, 64, 64};
    TEXTURES_BuildIcons[1] = (Rectangle){1*64,10*128, 64, 64};
    TEXTURES_BuildIcons[2] = (Rectangle){0*128,10*128+1*64, 64, 64};
    TEXTURES_BuildIcons[3] = (Rectangle){2*128+1*64,6*128, 64, 64};
    TEXTURES_BuildIcons[4] = (Rectangle){2*128,6*128, 64, 64};
    TEXTURES_BuildIcons[5] = (Rectangle){2*128,6*128+64, 64, 64};
    TEXTURES_BuildIcons[6] = (Rectangle){2*128+64,6*128+64, 64, 64};

    TEXTURES_Xray = (Rectangle){3*128, 6*128+64, 64, 64};

    SOUND_GameMusic = LoadSound("resources/spongia.mp3"); SetSoundVolume(SOUND_GameMusic, 0.5f);
    SOUND_Chaingun = LoadSound("resources/chaingun.mp3");
    SOUND_Laser = LoadSound("resources/laser.mp3");
    SOUND_Trebuchet = LoadSound("resources/trebuchet.mp3"); SetSoundVolume(SOUND_Trebuchet, 0.6f);
    SOUND_Destruction = LoadSound("resources/destruction.mp3"); SetSoundVolume(SOUND_Destruction, 0.9f);
    SOUND_Hit = LoadSound("resources/hit.mp3");
    SOUND_Kill = LoadSound("resources/kill.mp3");
    SOUND_Interact = LoadSound("resources/interact.mp3");
    SOUND_Wrong = LoadSound("resources/wrong.mp3");
}
