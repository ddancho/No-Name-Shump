#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "animation.h"
#include "level.h"
#include "player.h"
#include "utils.h"
#include "level_one.h"
#include "res_manager.h"

extern Level *level;

static void adjustStartPosition(Entity *anim, Vector2 position, Rectangle entityRec, EntityPosition entityPos);

static bool animationUpdate(void *entity, float dt);
static void animationDraw(void *entity);
static void animationDestroy(void *entity);

void animationCreate(Layer **head, float id, Vector2 position, Rectangle entityRec, EntityPosition entityPos,
                     EntityType entityType, float animationSpeed, unsigned int cols, unsigned int rows, AnimationName animationName){

    Entity *anim = entityCreate(head, id, entityType);

    anim->update = animationUpdate;
    anim->draw = animationDraw;
    anim->destroy = animationDestroy;

    anim->entityType = entityType;
    anim->texture = getAnimation(animationName);
    anim->cols = cols;
    anim->rows = rows;
    anim->rec.x = 0;
    anim->rec.y = 0;
    anim->rec.width = anim->texture.width / cols;
    anim->rec.height = anim->texture.height / rows;
    anim->animationIndex = 0;
    anim->animationSpeed = animationSpeed;
    anim->texCoords = allocMemForTexCoords(cols, rows, anim->rec.width, anim->rec.height);
    anim->isAnimation = true;

    if(entityType == ENEMY_BULLET_BOSS_ROTATOR_FX){
        anim->damage = ENEMY_BOSS_ROTATOR_DAMAGE;
        anim->currentHealth = ENEMY_BOSS_ROTATOR_HEALTH;
        anim->maxHealth = ENEMY_BOSS_ROTATOR_HEALTH;
    }

    adjustStartPosition(anim, position, entityRec, entityPos);
}

static bool animationUpdate(void *entity, float dt){
    Entity *e = (Entity*)entity;

    if(e->position.y > level->highBorder && e->entityType == ENEMY_BULLET_BOSS_ROTATOR_FX ){
        entityDestroy(&level->animationLayer, e->id);
        return true;
    }

    if(e->isAnimation == false){
        entityDestroy(&level->animationLayer, e->id);
        return true;
    }
    else
        e->animationIndex += e->animationSpeed * dt;

    if(e->animationIndex > e->cols * e->rows && e->entityType == ENEMY_BULLET_BOSS_ROTATOR_FX)
        e->animationIndex = 0;

    else if(e->animationIndex > e->cols * e->rows)
        e->isAnimation = false;

    if(e->isAnimation == true){
        e->rec.x = e->texCoords[((int)e->animationIndex)].x;
        e->rec.y = e->texCoords[((int)e->animationIndex)].y;

        if(e->entityType == ENEMY_BULLET_CANNON_HIT_FX){
            if(e->entityPos == RIGHT_POSITION || e->entityPos == LEFT_POSITION){
                e->position.y += 30 * dt;
            }
        }
        else if(e->entityType == ENEMY_CANNON_DESTROYED_FX)
            e->position.y += 30 * dt;

        else if(e->entityType == ENEMY_BULLET_BOSS_ROTATOR_FX)
            e->position.y += 200 * dt;

        else if(e->entityType == PLAYER_BULLET_HIT_FX)
            e->position.y += 30 * dt;
    }

    return false;
}

static void animationDraw(void *entity){
    Entity *e = (Entity*)entity;

    if(e->isAnimation == true){
        DrawTextureRec(e->texture, e->rec, e->position, RAYWHITE);
    }
}

static void animationDestroy(void *entity){
    Entity *e = (Entity*)entity;

    if(e->texCoords != NULL)
        free(e->texCoords);
}

static void adjustStartPosition(Entity *anim, Vector2 position, Rectangle entityRec, EntityPosition entityPos){
    if(anim->entityType == ENEMY_BULLET_CANNON_HIT_FX){
        if(entityPos == RIGHT_POSITION){
            anim->position.x = position.x - anim->rec.width + entityRec.width;
            anim->position.y = position.y - anim->rec.height / 2 + entityRec.height / 2;
            anim->entityPos = RIGHT_POSITION;
        }
        else if(entityPos == LEFT_POSITION){
            anim->position.x = position.x;
            anim->position.y = position.y - anim->rec.height / 2 + entityRec.height / 2;
            anim->entityPos = LEFT_POSITION;
        }
        else if(entityPos == DOWN_POSITION){
            anim->position.x = position.x - anim->rec.width / 2 + entityRec.width / 2;
            anim->position.y = position.y;
            anim->entityPos = DOWN_POSITION;
        }
        else if(entityPos == UP_POSITION){
            anim->position.x = position.x;
            anim->position.y = position.y;
            anim->entityPos = UP_POSITION;
        }
    }
    else if(anim->entityType == ENEMY_CANNON_DESTROYED_FX){
        if(entityPos == LEFT_POSITION){
            anim->position.x = position.x - 20;
            anim->position.y = position.y + GetRandomValue(-10, 10);
        }
        else if(entityPos == RIGHT_POSITION){
            anim->position.x = position.x;
            anim->position.y = position.y + GetRandomValue(-10, 10);
        }
        else if(entityPos == UP_POSITION){
            anim->position.x = position.x + GetRandomValue(-10, 10);
            anim->position.y = position.y - entityRec.height / 2;
        }
        else if(entityPos == DOWN_POSITION){
            anim->position.x = position.x + GetRandomValue(-10, 10);
            anim->position.y = position.y;
        }
    }
    else if(anim->entityType == ENEMY_BULLET_ZAKO_HIT_FX){
        anim->position.x = position.x;
        anim->position.y = position.y;
    }
    else if(anim->entityType == ENEMY_ZAKO_DESTROYED_FX){
        anim->position.x = position.x - entityRec.width / 2;
        anim->position.y = position.y - entityRec.height / 2;
    }
    else if(anim->entityType == PLAYER_BULLET_HIT_FX){
        anim->position.x = position.x;
        anim->position.y = position.y - entityRec.height / 2;
    }
    else if(anim->entityType == PLAYER_DESTROYED_FX){
        anim->position.x = position.x - entityRec.width / 2 + GetRandomValue(-30, 30);
        anim->position.y = position.y - entityRec.height / 2 + GetRandomValue(-30, 30);
    }
    else if(anim->entityType == ENEMY_BULLET_BOSS_HIT_FX){
        anim->position.x = position.x;
        anim->position.y = position.y;
    }
    else if(anim->entityType == ENEMY_BULLET_BOSS_DESTROYED_FX){
        anim->position.x = position.x + GetRandomValue(-10, 10);
        anim->position.y = position.y + GetRandomValue(-10, 10);
    }
    else if(anim->entityType == ENEMY_BULLET_BOSS_ROTATOR_FX){
        anim->position.x = position.x + 210;
        anim->position.y = position.y + 142;
    }
    else if(anim->entityType == ENEMY_BULLET_BOSS_ROTATOR_HIT_FX){
        anim->position.x = position.x;
        anim->position.y = position.y;
    }
    else if(anim->entityType == ENEMY_BULLET_BOSS_ROTATOR_DESTROYED_FX){
        anim->position.x = position.x;
        anim->position.y = position.y;
    }
    else if(anim->entityType == BOSS_DESTROYED_FX){
        anim->position.x = position.x;
        anim->position.y = position.y;
    }
}
























