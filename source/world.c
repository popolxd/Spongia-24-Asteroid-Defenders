#include "world.h"
#include "main.h"
#include "projectiles.h"
#include "textures.h"

#include <raylib.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

float peopleSize = 0.02f;
char* names[] = {"", "Chaingun", "Laser", "Blaster", "Generator", "Heal", "Mine", "Printer", "Main Building", ""};
float eventTimes[] = {10.0f, 150.0f, 20.0f, 150.0f};

void WORLD_InitWorld(WORLD_World* world)
{
    // inner stuff
    world->stone = 40.0f;
    world->iron = 0.0f;
    world->elLeft = 2;

    WORLD_InitRoids(world);

    world->projectilesLen = 1;
    world->projectiles = (PROJECTILES_Projectile*)malloc(sizeof(PROJECTILES_Projectile)*world->projectilesLen);
    world->projectiles[0].type = DEAD;

    world->entitiesLen = 1;
    world->entities = (ENTITIES_Entity*)malloc(sizeof(ENTITIES_Entity)*world->entitiesLen);
    world->entities[0].type = KILLED;

    world->shipsLen = 1;
    world->ships = (ENTITIES_Ship*)malloc(sizeof(ENTITIES_Ship)*world->shipsLen);
    world->ships[0].type = DESTROYED;
    world->ships[0].entities = (ENTITIES_Entity*)malloc(10);
    world->ships[0].entitiesLen = 0;

    // ENTITIES_Ship test;
    // ENTITY_InitShip(&test, JET);
    // ENTITY_CreateShip(&test, &world->ships, &world->shipsLen);

    // ENTITIES_Ship test2;
    // ENTITY_InitShip(&test2, JET, (Vector2){0.2*SCREEN_WIDTH - shipSizes[JET].x/2, SCREEN_HEIGHT});
    // ENTITY_CreateShip(&test2, &world->ships, &world->shipsLen);

    // outer stuff
    int gridWorldSize = SCREEN_WIDTH/3;
    world->gridWorldLoc = (Rectangle){SCREEN_WIDTH/2 - gridWorldSize/2, SCREEN_HEIGHT/2 - (gridWorldSize * (float)world->gridHeight / world->gridWidth)/2, gridWorldSize, gridWorldSize * (float)world->gridHeight / world->gridWidth};

    world->worldShake = (Vector2){0, 0};
    world->cameraScale = 1.0f; // zooming towards center of screen

    world->ts = world->gridWorldLoc.width / world->gridWidth;

    world->buildingToPlace = NOTHING;

    world->sidePanelXPos = SCREEN_WIDTH*0.91;
    world->useXRayView = 0;

    // people
    world->peopleLen = 2;
    world->people = (BUILDINGS_Person*)malloc(sizeof(BUILDINGS_Person)*world->peopleLen);

    for (int i = 0; i < world->peopleLen; i++) {
        world->people[i].hp = peopleMaxHp;

        world->people[i].xPos = world->xTarg, world->people[i].yPos = world->yTarg;
        world->people[i].worldPos = (Vector2){
            world->gridWorldLoc.x + world->xTarg*world->ts + MYMATH_RandInt(0, world->ts - peopleSize*SCREEN_WIDTH), 
            world->gridWorldLoc.y + world->yTarg*world->ts + MYMATH_RandInt(0, world->ts - peopleSize*SCREEN_WIDTH)
        };
    }
    world->selectedPerson = -1;

    // stars
    world->starLen = SCREEN_WIDTH * SCREEN_HEIGHT / 1050;
    world->starPos = (Vector2*)malloc(sizeof(Vector2)*world->starLen);

    for (int i = 0; i < world->starLen; i++) {
        world->starPos[i] = (Vector2){
            MYMATH_RandInt(-SCREEN_WIDTH*0.22, SCREEN_WIDTH*1.22),
            MYMATH_RandInt(-SCREEN_HEIGHT*0.22, SCREEN_HEIGHT*1.22)
        };
    }

    //
    world->curEvent = 0;
    world->timer = 0.0f;
    world->timer2 = 0.0f;
    world->timer3 = 0.0f;

    world->difficultyScale = 1.0f;
}

void WORLD_DrawWorld(WORLD_World* world, int* currentScreen)
{
    // stars
    for (int i = 0; i < world->starLen; i++) {
        DrawCircleV(GetScreenPos(world->starPos[i], world->cameraScale), 1.0f*world->cameraScale, WHITE);
    }

    Rectangle correctTarget;
    if (world->roidChosen == 0) correctTarget = (Rectangle){world->gridWorldLoc.x, world->gridWorldLoc.y - world->ts, world->ts*7, world->ts*6};
    else if (world->roidChosen == 1) correctTarget = (Rectangle){world->gridWorldLoc.x - world->ts, world->gridWorldLoc.y - world->ts, world->ts*9, world->ts*6};
    else if (world->roidChosen == 2) correctTarget = (Rectangle){world->gridWorldLoc.x - world->ts, world->gridWorldLoc.y - world->ts, world->ts*8, world->ts*6};

    DrawTexturePro(TEXTURES_Tileset, TEXTURES_Roids[world->roidChosen], GetScreenLocWShake(correctTarget, world->worldShake, world->cameraScale), (Vector2){0, 0}, 0.0f, WHITE);

    for (int i = 0; i < world->gridHeight; i++) {
        for (int j = 0; j < world->gridWidth; j++) {
            Rectangle worldLoc = (Rectangle){world->gridWorldLoc.x + j*world->ts, world->gridWorldLoc.y + i*world->ts, world->ts, world->ts};
            BUILDINGS_DrawBuilding(&world->grid[i][j], GetScreenLocWShake(worldLoc, world->worldShake, world->cameraScale));
        }
    }

    for (int i = 0; i < world->entitiesLen; i++) {
        ENTITIES_DrawEntity(&world->entities[i], world->worldShake, world->cameraScale);
    }

    for (int i = 0; i < world->shipsLen; i++) {
        ENITITIES_DrawShip(&world->ships[i], world->cameraScale);
    }

    for (int i = 0; i < world->projectilesLen; i++) {
        PROJECTILES_DrawProjectile(&world->projectiles[i], world->cameraScale);
    }

    if (world->useXRayView) { // kreslim osoby

        DrawRectangle(0, 0, world->sidePanelXPos, SCREEN_HEIGHT, (Color){20, 20, 20, 140});

        for (int i = 0; i < world->peopleLen; i++) {
            if (world->people[i].hp <= 0) continue;

            if (world->selectedPerson == i) {
                Rectangle highlightRect = (Rectangle){world->people[i].worldPos.x - peopleSize*SCREEN_WIDTH*0.1, world->people[i].worldPos.y - peopleSize*SCREEN_WIDTH*0.1, peopleSize*SCREEN_WIDTH*1.2, peopleSize*SCREEN_WIDTH*1.2};
                DrawRectangleRec(GetScreenLocWShake(highlightRect, world->worldShake, world->cameraScale), BLACK);
            }

            Color personColor = (Color){(float)(peopleMaxHp - world->people[i].hp) / peopleMaxHp * 255, (float)world->people[i].hp / peopleMaxHp * 255, 0, 255};
            Rectangle worldLoc = (Rectangle){world->people[i].worldPos.x, world->people[i].worldPos.y, peopleSize*SCREEN_WIDTH, peopleSize*SCREEN_WIDTH};

            DrawTexturePro(TEXTURES_Tileset, TEXTURES_Person, GetScreenLocWShake(worldLoc, world->worldShake, world->cameraScale), (Vector2){0, 0}, 0.0f, personColor);
            // DrawRectangleRec(GetScreenLocWShake(worldLoc, world->worldShake, world->cameraScale), personColor);
        }

    }

    Rectangle sidePanel = (Rectangle){SCREEN_WIDTH*0.91, 0, SCREEN_WIDTH*0.12, SCREEN_HEIGHT};
    DrawRectangleRec(sidePanel, (Color){15,15,15,255}); // side pannel
    Rectangle sidePanelEdge = (Rectangle){SCREEN_WIDTH*0.9098, 0, SCREEN_WIDTH*0.002, SCREEN_HEIGHT};
    DrawRectangleRec(sidePanelEdge, (Color){54, 167, 66,255}); // side pannel

    // checking bulding type
    Vector2 mousePos = GetMousePosition();
    Vector2 worldMousePos = GetWorldPosWShake(mousePos, world->worldShake, world->cameraScale);
    int xIndex = (worldMousePos.x - world->gridWorldLoc.x) / world->ts;
    int yIndex = (worldMousePos.y - world->gridWorldLoc.y) / world->ts;

    if (!(xIndex < 0 || xIndex >= world->gridWidth || yIndex < 0 || yIndex >= world->gridHeight) && world->grid[yIndex][xIndex].type != NOTHING && world->grid[yIndex][xIndex].type != UNPLACABLE) {
        DrawText(TextFormat("%s", names[world->grid[yIndex][xIndex].type]), SCREEN_WIDTH*0.02, SCREEN_HEIGHT*0.95, SCREEN_HEIGHT*0.04, WHITE);
    }

    if (!world->useXRayView) {
        for (int i = 1; i < 8; i++) {
            if (MYMATH_PointInsideRect(worldMousePos, BuildingRect(i))) {
                DrawText(TextFormat("%s", names[i]), world->sidePanelXPos - SCREEN_WIDTH*0.09, SCREEN_HEIGHT*0.9, SCREEN_HEIGHT*0.025, WHITE);
                DrawText(TextFormat("%d", (int)buildingCosts[i].x), world->sidePanelXPos - SCREEN_WIDTH*0.09, SCREEN_HEIGHT*0.95, SCREEN_HEIGHT*0.025, WHITE);
                DrawText(TextFormat("%d", (int)buildingCosts[i].y), world->sidePanelXPos - SCREEN_WIDTH*0.05, SCREEN_HEIGHT*0.95, SCREEN_HEIGHT*0.025, WHITE);

                DrawTexturePro(TEXTURES_Tileset, TEXTURES_StoneIcon, (Rectangle){world->sidePanelXPos - SCREEN_WIDTH*0.08, SCREEN_HEIGHT*0.935, SCREEN_WIDTH*0.03, SCREEN_WIDTH*0.03}, (Vector2){0, 0}, 0.0f, WHITE);
                DrawTexturePro(TEXTURES_Tileset, TEXTURES_IronIcon, (Rectangle){world->sidePanelXPos - SCREEN_WIDTH*0.04, SCREEN_HEIGHT*0.94, SCREEN_WIDTH*0.025, SCREEN_WIDTH*0.025}, (Vector2){0, 0}, 0.0f, WHITE);
                break;
            }
        }
    }

    if (world->buildingToPlace != NOTHING){
        DrawRectangleRounded(BuildingRect(world->buildingToPlace), 0.2, 0, GREEN);
    }
    if (!world->useXRayView) {
        // DrawTexturePro(TEXTURES_Tileset, TEXTURES_GunBase, BuildingRect(CHAINGUN), (Vector2){0, 0}, 0, WHITE);
        // DrawTexturePro(TEXTURES_Tileset, TEXTURES_ChaingunHead[0], BuildingRect(CHAINGUN), (Vector2){0, 0}, 0, WHITE);
        DrawTexturePro(TEXTURES_Tileset, TEXTURES_BuildIcons[0], BuildingRect(CHAINGUN), (Vector2){0, 0}, 0, WHITE);

        // DrawTexturePro(TEXTURES_Tileset, TEXTURES_GunBase, BuildingRect(LASER), (Vector2){0, 0}, 0, WHITE);
        // DrawTexturePro(TEXTURES_Tileset, TEXTURES_LaserHead[0], BuildingRect(LASER), (Vector2){0, 0}, 0, WHITE);
        DrawTexturePro(TEXTURES_Tileset, TEXTURES_BuildIcons[1], BuildingRect(LASER), (Vector2){0, 0}, 0, WHITE);

        // DrawTexturePro(TEXTURES_Tileset, TEXTURES_GunBase, BuildingRect(TREBUCHET), (Vector2){0, 0}, 0, WHITE);
        // DrawTexturePro(TEXTURES_Tileset, TEXTURES_TrebuchetHead[0], BuildingRect(TREBUCHET), (Vector2){0, 0}, 0, WHITE);
        DrawTexturePro(TEXTURES_Tileset, TEXTURES_BuildIcons[2], BuildingRect(TREBUCHET), (Vector2){0, 0}, 0, WHITE);

        // DrawTexturePro(TEXTURES_Tileset, TEXTURES_Generator[0], BuildingRect(GENERATOR), (Vector2){0, 0}, 0, WHITE);
        DrawTexturePro(TEXTURES_Tileset, TEXTURES_BuildIcons[3], BuildingRect(GENERATOR), (Vector2){0, 0}, 0, WHITE);
        // DrawTexturePro(TEXTURES_Tileset, TEXTURES_Heal[0], BuildingRect(HEAL), (Vector2){0, 0}, 0, WHITE);
        DrawTexturePro(TEXTURES_Tileset, TEXTURES_BuildIcons[4], BuildingRect(HEAL), (Vector2){0, 0}, 0, WHITE);
        // DrawTexturePro(TEXTURES_Tileset, TEXTURES_Mine[0], BuildingRect(MINE), (Vector2){0, 0}, 0, WHITE);
        DrawTexturePro(TEXTURES_Tileset, TEXTURES_BuildIcons[5], BuildingRect(MINE), (Vector2){0, 0}, 0, WHITE);
        // DrawTexturePro(TEXTURES_Tileset, TEXTURES_Printer[0], BuildingRect(PRINTER), (Vector2){0, 0}, 0, WHITE);
        DrawTexturePro(TEXTURES_Tileset, TEXTURES_BuildIcons[6], BuildingRect(PRINTER), (Vector2){0, 0}, 0, WHITE);
    }

    // dalsa loopa lebo marek je kar
    for (int i = 0; i < world->gridHeight; i++) {
        for (int j = 0; j < world->gridWidth; j++) {
            if (world->grid[i][j].type != LASER || world->grid[i][j].isDestroyed || world->grid[i][j].timer >= 1.0f) continue;
            
            Vector2 origin = (Vector2){world->gridWorldLoc.x + j*world->ts + world->ts*0.5, world->gridWorldLoc.y + i*world->ts + world->ts*0.5};
            BUILDINGS_DrawLaser(GetScreenPosWShake(origin, world->worldShake, world->cameraScale), world->grid[i][j].orientation, world->cameraScale);
        }
    }

    // DrawRectangleRec(SwitchViewRect, DARKGRAY);
    DrawTexturePro(TEXTURES_Tileset, TEXTURES_Xray, SwitchViewRect, (Vector2){0, 0}, 0, WHITE);

    DrawTexturePro(TEXTURES_Tileset, TEXTURES_StoneIcon, (Rectangle){SCREEN_WIDTH*0.038, -SCREEN_HEIGHT*0.01, SCREEN_WIDTH*0.05, SCREEN_WIDTH*0.05}, (Vector2){0, 0}, 0.0f, WHITE);
    DrawTexturePro(TEXTURES_Tileset, TEXTURES_IronIcon, (Rectangle){SCREEN_WIDTH*0.04, SCREEN_WIDTH*0.03, SCREEN_WIDTH*0.04, SCREEN_WIDTH*0.04}, (Vector2){0, 0}, 0.0f, WHITE);
    DrawTexturePro(TEXTURES_Tileset, TEXTURES_ElectricityIcon, (Rectangle){SCREEN_WIDTH*0.04, SCREEN_WIDTH*0.06, SCREEN_WIDTH*0.04, SCREEN_WIDTH*0.04}, (Vector2){0, 0}, 0.0f, WHITE);

    DrawText(TextFormat("%d", (int)(world->stone)), SCREEN_WIDTH*0.08, SCREEN_HEIGHT*0.02, SCREEN_WIDTH*0.02, WHITE);
    DrawText(TextFormat("%d", (int)(world->iron)), SCREEN_WIDTH*0.08, SCREEN_HEIGHT*0.02 + SCREEN_WIDTH*0.03, SCREEN_WIDTH*0.02, WHITE);
    DrawText(TextFormat("%d", world->elLeft), SCREEN_WIDTH*0.08, SCREEN_HEIGHT*0.02 + SCREEN_WIDTH*0.06, SCREEN_WIDTH*0.02, WHITE);

    // printf("%f\n", eventTimes[world->curEvent]);
    const char* text = TextFormat("%s in: %d s", (world->curEvent % 2 == 0) ? "Next wave" : "Rest", (int)(eventTimes[world->curEvent] - world->timer));
    DrawText(text, SCREEN_WIDTH*0.4, SCREEN_HEIGHT*0.02, SCREEN_WIDTH*0.02, WHITE);

    // ci som nahodou neprehral
    if (world->grid[world->yTarg][world->xTarg].isDestroyed) {
        WORLD_UninitWorld(world, currentScreen);
    }
}

void WORLD_UpdateWorld(WORLD_World* world)
{
    // events
    world->timer += ELAPSED;
    if (world->timer > eventTimes[world->curEvent]) {
        world->timer = 0.0f;
        world->timer2 = 0.0f;
        world->timer3 = 0.0f;
        world->curEvent++;

        if (world->curEvent == 4) {
            world->curEvent = 2;
            world->difficultyScale *= 0.5;
        }
    }

    switch (world->curEvent) {
        case 1:
            world->timer2 += ELAPSED;
            if (world->timer2 > 14.0f * world->difficultyScale / (1.0f / eventTimes[1] * world->timer + 1.0f)) {
                ENTITIES_Entity newEnt;
                ENTITIES_EntityType type = MYMATH_RandInt(1, 5);
                float angle = MYMATH_RandInt(0, 360) * PI / 180;
                Vector2 pos = (Vector2){
                    SCREEN_WIDTH/2 + 0.72f*SCREEN_WIDTH*cosf(angle),
                    SCREEN_HEIGHT/2 + 0.72f*SCREEN_WIDTH*sinf(angle),
                };
                ENTITIES_InitEntity(&newEnt, type, pos);
                ENTITIES_CreateEntity(&newEnt, &world->entities, &world->entitiesLen);

                world->timer2 = 0.0f;
            }
            break;
        case 3:
            world->timer2 += ELAPSED;
            if (world->timer2 > 10.0f * world->difficultyScale / (1.0f / eventTimes[3] * world->timer + 1.0f)) {
                ENTITIES_Entity newEnt;
                ENTITIES_EntityType type = MYMATH_RandInt(1, 5);
                float angle = MYMATH_RandInt(0, 360) * PI / 180;
                Vector2 pos = (Vector2){
                    SCREEN_WIDTH/2 + 0.72f*SCREEN_WIDTH*cosf(angle),
                    SCREEN_HEIGHT/2 + 0.72f*SCREEN_WIDTH*sinf(angle),
                };
                ENTITIES_InitEntity(&newEnt, type, pos);
                ENTITIES_CreateEntity(&newEnt, &world->entities, &world->entitiesLen);

                world->timer2 = 0.0f;
            }

            world->timer3 += ELAPSED;
            if (world->timer3 > 30.0f / (1.0f / eventTimes[3] * world->timer + 1.0f)) {
                ENTITIES_Ship newShip;
                ENTITIES_ShipType type = MYMATH_RandInt(1, 4);
                ENTITY_InitShip(&newShip, type);
                ENTITY_CreateShip(&newShip, &world->ships, &world->shipsLen);

                world->timer3 = 0.0f;
            }

            break;
        default:
            break;
    }

    // stars
    for (int i = 0; i < world->starLen; i++) {
        world->starPos[i].x -= 0.025f * SCREEN_WIDTH * ELAPSED;
        if (world->starPos[i].x < -SCREEN_WIDTH*0.22) world->starPos[i].x += SCREEN_WIDTH*1.44;
    }

    // input
    if (IsKeyPressed(KEY_ESCAPE)) world->useXRayView = !world->useXRayView;

    Vector2 mousePos = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

        // building placement
        if (world->buildingToPlace == NOTHING ) {
            if (!world->useXRayView) {
                if (MYMATH_PointInsideRect(mousePos, BuildingRect(CHAINGUN))) world->buildingToPlace = CHAINGUN;
                else if (MYMATH_PointInsideRect(mousePos, BuildingRect(LASER))) world->buildingToPlace = LASER;
                else if (MYMATH_PointInsideRect(mousePos, BuildingRect(TREBUCHET))) world->buildingToPlace = TREBUCHET;
                else if (MYMATH_PointInsideRect(mousePos, BuildingRect(GENERATOR))) world->buildingToPlace = GENERATOR;
                else if (MYMATH_PointInsideRect(mousePos, BuildingRect(HEAL))) world->buildingToPlace = HEAL;
                else if (MYMATH_PointInsideRect(mousePos, BuildingRect(MINE))) world->buildingToPlace = MINE;
                else if (MYMATH_PointInsideRect(mousePos, BuildingRect(PRINTER))) world->buildingToPlace = PRINTER;
            }
            if (MYMATH_PointInsideRect(mousePos, SwitchViewRect)) world->useXRayView = !world->useXRayView;

        } else {
            Vector2 worldMousePos = GetWorldPosWShake(mousePos, world->worldShake, world->cameraScale);

            int xIndex = (worldMousePos.x - world->gridWorldLoc.x) / world->ts;
            int yIndex = (worldMousePos.y - world->gridWorldLoc.y) / world->ts;

            if (!(xIndex < 0 || xIndex >= world->gridWidth || yIndex < 0 || yIndex >= world->gridHeight)) {
                if ((world->grid[yIndex][xIndex].type == NOTHING || world->grid[yIndex][xIndex].isDestroyed) && world->grid[yIndex][xIndex].type != UNPLACABLE) {
                if ((xIndex > 0 && world->grid[yIndex][xIndex-1].type != NOTHING && world->grid[yIndex][xIndex-1].type != UNPLACABLE) 
                || (xIndex < world->gridWidth-1 && world->grid[yIndex][xIndex+1].type != NOTHING && world->grid[yIndex][xIndex+1].type != UNPLACABLE) 
                || (yIndex > 0 && world->grid[yIndex-1][xIndex].type != NOTHING && world->grid[yIndex-1][xIndex].type != UNPLACABLE) 
                || (yIndex < world->gridHeight-1 && world->grid[yIndex+1][xIndex].type != NOTHING && world->grid[yIndex+1][xIndex].type != UNPLACABLE)) {
                if (buildingCosts[world->buildingToPlace].x <= world->stone && buildingCosts[world->buildingToPlace].y <= world->iron) {

                    world->stone -= buildingCosts[world->buildingToPlace].x;
                    world->iron -= buildingCosts[world->buildingToPlace].y;

                    BUILDINGS_InitBuilding(&world->grid[yIndex][xIndex], world->buildingToPlace);
                    PlaySound(SOUND_Interact);
                    // special case
                    if (world->buildingToPlace == GENERATOR) world->elLeft += 3;
                    else world->elLeft -= buildingElUsed[world->buildingToPlace];

                } else { // nemas dost surovin
                    PlaySound(SOUND_Wrong);
                }
                } else { // nemozes placovat upne doriti
                    PlaySound(SOUND_Wrong);
                }
                } else { // nemozes placovat na policka kde su ine budovy
                    PlaySound(SOUND_Wrong);
                }
            }
            world->buildingToPlace = NOTHING;
        }

        // people movement
        Vector2 worldMousePos = GetWorldPosWShake(mousePos, world->worldShake, world->cameraScale);
        float realPeopleSize = peopleSize*SCREEN_WIDTH;

        if (world->selectedPerson == -1) {
            for (int i = 0; i < world->peopleLen; i++) {
                if (MYMATH_PointInsideRect(worldMousePos, (Rectangle){world->people[i].worldPos.x, world->people[i].worldPos.y, realPeopleSize, realPeopleSize})) {
                if (world->people[i].hp > 0) {
                    world->selectedPerson = i;
                    break;
                }
                }            
            }

        } else {
            int xIndex = (worldMousePos.x - world->gridWorldLoc.x) / world->ts;
            int yIndex = (worldMousePos.y - world->gridWorldLoc.y) / world->ts;

            if (!(xIndex < 0 || xIndex >= world->gridWidth || yIndex < 0 || yIndex >= world->gridHeight)) { // klikol som na grid
            if (xIndex != world->people[world->selectedPerson].xPos || yIndex != world->people[world->selectedPerson].yPos) {
            if (world->grid[yIndex][xIndex].type != NOTHING && world->grid[yIndex][xIndex].type != UNPLACABLE) {
                
                world->people[world->selectedPerson].worldPos = (Vector2){
                    world->gridWorldLoc.x + xIndex*world->ts + MYMATH_RandInt(0, world->ts - peopleSize*SCREEN_WIDTH), 
                    world->gridWorldLoc.y + yIndex*world->ts + MYMATH_RandInt(0, world->ts - peopleSize*SCREEN_WIDTH)
                };

                // special cases (generators)
                if (world->grid[world->people[world->selectedPerson].yPos][world->people[world->selectedPerson].xPos].type == GENERATOR && !world->grid[world->people[world->selectedPerson].yPos][world->people[world->selectedPerson].xPos].isDestroyed) world->elLeft -= 1;
                if (world->grid[yIndex][xIndex].type == GENERATOR && !world->grid[yIndex][xIndex].isDestroyed) world->elLeft += 1;

                world->people[world->selectedPerson].xPos = xIndex;
                world->people[world->selectedPerson].yPos = yIndex;
            } else PlaySound(SOUND_Wrong);
            } else PlaySound(SOUND_Wrong);
            } else PlaySound(SOUND_Wrong);

            world->selectedPerson = -1;
        }
    }

    // zooming
    float mouseWheelMovement = GetMouseWheelMove();
    float zoomIn = 1.1f; float zoomOut = 1.0f / 1.1f;
    
    if (mouseWheelMovement != 0) {
        if (mouseWheelMovement > 0) {
            world->cameraScale = fminf(MAX_SCALE, world->cameraScale * zoomIn);
        } else {
            world->cameraScale = fmaxf(MIN_SCALE, world->cameraScale * zoomOut);
        }
    }

    // projectiles
    for (int i = 0; i < world->projectilesLen; i++) {
        if (world->projectiles[i].type == DEAD) continue;
    
        world->projectiles[i].pos.x += world->projectiles[i].vel.x * ELAPSED; 
        world->projectiles[i].pos.y += world->projectiles[i].vel.y * ELAPSED;

        WORLD_DefaultBulletCheck(world, &world->projectiles[i]);
    }

    // ships
    for (int i = 0; i < world->shipsLen; i++) {
        ENTITIES_UpdateShip(&world->ships[i], world->grid, world->gridWorldLoc, world->gridWidth, world->gridHeight, &world->elLeft, world->people, world->peopleLen, &world->projectiles, &world->projectilesLen);
    }

    // entities
    for (int i = 0; i < world->entitiesLen; i++) {
        switch (world->entities[i].type) {
            case GREMLIN:
            case MINION:
                ENTITIES_UpdateMinionOrGremlin(&world->entities[i], world->grid, world->gridWorldLoc, world->gridWidth, world->gridHeight, &world->elLeft, world->people, world->peopleLen);
                break;
            case CRAB:
            case CENTIPEDE:
                ENTITIES_UpdateCrabOrCentipede(&world->entities[i], world->grid, world->gridWorldLoc, world->gridWidth, world->gridHeight, &world->elLeft, world->people, world->peopleLen, &world->projectiles, &world->projectilesLen);
                break;
            default:
                break;
        }
    }

    // people
    for (int i = 0; i < world->gridHeight; i++) {
        for (int j = 0; j < world->gridWidth; j++) {
            world->grid[i][j].numOfPeopleOn = 0;
        }
    }
    for (int i = 0; i < world->peopleLen; i++) {
        if (world->people[i].hp <= 0) continue;
        world->grid[world->people[i].yPos][world->people[i].xPos].numOfPeopleOn++;
    }

    // buildings
    for (int i = 0; i < world->gridHeight; i++) {
        for (int j = 0; j < world->gridWidth; j++) {
            Rectangle worldLoc = (Rectangle){world->gridWorldLoc.x + j*world->ts, world->gridWorldLoc.y + i*world->ts, world->ts, world->ts};

            switch (world->grid[i][j].type) {
                case CHAINGUN:
                    WORLD_UpdateRotationOfBuilding(world, j, i);
                    BUILDINGS_UpdateChaingun(&world->grid[i][j], worldLoc, &world->projectiles, &world->projectilesLen, &world->elLeft);
                    break;
                case LASER:
                    WORLD_UpdateRotationOfBuilding(world, j, i);
                    BUILDINGS_UpdateLaser(&world->grid[i][j], worldLoc, &world->projectiles, &world->projectilesLen, &world->elLeft);
                    if (world->grid[i][j].timer < 1.0f) WORLD_ShootLaser(world, j, i);
                    break;
                case TREBUCHET:
                    WORLD_UpdateRotationOfBuilding(world, j, i);
                    BUILDINGS_UpdateTrebuchet(&world->grid[i][j], worldLoc, &world->projectiles, &world->projectilesLen, &world->elLeft);
                    break;
                case GENERATOR:
                    BUILDINGS_UpdateGenerator(&world->grid[i][j], &world->elLeft);
                    break;
                case HEAL:
                    BUILDINGS_UpdateHeal(&world->grid[i][j], world->people, world->peopleLen, j, i, &world->elLeft);
                    break;
                case MINE:
                    BUILDINGS_UpdateMine(&world->grid[i][j], &world->stone, &world->elLeft);
                    break;
                case PRINTER:
                    BUILDINGS_UpdatePrinter(&world->grid[i][j], &world->iron, &world->elLeft);
                    break;
                case MAIN_BUILDING:
                    BUILDINGS_UpdateMainBuilding(&world->grid[i][j], &world->elLeft);
                default:
                    break;
            }
        }
    }

    if (!IsSoundPlaying(SOUND_GameMusic)) PlaySound(SOUND_GameMusic); // trochu tres, ale nejde mi hudba
}

void WORLD_DefaultBulletCheck(WORLD_World* world, PROJECTILES_Projectile* projectile)
{
    if (projectile->isFriendly) { // loopujem enemakov
        for (int i = 0; i < world->entitiesLen; i++) {
            if (world->entities[i].type == KILLED) continue;

            if (MYMATH_PointInsideRect(projectile->pos, world->entities[i].loc)) { // hit
                projectile->type = DEAD;

                world->entities[i].hp -= projectile->dmg;
                if (world->entities[i].hp <= 0) {
                    world->entities[i].type = KILLED;
                    PlaySound(SOUND_Kill);
                }
                break;
            }
        }

        for (int i = 0; i < world->shipsLen; i++) {
            if (world->ships[i].type == DESTROYED) continue;

            for (int j = 0; j < world->ships[i].entitiesLen; j++) {
                if (world->ships[i].entities[j].type == KILLED) continue;

                Rectangle zabima = (Rectangle){world->ships[i].entities[j].loc.x + world->ships[i].loc.x, world->ships[i].entities[j].loc.y + world->ships[i].loc.y, world->ships[i].entities[j].loc.width, world->ships[i].entities[j].loc.height};
                if (MYMATH_PointInsideRect(projectile->pos, zabima)) { // hit
                    projectile->type = DEAD;

                    world->ships[i].entities[j].hp -= projectile->dmg;
                    if (world->ships[i].entities[j].hp <= 0) {
                        world->ships[i].entities[j].type = KILLED;
                        PlaySound(SOUND_Destruction);

                        int allGunsFucked = 1; // if I should destroy ship
                        for (int k = 0; k < world->ships[i].entitiesLen; k++) {
                            if (world->ships[i].entities[k].type != KILLED) {
                                allGunsFucked = 0;
                                break;
                            }
                        }

                        if (allGunsFucked) world->ships[i].type = DESTROYED;

                        i = world->shipsLen;
                        break;
                    }
                }
            }
        }

    } else { // budovy

        int xGridPos = (projectile->pos.x - world->gridWorldLoc.x) / world->ts;
        int yGridPos = (projectile->pos.y - world->gridWorldLoc.y) / world->ts;
        
        if (xGridPos >= 0 && xGridPos < world->gridWidth && yGridPos >= 0 && yGridPos < world->gridHeight && world->grid[yGridPos][xGridPos].type != NOTHING && world->grid[yGridPos][xGridPos].type != UNPLACABLE && world->grid[yGridPos][xGridPos].hp > 0) { // hit
            projectile->type = DEAD;

            world->grid[yGridPos][xGridPos].hp -= projectile->dmg;
            if (world->grid[yGridPos][xGridPos].hp <= 0) {
                world->grid[yGridPos][xGridPos].hp = 0;

                if (!world->grid[yGridPos][xGridPos].isDestroyed) {
                    world->grid[yGridPos][xGridPos].isDestroyed = 1;
                    if (world->grid[yGridPos][xGridPos].type == GENERATOR) world->elLeft -= 3; // special case
                    else world->elLeft += buildingElUsed[world->grid[yGridPos][xGridPos].type];

                    PlaySound(SOUND_Destruction);
                }
            }

            if (!world->grid[yGridPos][xGridPos].isDestroyed) PlaySound(SOUND_Hit);

            for (int i = 0; i < world->peopleLen; i++) {
                if (world->people[i].hp <= 0) continue;

                if (world->people[i].xPos == xGridPos && world->people[i].yPos == yGridPos) {
                    world->people[i].hp -= projectile->dmg;

                    if (world->people[i].hp < 0) {
                        world->people[i].hp = 0;
                        PlaySound(SOUND_Kill);
                    }
                }
            }
        }
    }
}

void WORLD_UpdateRotationOfBuilding(WORLD_World* world, int xPos, int yPos)
{
    Vector2 buildCentPos = (Vector2){world->gridWorldLoc.x + xPos*world->ts + world->ts/2, world->gridWorldLoc.y + yPos*world->ts + world->ts/2};
    float maxSqDist = SCREEN_WIDTH * SCREEN_WIDTH * 0.16;

    int needToChange = 0;
    if (world->grid[yPos][xPos].isTargetingShip) {

        if (world->grid[yPos][xPos].target == -1 || world->grid[yPos][xPos].shipTarget == -1) needToChange = 1;
        else {
            ENTITIES_Ship targetShip = world->ships[world->grid[yPos][xPos].shipTarget];
            ENTITIES_Entity targetEntity = world->ships[world->grid[yPos][xPos].shipTarget].entities[world->grid[yPos][xPos].target];

            if (targetShip.type == DESTROYED || targetEntity.type == KILLED) needToChange = 1;
            else {
                Vector2 dirTargVec = (Vector2){targetEntity.loc.x + targetShip.loc.x + targetEntity.loc.width - buildCentPos.x, targetEntity.loc.y + targetShip.loc.y + targetEntity.loc.height - buildCentPos.y};
                float distSq = dirTargVec.x*dirTargVec.x + dirTargVec.y*dirTargVec.y;

                if (distSq > maxSqDist) needToChange = 1;
            }
        }

    } else {
        if (world->grid[yPos][xPos].target == -1 || world->entities[world->grid[yPos][xPos].target].type == KILLED) needToChange = 1;
    }


    if (needToChange) { // musim zmenit target
        float minSqDist = 10000000000.0f;
        world->grid[yPos][xPos].target = -1;

        for (int i = 0; i < world->entitiesLen; i++) {
            if (world->entities[i].type == KILLED) continue;
            Vector2 dirVec = (Vector2){world->entities[i].loc.x + world->entities[i].loc.width/2 - buildCentPos.x, world->entities[i].loc.y + world->entities[i].loc.height/2 - buildCentPos.y};

            float sqDist = dirVec.x*dirVec.x + dirVec.y*dirVec.y;
            if (sqDist < minSqDist && sqDist < maxSqDist) {
                minSqDist = sqDist;
                world->grid[yPos][xPos].isTargetingShip = 0;
                world->grid[yPos][xPos].target = i;
            }
        }

        for (int i = 0; i < world->shipsLen; i++) {
            if (world->ships[i].type == DESTROYED) continue;

            for (int j = 0; j < world->ships[i].entitiesLen; j++) {
                if (world->ships[i].entities[j].type == KILLED) continue;
                Vector2 dirVec = (Vector2){world->ships[i].entities[j].loc.x + world->ships[i].loc.x + world->ships[i].entities[j].loc.width/2 - buildCentPos.x, world->ships[i].entities[j].loc.y + world->ships[i].loc.y + world->ships[i].entities[j].loc.height/2 - buildCentPos.y};
            
                float sqDist = dirVec.x*dirVec.x + dirVec.y*dirVec.y;
                if (sqDist < minSqDist && sqDist < maxSqDist) {
                    minSqDist = sqDist;
                    world->grid[yPos][xPos].isTargetingShip = 1;
                    world->grid[yPos][xPos].shipTarget = i;
                    world->grid[yPos][xPos].target = j;
                }
            }
        }

    } else {

        Vector2 dirVec;
        if (world->grid[yPos][xPos].isTargetingShip) {
            ENTITIES_Ship targetShip = world->ships[world->grid[yPos][xPos].shipTarget];
            ENTITIES_Entity targetEnemy = world->ships[world->grid[yPos][xPos].shipTarget].entities[world->grid[yPos][xPos].target];

            dirVec = (Vector2){targetEnemy.loc.x + targetShip.loc.x + targetEnemy.loc.width/2 - buildCentPos.x, targetEnemy.loc.y + targetShip.loc.y + targetEnemy.loc.height/2 - buildCentPos.y};
        
        } else {
            dirVec = (Vector2){world->entities[world->grid[yPos][xPos].target].loc.x + world->entities[world->grid[yPos][xPos].target].loc.width/2 - buildCentPos.x,
            world->entities[world->grid[yPos][xPos].target].loc.y + world->entities[world->grid[yPos][xPos].target].loc.height/2 - buildCentPos.y};
        }

        float wantedRot = MYMATH_RotationFromVec(dirVec);

        // some inefficient adjusting
        world->grid[yPos][xPos].orientation = world->grid[yPos][xPos].orientation - (int)(world->grid[yPos][xPos].orientation / (2*PI))*2*PI;
        if (wantedRot > world->grid[yPos][xPos].orientation && wantedRot - world->grid[yPos][xPos].orientation > PI) wantedRot -= 2*PI;
        else if (wantedRot < world->grid[yPos][xPos].orientation && world->grid[yPos][xPos].orientation - wantedRot > PI) wantedRot += 2*PI;

        if (wantedRot > world->grid[yPos][xPos].orientation) {
            world->grid[yPos][xPos].orientation = fminf(wantedRot, world->grid[yPos][xPos].orientation + world->grid[yPos][xPos].rotSpeed * ELAPSED);
        } else {
            world->grid[yPos][xPos].orientation = fmaxf(wantedRot, world->grid[yPos][xPos].orientation - world->grid[yPos][xPos].rotSpeed * ELAPSED);
        }
    }
}

// special case
void WORLD_ShootLaser(WORLD_World* world, int xPos, int yPos)
{
    Vector2 laserOrigin = (Vector2){world->gridWorldLoc.x + xPos*world->ts + world->ts*0.5, world->gridWorldLoc.y + yPos*world->ts + world->ts*0.5};
    Vector2 laserDir = (Vector2){sinf(world->grid[yPos][xPos].orientation), -cosf(world->grid[yPos][xPos].orientation)};

    for (int i = 0; i < world->entitiesLen; i++) {
        if (world->entities[i].type == KILLED) continue;

        if (MYMATH_IsRayRectCollision(laserOrigin, laserDir, world->entities[i].loc)) {
            world->entities[i].hp -= 36.0f * ELAPSED;
            if (world->entities[i].hp <= 0) world->entities[i].type = KILLED;
        }
    }
}

void WORLD_InitRoids(WORLD_World* world)
{
    world->roidChosen = MYMATH_RandInt(0, 3);

    if (world->roidChosen == 0) {
        world->gridWidth = 6;
        world->gridHeight = 5;

        world->grid = (BUILDINGS_Building**)malloc(sizeof(BUILDINGS_Building*)*world->gridHeight);
        for (int i = 0; i < world->gridHeight; i++) {
            world->grid[i] = (BUILDINGS_Building*)malloc(sizeof(BUILDINGS_Building)*world->gridWidth);
            for (int j = 0; j < world->gridWidth; j++) {
                world->grid[i][j].type = NOTHING;
            }
        }

        world->grid[0][0].type = UNPLACABLE;
        world->grid[0][1].type = UNPLACABLE;
        world->grid[1][0].type = UNPLACABLE;
        world->grid[2][0].type = UNPLACABLE;
        world->grid[2][5].type = UNPLACABLE;
        world->grid[3][5].type = UNPLACABLE;
        world->grid[4][0].type = UNPLACABLE;
        world->grid[4][1].type = UNPLACABLE;
        world->grid[4][2].type = UNPLACABLE;
        world->grid[4][4].type = UNPLACABLE;
        world->grid[4][5].type = UNPLACABLE;

        world->xTarg = 3; world->yTarg = 2;
        BUILDINGS_InitBuilding(&world->grid[2][3], MAIN_BUILDING);
        world->elLeft -= buildingElUsed[MAIN_BUILDING];

    } else if (world->roidChosen == 1) {
        world->gridWidth = 7;
        world->gridHeight = 4;

        world->grid = (BUILDINGS_Building**)malloc(sizeof(BUILDINGS_Building*)*world->gridHeight);
        for (int i = 0; i < world->gridHeight; i++) {
            world->grid[i] = (BUILDINGS_Building*)malloc(sizeof(BUILDINGS_Building)*world->gridWidth);
            for (int j = 0; j < world->gridWidth; j++) {
                world->grid[i][j].type = NOTHING;
            }
        }

        world->grid[0][0].type = UNPLACABLE;
        world->grid[0][4].type = UNPLACABLE;
        world->grid[0][5].type = UNPLACABLE;
        world->grid[0][6].type = UNPLACABLE;
        world->grid[1][5].type = UNPLACABLE;
        world->grid[1][6].type = UNPLACABLE;
        world->grid[2][5].type = UNPLACABLE;
        world->grid[2][6].type = UNPLACABLE;
        world->grid[3][0].type = UNPLACABLE;

        world->xTarg = 2; world->yTarg = 2;
        BUILDINGS_InitBuilding(&world->grid[2][2], MAIN_BUILDING);
        world->elLeft -= buildingElUsed[MAIN_BUILDING];

    } else if (world->roidChosen == 2) {
        world->gridWidth = 6;
        world->gridHeight = 4;

        world->grid = (BUILDINGS_Building**)malloc(sizeof(BUILDINGS_Building*)*world->gridHeight);
        for (int i = 0; i < world->gridHeight; i++) {
            world->grid[i] = (BUILDINGS_Building*)malloc(sizeof(BUILDINGS_Building)*world->gridWidth);
            for (int j = 0; j < world->gridWidth; j++) {
                world->grid[i][j].type = NOTHING;
            }
        }

        world->grid[0][0].type = UNPLACABLE;
        world->grid[0][1].type = UNPLACABLE;
        world->grid[0][2].type = UNPLACABLE;
        world->grid[0][3].type = UNPLACABLE;
        world->grid[0][5].type = UNPLACABLE;
        world->grid[2][0].type = UNPLACABLE;
        world->grid[2][1].type = UNPLACABLE;
        world->grid[2][5].type = UNPLACABLE;
        world->grid[3][0].type = UNPLACABLE;
        world->grid[3][4].type = UNPLACABLE;
        world->grid[3][5].type = UNPLACABLE;

        world->xTarg = 3; world->yTarg = 2;
        BUILDINGS_InitBuilding(&world->grid[2][3], MAIN_BUILDING);
        world->elLeft -= buildingElUsed[MAIN_BUILDING];
    }
}

void WORLD_UninitWorld(WORLD_World* world, int* currentScreen)
{
    for (int i = 0; i < world->gridHeight; i++) {
        free(world->grid[i]);
    }
    free(world->grid);
    free(world->projectiles);
    for (int i = 0; i < world->shipsLen; i++) {
        free(world->ships[i].entities);
    }
    free(world->ships);
    free(world->entities);
    free(world->people);
    free(world->starPos);
    
    *currentScreen = 0;
}
