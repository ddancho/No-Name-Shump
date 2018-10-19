#include <stdio.h>
#include <raylib.h>

#include "enemy_bullet.h"
#include "level.h"
#include "res_manager.h"

extern Level *level;
extern const unsigned int SCREEN_WIDTH;

static void adjustStartPosition(Entity *entity, Rectangle eRec);

static bool eBulletUpdate(void *entity, float dt);
static void eBulletDraw(void *entity);
static void eBulletDestroy(void *entity);

void eBulletCreate(Layer **head, float id, Rectangle entityRec, EntityPosition entityPos, EntityType entityType,
                   Vector2 vSpeed, Vector2 position, float speed, float damage, float health, TextureName textureName){

    Entity *entity = entityCreate(head, id, entityType);

    entity->update = eBulletUpdate;
    entity->draw = eBulletDraw;
    entity->destroy = eBulletDestroy;

    entity->damage = damage;
    entity->currentHealth = health;
    entity->maxHealth = health;
    //
    entity->entityPos = entityPos;
    entity->position = position;
    entity->vSpeed = vSpeed;
    entity->speed = speed;
    entity->scale = 1.0;
    entity->isDraw = true;
    entity->texture = getTexture(textureName);

    if(entityType == ENEMY_BULLET_BOSS){
        entity->rec.x = 458;
        entity->rec.y = 10;
        entity->rec.width = 48;
        entity->rec.height = 48;
    }
    else{
        entity->rec.x = 0;
        entity->rec.y = 0;
        entity->rec.width = entity->texture.width;
        entity->rec.height = entity->texture.height;
    }

    adjustStartPosition(entity, entityRec);
}

static bool eBulletUpdate(void *entity, float dt){
    Entity *e = (Entity*)entity;

    // ENEMY_BULLET_CANNON
    if(e->entityType == ENEMY_BULLET_CANNON){
        if(e->entityPos == RIGHT_POSITION){
            e->position.x += e->speed * dt; // 0.016667
            e->position.y += 30 * dt;
        }

        else if(e->entityPos == LEFT_POSITION){
            e->position.x -= e->speed * dt;
            e->position.y += 30 * dt;
        }

        else if(e->entityPos == UP_POSITION)
            e->position.y += e->speed * dt;

        else if(e->entityPos == DOWN_POSITION)
            e->position.y -= e->speed * dt;
    }
    // ENEMY_BULLET_ZAKO
    else if(e->entityType == ENEMY_BULLET_ZAKO){
        if(e->entityPos == CENTER_POSITION){
            e->position.x += e->vSpeed.x * e->speed * dt;
            e->position.y += e->vSpeed.y * e->speed * dt;
        }
    }
    // ENEMY_BULLET_BOSS
    else if(e->entityType == ENEMY_BULLET_BOSS){
        e->position.y += e->speed * dt;
    }

    if(e->position.y + e->rec.height < level->lowBorder || e->position.y > level->highBorder ||
       e->position.x + e->rec.width < 0 || e->position.x > SCREEN_WIDTH){
        e->isDraw = false;
        entityDestroy(&level->eBulletsLayer, e->id);
        return true;
    }

    return false;
}

static void eBulletDraw(void *entity){
    Entity *e = (Entity*)entity;

    DrawTextureRec(e->texture, e->rec, e->position, RAYWHITE);
}

static void eBulletDestroy(void *entity){
    Entity *e = (Entity*)entity;
}

static void adjustStartPosition(Entity *entity, Rectangle eRec){
    // ENEMY_BULLET_CANNON
    if(entity->entityType == ENEMY_BULLET_CANNON){
        if(entity->entityPos == RIGHT_POSITION){
            entity->position.x = entity->position.x - entity->rec.width / 2 + eRec.width;
            entity->position.y = entity->position.y - entity->rec.height / 2 + eRec.height / 2;
        }
        else if(entity->entityPos == LEFT_POSITION){
            //entity->position.x = entity->position.x;
            entity->position.y = entity->position.y - entity->rec.height / 2 + eRec.height / 2;
        }
        else if(entity->entityPos == UP_POSITION){
            entity->position.x = entity->position.x - entity->rec.width / 2 + eRec.width / 2;
            entity->position.y = entity->position.y - entity->rec.height / 2 + eRec.height;
        }
        else if(entity->entityPos == DOWN_POSITION){
            entity->position.x = entity->position.x - entity->rec.width / 2 + eRec.width / 2;
            //entity->position.y = entity->position.y;
        }
    }
    // ENEMY BULLET ZAKO
    else if(entity->entityType == ENEMY_BULLET_ZAKO){
        if(entity->entityPos == CENTER_POSITION){
            entity->position.x = entity->position.x - entity->rec.width / 2 + eRec.width / 2;
            entity->position.y = entity->position.y - entity->rec.height / 2 + eRec.height / 2;
        }
    }
    // ENEMY_BULLET_BOSS
    else if(entity->entityType == ENEMY_BULLET_BOSS){
        if(entity->entityPos == LEFT_POSITION){
            entity->position.x = entity->position.x + 70;
            entity->position.y = entity->position.y + 110;
        }
        else if(entity->entityPos == RIGHT_POSITION){
            entity->position.x = entity->position.x + 334;
            entity->position.y = entity->position.y + 110;
        }
    }
}



















