#include <stdio.h>

#include "bullet.h"
#include "player.h"
#include "level.h"
#include "res_manager.h"

extern Level *level;

static bool bulletUpdate(void *entity, float dt);
static void bulletDraw(void *entity);
static void bulletDestroy(void *entity);

void bulletCreate(Layer **head, float id, EntityType entityType,float positionX, float positionY, float damage, float health){
    Entity *entity = entityCreate(head, id, entityType);

    entity->update = bulletUpdate;
    entity->draw = bulletDraw;
    entity->destroy = bulletDestroy;

    entity->texture = getTexture(BULLET_01);
    entity->speed = 300;
    entity->rotation = 0;
    entity->scale = 1.0;
    entity->damage = damage;
    entity->currentHealth = health;
    entity->maxHealth = health;
    entity->rec.x = 0;
    entity->rec.y = 0;
    entity->rec.width = entity->texture.width;
    entity->rec.height = entity->texture.height;
    entity->position.x = positionX - entity->rec.width / 2;
    entity->position.y = positionY - entity->rec.height / 2;
    entity->isDraw = true;
}

static bool bulletUpdate(void *entity, float dt){
    Entity *e = (Entity*)entity;

    e->position.y -= e->speed * dt;

    if(e->position.y < level->infoBorder){
        e->isDraw = false;
        entityDestroy(&level->player->bulletsLayer, e->id);
        return true;
    }

    return false;
}

static void bulletDraw(void *entity){
    Entity *e = (Entity*)entity;

    DrawTextureRec(e->texture, e->rec, e->position, RAYWHITE);
}

static void bulletDestroy(void *entity){
    Entity *e = (Entity*)entity;
}




































