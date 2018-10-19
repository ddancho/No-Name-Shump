#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <raylib.h>

#include "boss.h"
#include "level.h"
#include "enemy_bullet.h"
#include "animation.h"
#include "level_one.h"
#include "game_stage.h"
#include "res_manager.h"

extern Level *level;
extern GameStage currentStage;

static float distance(Vector2 a, Vector2 b);
static Vector2 pathFollowing(Boss *boss);
static void addPathNode(Boss *boss, int index, Vector2 node);

Boss* bossCreate(void){
    Boss *boss = NULL;

    boss = malloc(sizeof(Boss));
    if(boss == NULL){
        fprintf(stderr, "boss malloc err...\n");
        return NULL;
    }

    boss->damage = ENEMY_BOSS_DAMAGE;
    boss->health = ENEMY_BOSS_HEALTH;

    boss->position = (Vector2){125, 250};
    boss->texture = getTexture(ENEMY_BOSS_01);
    boss->rec.x = 0;
    boss->rec.y = 0;
    boss->rec.width = 448;
    boss->rec.height = 256;
    boss->fireRate = 0;
    boss->rFireRate = 0;
    boss->speed = 200;
    // we are on the center already
    boss->currentNode = 1;
    boss->maxNode = 5;
    boss->pathDirection = GO_FORTH;
    boss->path = NULL;
    boss->screenshotFileName = NULL;

    boss->path = malloc(sizeof(Path));
    if(boss->path == NULL){
        fprintf(stderr, "path malloc err...\n");
        free(boss);
        return NULL;
    }
    boss->path->nodes = NULL;

    boss->path->nodes = malloc(sizeof(Vector2) * boss->maxNode);
    if(boss->path->nodes == NULL){
        fprintf(stderr, "path nodes malloc err...\n");
        free(boss->path);
        free(boss);
        return NULL;
    }

    addPathNode(boss, 0, (Vector2){125, 250}); // CENTER POSITION
    addPathNode(boss, 1, (Vector2){15, 250});  // LEFT   POSITION
    addPathNode(boss, 2, (Vector2){255, 250}); // RIGHT  POSITION
    addPathNode(boss, 3, (Vector2){125, 450}); // UP     POSITION
    addPathNode(boss, 4, (Vector2){125, 75});  // DOWN   POSITION

    boss->shield = malloc(sizeof(Shield));
    if(boss->shield == NULL){
        fprintf(stderr, "shield malloc err...\n");
        return NULL;
    }

    boss->shield->damage = ENEMY_BOSS_SHIELD_DAMAGE;
    boss->shield->health = ENEMY_BOSS_SHIELD_HEALTH;

    boss->shield->position.x = boss->position.x - boss->rec.width / 16;
    boss->shield->position.y = boss->position.y - boss->rec.height / 2;
    boss->shield->texture = getTexture(RING_01);
    boss->shield->rec.x = 0;
    boss->shield->rec.y = 0;
    boss->shield->rec.width = boss->shield->texture.width;
    boss->shield->rec.height = boss->shield->texture.height;
    boss->shield->rotation = 0;
    boss->shield->rotationSpeed = 0;
    boss->shield->rotationRate = 0;

    return boss;
}

void bossUpdate(Boss *boss, float dt){
    static float ssRate = 0;
    static float id = 0.000001;
    static bool leftBullet = false;

    if(boss->health > 0){
        boss->shield->rotationRate += dt;

        if(boss->shield->rotationRate > 0 && boss->shield->rotationRate < 5.0)
            boss->shield->rotationSpeed = 35;

        else if(boss->shield->rotationRate > 5.0 && boss->shield->rotationRate < 10.0)
            boss->shield->rotationSpeed = -55;

        else if(boss->shield->rotationRate > 10.0 && boss->shield->rotationRate < 15.0){
            boss->shield->rotationSpeed = 75;
        }
        else
            boss->shield->rotationRate = 0;

        boss->shield->rotation += boss->shield->rotationSpeed * dt;

        Vector2 target = pathFollowing(boss);

        float xDist = abs(target.x - boss->position.x);
        float yDist = abs(target.y - boss->position.y);
        float totalDist = xDist + yDist;

        float vSpeedX = xDist / totalDist;
        float vSpeedY = 1.0 - vSpeedX;

        if(target.x < boss->position.x)
            vSpeedX *= -1;
        if(target.y < boss->position.y)
            vSpeedY *= -1;

        boss->position.x += vSpeedX * boss->speed * dt;
        boss->position.y += vSpeedY * boss->speed * dt;

        boss->fireRate += dt;

        if(level->boss != NULL && level->boss->fireRate > 0.25 && level->boss->fireRate < 0.50 && leftBullet == false){
            leftBullet = true;
            eBulletCreate(&level->eBulletsLayer, id, level->boss->rec, LEFT_POSITION, ENEMY_BULLET_BOSS,
                         (Vector2){0, 0}, (Vector2){level->boss->position.x, level->boss->position.y}, 300,
                          ENEMY_BOSS_BULLET_DAMAGE, ENEMY_BOSS_BULLET_HEALTH, ENEMY_BOSS_01);
            id += 0.000001;
        }
        else if(level->boss != NULL && level->boss->fireRate > 0.5){
            leftBullet = false;
            level->boss->fireRate = 0;
            eBulletCreate(&level->eBulletsLayer, id, level->boss->rec, RIGHT_POSITION, ENEMY_BULLET_BOSS,
                         (Vector2){0, 0}, (Vector2){level->boss->position.x, level->boss->position.y}, 300,
                          ENEMY_BOSS_BULLET_DAMAGE, ENEMY_BOSS_BULLET_HEALTH, ENEMY_BOSS_01);
            id += 0.000001;
        }

        boss->rFireRate += dt;
        if(boss->rFireRate > 0.55){
            boss->rFireRate = 0;
            animationCreate(&level->animationLayer, id, boss->position, boss->rec, CENTER_POSITION, ENEMY_BULLET_BOSS_ROTATOR_FX,
                            30, 5, 1, ROTATOR_01);
            id += 0.000001;
        }
    }

    else if(boss->health <= 0){
        ssRate += dt;
        if(ssRate >= 1.0){
            ssRate = 0;
            boss->screenshotFileName = "bossDeath.png";
            TakeScreenshot(boss->screenshotFileName);
            boss->screenshotTexture = LoadTexture(boss->screenshotFileName);
            currentStage = WIN_STAGE;
        }
    }
}

void bossDraw(Boss *boss){
    if(boss->health > 0)
        DrawTextureRec(boss->texture, boss->rec, boss->position, RAYWHITE);

    //DrawRectangleLines(boss->position.x + 16, boss->position.y + 36, 100, 78, RED);

    //DrawRectangleLines(boss->position.x + 340, boss->position.y + 36, 100, 78, RED);

    //DrawRectangleLines(boss->position.x + 108, boss->position.y + 156, 232, 90, RED);

    if(boss->shield->health > 0){

        Rectangle destRec = {boss->position.x + boss->rec.width / 2, boss->position.y + boss->rec.height / 2,
                             boss->shield->rec.width, boss->shield->rec.height};

        Vector2 origin = {boss->shield->rec.width / 2, boss->shield->rec.height / 2};

        DrawTexturePro(boss->shield->texture, boss->shield->rec, destRec, origin, boss->shield->rotation, RAYWHITE);
    }
}

void bossDestroy(Boss *boss){
    if(boss->path->nodes != NULL)
        free(boss->path->nodes);

    if(boss->path != NULL)
        free(boss->path);

    if(boss->shield != NULL)
        free(boss->shield);

    if(boss->screenshotFileName != NULL){
        remove(boss->screenshotFileName);
        boss->screenshotFileName = NULL;
        UnloadTexture(boss->screenshotTexture);
    }

    free(boss);
}

static Vector2 pathFollowing(Boss *boss){
    Vector2 target = {0, 0};

    if(boss->path != NULL){
        target = boss->path->nodes[boss->currentNode];

        if(distance(boss->position, target) < 10){
            boss->currentNode += boss->pathDirection;

            if(boss->currentNode >= boss->maxNode || boss->currentNode < 0){
                boss->pathDirection *= (-1);
                boss->currentNode += boss->pathDirection;
            }
        }
    }

    return target;
}

static void addPathNode(Boss *boss, int index, Vector2 node){
    boss->path->nodes[index] = node;
}

static float distance(Vector2 a, Vector2 b){
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}




























