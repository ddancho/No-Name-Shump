#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <raylib.h>

#include "enemy.h"
#include "level.h"
#include "enemy_bullet.h"
#include "player.h"
#include "level_one.h"
#include "res_manager.h"

extern Level *level;

static bool enemyUpdate(void *entity, float dt);
static void enemyDraw(void *entity);
static void enemyDestroy(void *entity);

Entity* enemyCreate(Layer **head, float id, EntityType entityType, Vector2 position, float speed, float rotation,
                 float damage, float health, float score, TextureName textureName){
    Entity *entity = entityCreate(head, id, entityType);

    entity->update = enemyUpdate;
    entity->draw = enemyDraw;
    entity->destroy = enemyDestroy;

    entity->damage = damage;
    entity->currentHealth = health;
    entity->maxHealth = health;
    entity->score = score;
    //
    entity->texture = getTexture(textureName);
    entity->position = position;
    entity->speed = speed;
    entity->rotation = rotation;
    entity->scale = 1.0;
    entity->rec.x = 0;
    entity->rec.y = 0;
    entity->rec.width = entity->texture.width;
    entity->rec.height = entity->texture.height;
    entity->fireRate = 0;
    entity->isDraw = false;

    if(entity->entityType == ENEMY_BLOCK && entity->id == 21)
        entity->entityPos = RIGHT_POSITION;
    else if(entity->entityType == ENEMY_BLOCK)
        entity->entityPos = LEFT_POSITION;

    return entity;
}

static bool enemyUpdate(void *entity, float dt){
    Entity *e = (Entity*)entity;
    Vector2 p = {0, 0};
    static float id = 0.000001;

    p.x = e->position.x;
    p.y = e->position.y - (level->start * level->tileMap->tileSet->tileHeight) + level->lowBorder;

    if(p.y > level->highBorder){
        entityDestroy(&level->enemiesLayer, e->id);
        return true;
    }

    if(level->y >= level->start && (p.y + e->rec.height) >= level->lowBorder && p.y <= level->highBorder)
        e->isDraw = true;
    else
        e->isDraw = false;

    if(e->entityType == ENEMY_BLOCK){
        if(e->entityPos == RIGHT_POSITION && e->isDraw == true){
            e->position.x += e->speed * dt;
        }
        else if(e->entityPos == LEFT_POSITION && e->isDraw == true){
            e->position.x -= e->speed * dt;
        }
    }

    if(e->entityType == ENEMY_ZAKO && e->isDraw == true)
        e->fireRate += dt;

    if(e->entityType == ENEMY_ZAKO && e->isDraw == true && e->fireRate > 0.9){
        e->fireRate = 0;
        e->entityPos = CENTER_POSITION;

        float xDist = abs(level->player->position.x + level->player->rec.width / 2 - p.x);
        float yDist = abs(level->player->position.y + level->player->rec.height / 2 - p.y);
        float totalDist = xDist + yDist;

        float vSpeedX = xDist / totalDist;
        float vSpeedY = 1.0 - vSpeedX;

        if(level->player->position.x < p.x)
            vSpeedX *= -1;
        if(level->player->position.y < p.y)
            vSpeedY *= -1;

        eBulletCreate(&level->eBulletsLayer, id, e->rec, e->entityPos, ENEMY_BULLET_ZAKO, (Vector2){vSpeedX, vSpeedY}, (Vector2){p.x, p.y},
                      200, ENEMY_ZAKO_BULLET_DAMAGE, ENEMY_ZAKO_BULLET_HEALTH, ENEMY_BULLET_01);

        PlaySound(getSound(SOUND_02));

        id += 0.000001;
    }

    if(e->entityType == ENEMY_CANNON && e->isDraw == true)
        e->fireRate += dt;

    if(e->entityType == ENEMY_CANNON && e->isDraw && e->fireRate > 1.2){
        e->fireRate = 0;
        TextureName textureName;

        if(e->rotation == 0){
            textureName = ENEMY_BEAM_0;
            e->entityPos = RIGHT_POSITION;
        }

        else if(e->rotation == 90){
            textureName = ENEMY_BEAM_90;
            e->entityPos = UP_POSITION;
        }

        else if(e->rotation == 180){
            textureName = ENEMY_BEAM_180;
            e->entityPos = LEFT_POSITION;
        }

        else if(e->rotation == 270){
            textureName = ENEMY_BEAM_270;
            e->entityPos = DOWN_POSITION;
        }

        eBulletCreate(&level->eBulletsLayer, id, e->rec, e->entityPos, ENEMY_BULLET_CANNON, (Vector2){0, 0}, (Vector2){p.x, p.y},
                      200, ENEMY_CANNON_BULLET_DAMAGE, ENEMY_CANNON_BULLET_HEALTH, textureName);

        PlaySound(getSound(SOUND_01));

        id += 0.000001;
    }

    return false;
}

static void enemyDraw(void *entity){
    Entity *e = (Entity*)entity;

    if(e->isDraw == true){
        Vector2 p = {e->position.x, e->position.y - (level->start * level->tileMap->tileSet->tileHeight) + level->lowBorder};

        // draw health bar
        // background
        DrawRectangle(p.x, p.y - 15, e->rec.width, 10, LIGHTGRAY);
        // health bar
        DrawRectangle(p.x, p.y - 15, (int)((e->currentHealth / e->maxHealth) * e->rec.width), 10, MAROON);
        // lines around background
        DrawRectangleLines(p.x, p.y - 15, e->rec.width, 10, GRAY);

        // draw entity
        DrawTextureRec(e->texture, e->rec, p, RAYWHITE);
    }
}

static void enemyDestroy(void *entity){
    Entity *e = (Entity*)entity;
}
































