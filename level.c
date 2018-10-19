#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <raylib.h>

#include "level.h"
#include "enemy.h"
#include "collision_tile.h"
#include "animation.h"
#include "level_one.h"
#include "game_stage.h"
#include "res_manager.h"

extern GameStage currentStage;
extern const unsigned int SCREEN_WIDTH;
extern const unsigned int SCREEN_HEIGHT;

static void playerDestroyedAnimation(Level *level);
static void cannonDestroyedAnimation(Level *level, Entity *e);
static void bossBulletDestroyedAnimation(Level *level, Entity *e);

static void levelCollision(Level *level);
static bool boxBoxCollision(Entity *entityA, Entity *entityB);
static bool boxPlayerCollision(Player *player, Entity *entityA);
static bool playerBossCollision(Player *player, Boss *boss);
static bool playerShieldCollision(Player *player, Boss *boss);
static void drawHealthBar(Level *level);

Level* levelCreate(Player *player, unsigned int levelNo){
    Level *level = NULL;

    level = malloc(sizeof(Level));
    if(level == NULL){
        fprintf(stderr, "level malloc err...\n");
        return NULL;
    }

    level->player = player;
    level->boss = NULL;
    level->levelNo = levelNo;
    level->score = 0;
    level->hiScore = 9999; // load hiscore from somewhere <<<<<<<<<<<<<<<<<<<<<----------------------------------- TO DO
    level->start = 119.0;
    //level->start = 2.0;
    level->y = 0;
    level->lowBorder = 64.0;
    level->highBorder = 961.0;
    level->infoBorder = 96.0;
    level->scrollSpeed = 1.0;
    level->enemiesLayer = NULL;
    level->eBulletsLayer = NULL;
    level->collisionTilesVsCannonsLayer = NULL;
    level->collisionTilesVsBlocksLayer = NULL;
    level->collisionTilesVsPlayer = NULL;
    level->animationLayer = NULL;
    level->tileMap = loadTileMap("resources/tile_map/mecha_bg2_map.lua");
    level->texture = getTexture(TILE_01);

    for(int i = 0; i < level->tileMap->objectCount; i++){
        if(strcmp(level->tileMap->object[i].name, "zako") == 0){
            enemyCreate(&level->enemiesLayer, level->tileMap->object[i].id, ENEMY_ZAKO, (Vector2){level->tileMap->object[i].x, level->tileMap->object[i].y},
                        0, 0, ENEMY_ZAKO_UNIT_DAMAGE, ENEMY_ZAKO_UNIT_HEALTH, ENEMY_ZAKO_SCORE, ENEMY_03);
        }

        else if(strcmp(level->tileMap->object[i].name, "block") == 0){
            enemyCreate(&level->enemiesLayer, level->tileMap->object[i].id, ENEMY_BLOCK, (Vector2){level->tileMap->object[i].x, level->tileMap->object[i].y},
                         25, 0, ENEMY_BLOCK_UNIT_DAMAGE, ENEMY_BLOCK_UNIT_HEALTH, 0, ENEMY_BLOCK_01);
        }

        else if(strcmp(level->tileMap->object[i].name, "canon") == 0){
            TextureName textureName;
            float rotation = atof(level->tileMap->object[i].type);

            if(rotation == 0){
                textureName = ENEMY_LARGE_CANON_0;
            }

            else if(rotation == 90){
                textureName = ENEMY_LARGE_CANON_90;
            }

            else if(rotation == 180){
                textureName = ENEMY_LARGE_CANON_180;
            }

            else if(rotation == 270){
                textureName = ENEMY_LARGE_CANON_270;
            }

            enemyCreate(&level->enemiesLayer, level->tileMap->object[i].id, ENEMY_CANNON, (Vector2){level->tileMap->object[i].x, level->tileMap->object[i].y},
                        0, rotation, ENEMY_CANNON_UNIT_DAMAGE, ENEMY_CANNON_UNIT_HEALTH, ENEMY_CANNON_SCORE, textureName);
        }
    }
    // TILES VS PLAYER  , HARD CODED <<<<<<---------------------------------
    for(int i = 0; i < 468; i++){
        collisionTileCreate(&level->collisionTilesVsPlayer, level->tileMap->layer[1].collTiles[i].id, COLLISION_TILE,
                            level->tileMap->tileSet->tileWidth, level->tileMap->tileSet->tileHeight, level->tileMap->layer[1].collTiles[i].index, false);
    }
    free(level->tileMap->layer[1].collTiles);

    // TILES VS ENEMY CANNON BULETS
    //  1.
    collisionTileCreate(&level->collisionTilesVsCannonsLayer, (2 + (64 * level->tileMap->layer[1].width)), COLLISION_TILE,
                                    level->tileMap->tileSet->tileWidth, level->tileMap->tileSet->tileHeight, -1, false);
    collisionTileCreate(&level->collisionTilesVsCannonsLayer, (2 + (65 * level->tileMap->layer[1].width)), COLLISION_TILE,
                                    level->tileMap->tileSet->tileWidth, level->tileMap->tileSet->tileHeight, -1, false);
    //  2.
    collisionTileCreate(&level->collisionTilesVsCannonsLayer, (2 + (82 * level->tileMap->layer[1].width)), COLLISION_TILE,
                                    level->tileMap->tileSet->tileWidth, level->tileMap->tileSet->tileHeight, -1, false);
    collisionTileCreate(&level->collisionTilesVsCannonsLayer, (2 + (83 * level->tileMap->layer[1].width)), COLLISION_TILE,
                                    level->tileMap->tileSet->tileWidth, level->tileMap->tileSet->tileHeight, -1, false);
    //  3.
    collisionTileCreate(&level->collisionTilesVsCannonsLayer, (19 + (73 * level->tileMap->layer[1].width)), COLLISION_TILE,
                                    level->tileMap->tileSet->tileWidth, level->tileMap->tileSet->tileHeight, -1, false);
    collisionTileCreate(&level->collisionTilesVsCannonsLayer, (19 + (74 * level->tileMap->layer[1].width)), COLLISION_TILE,
                                    level->tileMap->tileSet->tileWidth, level->tileMap->tileSet->tileHeight, -1, false);
    //  4.
    collisionTileCreate(&level->collisionTilesVsCannonsLayer, (17 + (41 * level->tileMap->layer[1].width)), COLLISION_TILE,
                                    level->tileMap->tileSet->tileWidth, level->tileMap->tileSet->tileHeight, -1, false);

    // TILES VS ENEMY BLOCKS
    //  1.
    collisionTileCreate(&level->collisionTilesVsBlocksLayer, (7 + (115 * level->tileMap->layer[1].width)), COLLISION_TILE,
                                    level->tileMap->tileSet->tileWidth, level->tileMap->tileSet->tileHeight, -1, false);
    collisionTileCreate(&level->collisionTilesVsBlocksLayer, (19 + (115 * level->tileMap->layer[1].width)), COLLISION_TILE,
                                    level->tileMap->tileSet->tileWidth, level->tileMap->tileSet->tileHeight, -1, false);
    //  2.
    collisionTileCreate(&level->collisionTilesVsBlocksLayer, (2 + (90 * level->tileMap->layer[1].width)), COLLISION_TILE,
                                    level->tileMap->tileSet->tileWidth, level->tileMap->tileSet->tileHeight, -1, false);
    collisionTileCreate(&level->collisionTilesVsBlocksLayer, (12 + (90 * level->tileMap->layer[1].width)), COLLISION_TILE,
                                    level->tileMap->tileSet->tileWidth, level->tileMap->tileSet->tileHeight, -1, false);
    //  3.
    collisionTileCreate(&level->collisionTilesVsBlocksLayer, (5 + (80 * level->tileMap->layer[1].width)), COLLISION_TILE,
                                    level->tileMap->tileSet->tileWidth, level->tileMap->tileSet->tileHeight, -1, false);
    collisionTileCreate(&level->collisionTilesVsBlocksLayer, (12 + (80 * level->tileMap->layer[1].width)), COLLISION_TILE,
                                    level->tileMap->tileSet->tileWidth, level->tileMap->tileSet->tileHeight, -1, false);
    //  4.
    collisionTileCreate(&level->collisionTilesVsBlocksLayer, (7 + (71 * level->tileMap->layer[1].width)), COLLISION_TILE,
                                    level->tileMap->tileSet->tileWidth, level->tileMap->tileSet->tileHeight, -1, false);
    collisionTileCreate(&level->collisionTilesVsBlocksLayer, (14 + (71 * level->tileMap->layer[1].width)), COLLISION_TILE,
                                    level->tileMap->tileSet->tileWidth, level->tileMap->tileSet->tileHeight, -1, false);
    // 5.
    collisionTileCreate(&level->collisionTilesVsBlocksLayer, (5 + (46 * level->tileMap->layer[1].width)), COLLISION_TILE,
                                    level->tileMap->tileSet->tileWidth, level->tileMap->tileSet->tileHeight, -1, false);
    collisionTileCreate(&level->collisionTilesVsBlocksLayer, (19 + (46 * level->tileMap->layer[1].width)), COLLISION_TILE,
                                    level->tileMap->tileSet->tileWidth, level->tileMap->tileSet->tileHeight, -1, false);

    return level;
}

void levelUpdate(Level *level, float dt){
    UpdateMusicStream(getMusic(MUSIC_01));

    static int ts = 1;
    char buffer[256] = {0};
    if(IsKeyPressed(KEY_ENTER) == true){
        sprintf(buffer, "ScreenShot No.%d.png", ts++);
        TakeScreenshot(buffer);
    }

    if(IsKeyPressed(KEY_M) == true){
        currentStage = MENU_STAGE;
        playerDestroy(level->player);
        levelDestroy(level);
        level->player = NULL;
        level = NULL;

        PauseMusicStream(getMusic(MUSIC_01));
        ResumeMusicStream(getMusic(MUSIC_02));
        return;
    }

    if(level->player->currentHealth > 0 && level->start > 0)
        level->start -= level->scrollSpeed * dt;

    if(level->player->currentHealth > 0 && level->start > 0 && level->start < 0.5 && level->boss == NULL){
        level->boss = bossCreate();
    }

    if(level->player->currentHealth > 0)
        levelCollision(level);

    if(level->player->currentHealth > 0 && level->enemiesLayer != NULL)
        layerUpdate(level->enemiesLayer, dt);

    if(level->boss != NULL)
        bossUpdate(level->boss, dt);

    if(level->eBulletsLayer != NULL)
        layerUpdate(level->eBulletsLayer, dt);

    if(level->animationLayer != NULL)
        layerUpdate(level->animationLayer, dt);
}

void levelDraw(Level *level){
    int index = 0;
    Vector2 newPos = {0, 0};
    Color color = {0};

    // draw tilemap
    for(int i = 0; i < level->tileMap->layerCount; i++){
        for(int y = 0; y < level->tileMap->layer[i].height; y++){
            for(int x = 0; x < level->tileMap->layer[i].width; x++){
                index = level->tileMap->layer[i].layerData[x + (y *level->tileMap->layer[i].width)].index;

                newPos.y = level->tileMap->layer[i].layerData[x + (y *level->tileMap->layer[i].width)].position.y
                            - (level->start * level->tileMap->tileSet->tileHeight) + level->lowBorder;

                if(y >= level->start && newPos.y >= level->lowBorder && newPos.y <= level->highBorder){
                    level->y = y;
                    newPos.x = level->tileMap->layer[i].layerData[x + (y *level->tileMap->layer[i].width)].position.x;
                    color = level->tileMap->tileSet->tile[index].color;

                    DrawTextureRec(level->texture, level->tileMap->tileSet->tile[index].rec, newPos, color);

                    if(i == 1 && level->tileMap->tileSet->tile[index].isCollisionTile == true){
                        Entity *entity1 = getEntityById(level->collisionTilesVsPlayer, x + (y *level->tileMap->layer[i].width));
                        if(entity1 != NULL){
                            entity1->position.x = newPos.x;
                            entity1->position.y = newPos.y;
                            entity1->isDraw = true;
                        }

                        Entity *entity2 = getEntityById(level->collisionTilesVsCannonsLayer, (x + (y * level->tileMap->layer[1].width)));
                        if(entity2 != NULL){
                            entity2->position.x = newPos.x;
                            entity2->position.y = newPos.y;
                            entity2->isDraw = true;
                        }
                        Entity *entity3 = getEntityById(level->collisionTilesVsBlocksLayer, (x + (y * level->tileMap->layer[1].width)));
                        if(entity3 != NULL){
                            entity3->position.x = newPos.x;
                            entity3->position.y = newPos.y;
                            entity3->isDraw = true;
                            // ONLY LEFT AND RIGHT FOR THE BLOCKS
                            if(newPos.x < SCREEN_WIDTH / 2)
                                entity3->entityPos = LEFT_POSITION;
                            else
                                entity3->entityPos = RIGHT_POSITION;
                        }
                    }
                }
                else if(i == 1 && level->tileMap->tileSet->tile[index].isCollisionTile == true){
                    Entity *entity1 = getEntityById(level->collisionTilesVsPlayer, x + (y *level->tileMap->layer[i].width));
                    if(entity1 != NULL){
                        entity1->isDraw = false;
                    }

                    Entity *entity2 = getEntityById(level->collisionTilesVsCannonsLayer, (x + (y * level->tileMap->layer[1].width)));
                    if(entity2 != NULL)
                        entity2->isDraw = false;

                    Entity *entity3 = getEntityById(level->collisionTilesVsBlocksLayer, (x + (y * level->tileMap->layer[1].width)));
                    if(entity3 != NULL)
                        entity3->isDraw = false;
                }
            }
        }
    }

    if(level->enemiesLayer != NULL)
        layerDraw(level->enemiesLayer);

    if(level->boss != NULL)
        bossDraw(level->boss);

    if(level->eBulletsLayer != NULL)
        layerDraw(level->eBulletsLayer);

    if(level->animationLayer != NULL)
        layerDraw(level->animationLayer);

    DrawRectangle(0, 0, 720, level->infoBorder, RAYWHITE);

    DrawText("HEALTH", 10, 10, 24, BLACK);
    DrawText("HiSCORE", 290, 10, 24, RED);  DrawText(FormatText("%08i", level->hiScore), 285, 30, 24, RED);
    DrawText("SCORE", 590, 10, 24, BLUE);   DrawText(FormatText("%08i", level->score), 572, 30, 24, BLUE);

    drawHealthBar(level);
}

static void drawHealthBar(Level *level){
    // background
    DrawRectangle(8, 35, 100, 20, LIGHTGRAY);

    // health bar
    DrawRectangle(8, 35, (int)((level->player->currentHealth / level->player->maxHealth) * 100), 20, MAROON);

    // lines around background
    DrawRectangleLines(8, 35, 100, 20, GRAY);
}

void levelDestroy(Level *level){
    if(level != NULL){
        if(level->enemiesLayer != NULL)
            layerDestroy(&level->enemiesLayer);
        if(level->eBulletsLayer != NULL)
            layerDestroy(&level->eBulletsLayer);
        if(level->collisionTilesVsCannonsLayer != NULL)
            layerDestroy(&level->collisionTilesVsCannonsLayer);
        if(level->collisionTilesVsBlocksLayer != NULL)
            layerDestroy(&level->collisionTilesVsBlocksLayer);
        if(level->collisionTilesVsPlayer != NULL)
            layerDestroy(&level->collisionTilesVsPlayer);
        if(level->animationLayer != NULL)
            layerDestroy(&level->animationLayer);
        if(level->boss != NULL)
            bossDestroy(level->boss);

        freeTileMap(level->tileMap);
        free(level);
    }
}

static void levelCollision(Level *level){
    // collision tiles vs enemy cannons bullets
    int bulletsSize = getLayerSize(level->eBulletsLayer);
    int cTilesSize = getLayerSize(level->collisionTilesVsCannonsLayer);

    Entity *currentBullet = NULL;
    Entity *currentTile = NULL;

    for(int i = 0; i < bulletsSize; i++){
        for(int j = 0; j < cTilesSize; j++){
            currentBullet = getEntityByIndex(level->eBulletsLayer, i);
            if(currentBullet != NULL && currentBullet->entityType == ENEMY_BULLET_CANNON){

                currentTile = getEntityByIndex(level->collisionTilesVsCannonsLayer, j);
                if(currentTile != NULL && currentTile->isDraw == true){

                    if(boxBoxCollision(currentBullet, currentTile) == true){
                        animationCreate(&level->animationLayer, currentBullet->id, currentBullet->position, currentBullet->rec, currentBullet->entityPos,
                                         ENEMY_BULLET_CANNON_HIT_FX, 10, 7, 1, EXPLOSION_01);

                        entityDestroy(&level->eBulletsLayer, currentBullet->id);
                        bulletsSize--;
                    }

                }
            }
        }
    }

    // collision tiles vs enemy blocks
    Entity *currentEnemy = NULL;
    currentTile = NULL;

    int enemiesSize = getLayerSize(level->enemiesLayer);
    cTilesSize = getLayerSize(level->collisionTilesVsBlocksLayer);

    for(int i = 0; i < enemiesSize; i++){
        for(int j = 0; j < cTilesSize; j++){
            currentEnemy = getEntityByIndex(level->enemiesLayer, i);
            if(currentEnemy != NULL && currentEnemy->entityType == ENEMY_BLOCK && currentEnemy->isDraw){
                // block 21 vs block 22
                if(currentEnemy->id == 21){
                    Entity *block22 = getEntityById(level->enemiesLayer, 22);
                    if(block22 != NULL && block22->isDraw == true){
                        if(currentEnemy->position.x < block22->position.x + block22->rec.width){
                            currentEnemy->entityPos = RIGHT_POSITION;
                        }
                    }
                }
                else if(currentEnemy->id == 22){
                    Entity *block21 = getEntityById(level->enemiesLayer, 21);
                    if(block21 != NULL && block21->isDraw == true){
                        if(currentEnemy->position.x + currentEnemy->rec.width > block21->position.x ){
                            currentEnemy->entityPos = LEFT_POSITION;
                        }
                    }
                }
                // block vs tile
                currentTile = getEntityByIndex(level->collisionTilesVsBlocksLayer, j);
                if(currentTile != NULL && currentTile->isDraw){

                    int range = currentTile->position.y -
                                  (currentEnemy->position.y - (level->start * level->tileMap->tileSet->tileHeight) + level->lowBorder);

                    if(abs(range) < currentEnemy->rec.height / 2){
                        if(currentTile->entityPos == LEFT_POSITION){
                            if(currentEnemy->entityPos == LEFT_POSITION){
                                if(currentEnemy->position.x < currentTile->position.x + currentTile->rec.width)
                                    currentEnemy->entityPos = RIGHT_POSITION;
                            }
                        }
                        else if(currentTile->entityPos == RIGHT_POSITION){
                                if(currentEnemy->entityPos == RIGHT_POSITION){
                                    if(currentEnemy->position.x + currentEnemy->rec.width > currentTile->position.x)
                                        currentEnemy->entityPos = LEFT_POSITION;
                            }
                        }
                    }
                }
            }
        }
    }

    // collision player bullets vs enemy zako or enemy cannon or enemy block
    bulletsSize = getLayerSize(level->player->bulletsLayer);
    enemiesSize = getLayerSize(level->enemiesLayer);
    currentBullet = NULL;
    currentEnemy = NULL;

    for(int i = 0; i < bulletsSize; i++){
        for(int j = 0; j < enemiesSize; j++){

            currentBullet = getEntityByIndex(level->player->bulletsLayer, i);
            if(currentBullet != NULL && currentBullet->isDraw == true){

                currentEnemy = getEntityByIndex(level->enemiesLayer, j);
                if(currentEnemy != NULL && currentEnemy->isDraw == true){

                    float tempY = currentEnemy->position.y - (level->start * level->tileMap->tileSet->tileHeight) + level->lowBorder;

                    float range = currentBullet->position.y - tempY;

                    if(abs(range) < currentEnemy->rec.height * 2){
                        float oldY = currentEnemy->position.y;
                        currentEnemy->position.y = tempY;

                        if(boxBoxCollision(currentEnemy, currentBullet) == true){
                            PlaySound(getSound(SOUND_04));

                            currentEnemy->currentHealth -= currentBullet->damage;

                            currentBullet->currentHealth -= currentEnemy->damage;

                            if(currentEnemy->entityType == ENEMY_ZAKO){

                                if(currentEnemy->currentHealth <= 0){
                                    PlaySound(getSound(SOUND_05));

                                    animationCreate(&level->animationLayer, currentEnemy->id, currentEnemy->position, currentEnemy->rec, currentEnemy->entityPos,
                                                ENEMY_ZAKO_DESTROYED_FX, 25, 4, 4, EXPLOSION_04);

                                    entityDestroy(&level->enemiesLayer, currentEnemy->id);
                                    enemiesSize--;

                                    level->score += ENEMY_ZAKO_SCORE;
                                }

                                if(currentBullet->currentHealth <= 0){
                                    animationCreate(&level->animationLayer, currentBullet->id, currentBullet->position, currentBullet->rec, currentEnemy->entityPos,
                                                PLAYER_BULLET_HIT_FX, 25, 9, 1, BULLET_HIT_01);

                                    entityDestroy(&level->player->bulletsLayer, currentBullet->id);
                                    bulletsSize--;
                                }

                                currentEnemy->position.y = oldY;
                            }

                            else if(currentEnemy->entityType == ENEMY_CANNON){

                                if(currentEnemy->currentHealth <= 0){
                                    cannonDestroyedAnimation(level, currentEnemy);

                                    entityDestroy(&level->enemiesLayer, currentEnemy->id);
                                    enemiesSize--;

                                    level->score += ENEMY_CANNON_SCORE;
                                }

                                if(currentBullet->currentHealth <= 0){
                                    animationCreate(&level->animationLayer, currentBullet->id, currentBullet->position, currentBullet->rec, currentEnemy->entityPos,
                                                PLAYER_BULLET_HIT_FX, 25, 9, 1, BULLET_HIT_01);

                                    entityDestroy(&level->player->bulletsLayer, currentBullet->id);
                                    bulletsSize--;
                                }

                                currentEnemy->position.y = oldY;
                            }

                            else if(currentEnemy->entityType == ENEMY_BLOCK){

                                if(currentBullet->currentHealth <= 0){
                                    animationCreate(&level->animationLayer, currentBullet->id, currentBullet->position, currentBullet->rec, currentEnemy->entityPos,
                                                PLAYER_BULLET_HIT_FX, 25, 9, 1, BULLET_HIT_01);

                                    entityDestroy(&level->player->bulletsLayer, currentBullet->id);
                                    bulletsSize--;
                                }

                                currentEnemy->position.y = oldY;
                            }
                        }
                        else
                            currentEnemy->position.y = oldY;
                    }
                }
            }
        }
    }

    // collision player bullets vs boss bullets
    int pBulletsSize = getLayerSize(level->player->bulletsLayer);
    bulletsSize = getLayerSize(level->eBulletsLayer);
    Entity *currentPBullet = NULL;
    currentBullet = NULL;

    for(int i = 0; i < pBulletsSize; i++){
        for(int j = 0; j < bulletsSize; j++){

            currentPBullet = getEntityByIndex(level->player->bulletsLayer, i);
            if(currentPBullet != NULL && currentPBullet->isDraw == true){

                currentBullet = getEntityByIndex(level->eBulletsLayer, j);
                if(currentBullet != NULL && currentBullet->isDraw == true && currentBullet->entityType == ENEMY_BULLET_BOSS){

                    float rangeY = currentPBullet->position.y - currentBullet->position.y;

                    if(abs(rangeY) < currentBullet->rec.height * 2){

                        if(boxBoxCollision(currentPBullet, currentBullet) == true){

                            currentPBullet->currentHealth -= currentBullet->damage;

                            currentBullet->currentHealth -= currentPBullet->damage;

                            if(currentBullet->currentHealth <= 0){
                                bossBulletDestroyedAnimation(level, currentBullet);

                                entityDestroy(&level->eBulletsLayer, currentBullet->id);
                                bulletsSize--;
                            }

                            if(currentPBullet->currentHealth <= 0){
                                animationCreate(&level->animationLayer, currentPBullet->id, currentPBullet->position, currentPBullet->rec, currentPBullet->entityPos,
                                                PLAYER_BULLET_HIT_FX, 25, 9, 1, BULLET_HIT_01);

                                entityDestroy(&level->player->bulletsLayer, currentPBullet->id);
                                pBulletsSize--;
                            }

                        }
                    }
                }
            }
        }
    }

    // collision player bullets vs boss rotators
    pBulletsSize = getLayerSize(level->player->bulletsLayer);
    bulletsSize = getLayerSize(level->animationLayer);
    currentPBullet = NULL;
    currentBullet = NULL;

    for(int i = 0; i < pBulletsSize; i++){
        for(int j = 0; j < bulletsSize; j++){
            currentPBullet = getEntityByIndex(level->player->bulletsLayer, i);
            if(currentPBullet != NULL && currentPBullet->isDraw == true){
                currentBullet = getEntityByIndex(level->animationLayer, j);
                if(currentBullet != NULL && currentBullet->isAnimation == true && currentBullet->entityType == ENEMY_BULLET_BOSS_ROTATOR_FX){

                    float rangeY = currentPBullet->position.y - currentBullet->position.y;

                    if(abs(rangeY) < currentBullet->rec.height * 2){
                        if(boxBoxCollision(currentPBullet, currentBullet) == true){

                            currentPBullet->currentHealth -= currentBullet->damage;

                            currentBullet->currentHealth -= currentPBullet->damage;

                            if(currentBullet->currentHealth <= 0){
                                animationCreate(&level->animationLayer, currentBullet->id, currentBullet->position, currentBullet->rec,
                                            currentBullet->entityPos, ENEMY_BULLET_BOSS_ROTATOR_DESTROYED_FX, 30, 9, 1, EXPLOSION_08);

                                entityDestroy(&level->animationLayer, currentBullet->id);
                                bulletsSize--;
                            }

                            if(currentPBullet->currentHealth <= 0){
                                animationCreate(&level->animationLayer, currentPBullet->id, currentPBullet->position, currentPBullet->rec, currentPBullet->entityPos,
                                                PLAYER_BULLET_HIT_FX, 25, 9, 1, BULLET_HIT_01);

                                entityDestroy(&level->player->bulletsLayer, currentPBullet->id);
                                pBulletsSize--;
                            }
                        }
                    }
                }
            }
        }
    }


    // collision player bullets vs boss shield
    pBulletsSize = getLayerSize(level->player->bulletsLayer);
    currentPBullet = NULL;

    if(level->boss != NULL && level->boss->shield->health > 0){
        for(int i = 0; i < pBulletsSize; i++){
            currentPBullet = getEntityByIndex(level->player->bulletsLayer, i);
            if(currentPBullet != NULL && currentPBullet->isDraw == true){

            if(CheckCollisionPointCircle((Vector2){currentPBullet->position.x, currentPBullet->position.y},
                                         (Vector2){level->boss->position.x + level->boss->rec.width / 2, level->boss->position.y + level->boss->rec.height / 2},
                                         245) == true){

                    level->boss->shield->health -= level->player->bulletDamage;

                    level->player->bulletHealth -= level->boss->shield->damage;

                    if(level->player->bulletHealth <= 0){
                        animationCreate(&level->animationLayer, currentPBullet->id, currentPBullet->position, currentPBullet->rec, currentPBullet->entityPos,
                                                PLAYER_BULLET_HIT_FX, 35, 9, 1, BULLET_HIT_01);

                        entityDestroy(&level->player->bulletsLayer, currentPBullet->id);
                        pBulletsSize--;
                    }
                }
            }
        }
    }

    // collision player bullets vs boss ship
    pBulletsSize = getLayerSize(level->player->bulletsLayer);
    currentPBullet = NULL;

    if(level->boss != NULL && level->boss->health > 0){
        for(int i = 0; i < pBulletsSize; i++){
            currentPBullet = getEntityByIndex(level->player->bulletsLayer, i);
            if(currentPBullet != NULL && currentPBullet->isDraw == true){

                if(CheckCollisionPointRec((Vector2){currentPBullet->position.x, currentPBullet->position.y}, (Rectangle){level->boss->position.x + 16, level->boss->position.y + 36, 100, 78}) == true ||
                   CheckCollisionPointRec((Vector2){currentPBullet->position.x, currentPBullet->position.y}, (Rectangle){level->boss->position.x + 340, level->boss->position.y + 36, 100, 78}) == true ||
                   CheckCollisionPointRec((Vector2){currentPBullet->position.x, currentPBullet->position.y}, (Rectangle){level->boss->position.x + 108, level->boss->position.y + 156, 232, 90}) == true){

                    level->boss->health -= level->player->bulletDamage;

                    level->player->bulletHealth -= level->boss->damage;

                    if(level->player->bulletHealth <= 0){
                        animationCreate(&level->animationLayer, currentPBullet->id, currentPBullet->position, currentPBullet->rec, currentPBullet->entityPos,
                                                PLAYER_BULLET_HIT_FX, 35, 9, 1, BULLET_HIT_01);

                        entityDestroy(&level->player->bulletsLayer, currentPBullet->id);
                        pBulletsSize--;
                    }

                    if(level->boss->health <= 0){
                        animationCreate(&level->animationLayer, 0xfabb, (Vector2){level->boss->position.x + 16, level->boss->position.y + 36}, level->boss->rec, CENTER_POSITION, BOSS_DESTROYED_FX,
                                        10, 7, 2, EXPLOSION_10);
                        animationCreate(&level->animationLayer, 0xfacc, (Vector2){level->boss->position.x + 340, level->boss->position.y + 36}, level->boss->rec, CENTER_POSITION, BOSS_DESTROYED_FX,
                                        12, 7, 2, EXPLOSION_10);
                        animationCreate(&level->animationLayer, 0xfadd, (Vector2){level->boss->position.x + 108, level->boss->position.y + 156}, level->boss->rec, CENTER_POSITION, BOSS_DESTROYED_FX,
                                        14, 7, 2, EXPLOSION_10);
                        animationCreate(&level->animationLayer, 0xfaee, (Vector2){level->boss->position.x + 160, level->boss->position.y + 36}, level->boss->rec, CENTER_POSITION, BOSS_DESTROYED_FX,
                                        10, 7, 2, EXPLOSION_10);
                        animationCreate(&level->animationLayer, 0xfaff, (Vector2){level->boss->position.x + 50, level->boss->position.y + 100}, level->boss->rec, CENTER_POSITION, BOSS_DESTROYED_FX,
                                        12, 7, 2, EXPLOSION_10);

                        break;
                    }
                }
            }
        }
    }

    // collision player vs tiles
    currentTile = NULL;
    cTilesSize = getLayerSize(level->collisionTilesVsPlayer);

    if(level->player->currentHealth > 0){
        for(int i = 0; i < cTilesSize; i++){
            currentTile = getEntityByIndex(level->collisionTilesVsPlayer, i);
            if(currentTile != NULL && currentTile->isDraw == true){

                float rangeY = currentTile->position.y - level->player->position.y;

                if(abs(rangeY) < level->player->rec.height * 2){
                    if(level->player->isCollision == false && boxPlayerCollision(level->player, currentTile) == true){
                        level->player->isCollision = true;
                        level->player->collisionTile = currentTile;

                        level->player->currentHealth -= COLLISION_TILE_DAMAGE;

                        if(level->player->currentHealth <= 0){
                            playerDestroyedAnimation(level);
                            break;
                        }
                    }
                }
            }
        }
    }

    // collision player vs enemy zako bullets or cannon bullets or boss bullets
    bulletsSize = getLayerSize(level->eBulletsLayer);
    currentBullet = NULL;

    for(int i = 0; i < bulletsSize; i++){
        currentBullet = getEntityByIndex(level->eBulletsLayer, i);
        if(currentBullet != NULL){

            float range = currentBullet->position.y - level->player->position.y;

            if(abs(range) < level->player->rec.height * 2){
                if(boxPlayerCollision(level->player, currentBullet) == true){
                    currentBullet->currentHealth -= level->player->playerDamage;

                    level->player->currentHealth -= currentBullet->damage;

                    if(currentBullet->entityType == ENEMY_BULLET_ZAKO){
                        animationCreate(&level->animationLayer, currentBullet->id, currentBullet->position, currentBullet->rec, currentBullet->entityPos,
                                        ENEMY_BULLET_ZAKO_HIT_FX, 30, 1, 10, EXPLOSION_02);
                    }

                    else if(currentBullet->entityType == ENEMY_BULLET_CANNON){
                        animationCreate(&level->animationLayer, currentBullet->id, currentBullet->position, currentBullet->rec, currentBullet->entityPos,
                                        ENEMY_BULLET_CANNON_HIT_FX, 10, 7, 1, EXPLOSION_01);
                    }

                    else if(currentBullet->entityType == ENEMY_BULLET_BOSS){
                        animationCreate(&level->animationLayer, currentBullet->id, currentBullet->position, currentBullet->rec, currentBullet->entityPos,
                                        ENEMY_BULLET_BOSS_HIT_FX, 15, 7, 1, EXPLOSION_06);
                    }

                    if(currentBullet->currentHealth <= 0){
                        entityDestroy(&level->eBulletsLayer, currentBullet->id);
                        bulletsSize--;
                    }

                    if(level->player->currentHealth <= 0){
                            playerDestroyedAnimation(level);
                            break;
                    }
                }
            }
        }
    }

    // collision player vs enemy zako or cannon or block
    enemiesSize = getLayerSize(level->enemiesLayer);
    currentEnemy = NULL;

    for(int i = 0; i < enemiesSize; i++){
        currentEnemy = getEntityByIndex(level->enemiesLayer, i);
        if(currentEnemy != NULL && currentEnemy->isDraw == true){
            float tempY = currentEnemy->position.y - (level->start * level->tileMap->tileSet->tileHeight) + level->lowBorder;

            float range = level->player->position.y - tempY;

            if(abs(range) < level->player->rec.height * 2){
                float oldY = currentEnemy->position.y;
                currentEnemy->position.y = tempY;

                if(boxPlayerCollision(level->player, currentEnemy) == true){
                    currentEnemy->currentHealth -= level->player->playerDamage;

                    level->player->currentHealth -= currentEnemy->damage;

                    if(currentEnemy->entityType == ENEMY_ZAKO){

                        if(currentEnemy->currentHealth <= 0){
                            animationCreate(&level->animationLayer, currentEnemy->id, currentEnemy->position, currentEnemy->rec, currentEnemy->entityPos,
                                            ENEMY_ZAKO_DESTROYED_FX, 25, 4, 4, EXPLOSION_04);

                            entityDestroy(&level->enemiesLayer, currentEnemy->id);
                            enemiesSize--;

                            level->score += ENEMY_ZAKO_SCORE;
                        }

                        if(level->player->currentHealth <= 0){
                            playerDestroyedAnimation(level);
                            break;
                        }

                        currentEnemy->position.y = oldY;
                    }
                    else if(currentEnemy->entityType == ENEMY_CANNON){

                        if(currentEnemy->currentHealth <= 0){
                            cannonDestroyedAnimation(level, currentEnemy);

                            entityDestroy(&level->enemiesLayer, currentEnemy->id);

                            enemiesSize--;

                            level->score += ENEMY_CANNON_SCORE;
                        }

                        if(level->player->currentHealth <= 0){
                            playerDestroyedAnimation(level);
                            break;
                        }

                        currentEnemy->position.y = oldY;
                    }
                    // CANT BE DESTROYED <<<<<<<<<<<<<<<-------------------------------------------<<<<<<<<<<<<<<<<<------------
                    // GAME OVER
                    else if(currentEnemy->entityType == ENEMY_BLOCK){
                        currentEnemy->position.y = oldY;

                        level->player->currentHealth -= ENEMY_BLOCK_UNIT_DAMAGE;

                        if(level->player->currentHealth <= 0){
                            playerDestroyedAnimation(level);
                            break;
                        }
                    }
                }
                else
                    currentEnemy->position.y = oldY;
            }
        }
    }

    // collision player vs boss rotators
    bulletsSize = getLayerSize(level->animationLayer);
    currentBullet = NULL;

    for(int i = 0; i < bulletsSize; i++){
        currentBullet = getEntityByIndex(level->animationLayer, i);
        if(currentBullet != NULL && currentBullet->isAnimation == true && currentBullet->entityType == ENEMY_BULLET_BOSS_ROTATOR_FX){

            float rangeY = level->player->position.y - currentBullet->position.y;

            if(abs(rangeY) < level->player->rec.height * 2){
                if(boxPlayerCollision(level->player, currentBullet) == true){
                    currentBullet->currentHealth -= level->player->playerDamage;

                    level->player->currentHealth -= currentBullet->damage;

                    if(level->player->currentHealth <= 0){
                        playerDestroyedAnimation(level);
                        break;
                    }

                    if(currentBullet->currentHealth <= 0){
                        animationCreate(&level->animationLayer, currentBullet->id, currentBullet->position, currentBullet->rec,
                                        currentBullet->entityPos, ENEMY_BULLET_BOSS_ROTATOR_HIT_FX, 20, 7, 1, EXPLOSION_09);

                        entityDestroy(&level->animationLayer, currentBullet->id);
                        bulletsSize--;
                    }
                }
            }
        }
    }

    // collision player vs boss shield
    if(level->boss != NULL && level->boss->shield->health > 0){
        if(playerShieldCollision(level->player, level->boss) == true){
            level->player->currentHealth -= level->boss->shield->damage;

            if(level->player->currentHealth <= 0){
                playerDestroyedAnimation(level);
            }
        }
    }

    // collision player vs boss ship
    if(level->boss != NULL && level->boss->health > 0){
        if(playerBossCollision(level->player, level->boss) == true){
            level->player->currentHealth -= level->boss->damage;

            if(level->player->currentHealth <= 0){
                playerDestroyedAnimation(level);
            }
        }
    }
}

static void playerDestroyedAnimation(Level *level){
    animationCreate(&level->animationLayer, 1001, level->player->position, level->player->rec, CENTER_POSITION,
        PLAYER_DESTROYED_FX, 15, 4, 6, EXPLOSION_03);
    animationCreate(&level->animationLayer, 1002, level->player->position, level->player->rec, CENTER_POSITION,
        PLAYER_DESTROYED_FX, 25, 4, 6, EXPLOSION_03);
    animationCreate(&level->animationLayer, 1003, level->player->position, level->player->rec, CENTER_POSITION,
        PLAYER_DESTROYED_FX, 35, 4, 6, EXPLOSION_03);
}

static void cannonDestroyedAnimation(Level *level, Entity *e){
    animationCreate(&level->animationLayer, e->id, e->position, e->rec, e->entityPos,
        ENEMY_CANNON_DESTROYED_FX, 15, 4, 4, EXPLOSION_05);
    animationCreate(&level->animationLayer, e->id + 1, e->position, e->rec, e->entityPos,
        ENEMY_CANNON_DESTROYED_FX, 25, 4, 4, EXPLOSION_05);
    animationCreate(&level->animationLayer, e->id + 2, e->position, e->rec, e->entityPos,
        ENEMY_CANNON_DESTROYED_FX, 35, 4, 4, EXPLOSION_05);
}

static void bossBulletDestroyedAnimation(Level *level, Entity *e){
    animationCreate(&level->animationLayer, e->id, e->position, e->rec,
        e->entityPos,ENEMY_BULLET_BOSS_DESTROYED_FX, 15, 6, 1, EXPLOSION_07);
    animationCreate(&level->animationLayer, e->id + 1, e->position, e->rec,
        e->entityPos,ENEMY_BULLET_BOSS_DESTROYED_FX, 15, 6, 1, EXPLOSION_07);
    animationCreate(&level->animationLayer, e->id + 2, e->position, e->rec,
        e->entityPos,ENEMY_BULLET_BOSS_DESTROYED_FX, 15, 6, 1, EXPLOSION_07);
}

static bool boxBoxCollision(Entity *entityA, Entity *entityB){
	if(entityA->position.x + entityA->rec.width <= entityB->position.x)
		return false;

	if(entityA->position.y + entityA->rec.height <= entityB->position.y)
		return false;

	if(entityA->position.x >= entityB->position.x + entityB->rec.width)
		return false;

	if(entityA->position.y >= entityB->position.y + entityB->rec.height)
		return false;

	return true;
}

static bool boxPlayerCollision(Player *player, Entity *entityA){
    Rectangle rec1 = {player->position.x, player->position.y, player->rec.width, player->rec.height};

    Rectangle rec2 = {entityA->position.x, entityA->position.y, entityA->rec.width, entityA->rec.height};

    if(CheckCollisionRecs(rec1, rec2) == true){
        return true;
    }

    return false;
}

static bool playerBossCollision(Player *player, Boss *boss){
    Rectangle rec1 = {player->position.x, player->position.y, player->rec.width, player->rec.height};

    Rectangle rec2 = {boss->position.x, boss->position.y, boss->rec.width, boss->rec.height};

    if(CheckCollisionRecs(rec1, rec2) == true){
        return true;
    }

    return false;
}

static bool playerShieldCollision(Player *player, Boss *boss){
    Rectangle rec1 = {player->position.x, player->position.y, player->rec.width, player->rec.height};

    Rectangle rec2 = {boss->position.x - boss->rec.width / 16, boss->position.y - boss->rec.height / 2,
                      boss->shield->rec.width, boss->shield->rec.height};

    if(CheckCollisionRecs(rec1, rec2) == true){
        return true;
    }

    return false;
}


















































































































