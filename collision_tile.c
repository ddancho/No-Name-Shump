#include <stdio.h>
#include <raylib.h>

#include "collision_tile.h"

static bool collisionTileUpdate(void *entity, float dt);
static void collisionTileDraw(void *entity);
static void collisionTileDestroy(void *entity);

Entity* collisionTileCreate(Layer **head, float id, EntityType entityType, unsigned int width, unsigned int height, int tileID, bool isDraw){

    Entity *entity = entityCreate(head, id, entityType);

    entity->update = collisionTileUpdate;
    entity->draw = collisionTileDraw;
    entity->destroy = collisionTileDestroy;

    entity->entityType = entityType;
    entity->position.x = 0;
    entity->position.y = 0;
    entity->rec.x = 0;
    entity->rec.y = 0;
    entity->rec.width = width;
    entity->rec.height = height;
    entity->isDraw = isDraw;

    if(tileID >= 0)
        entity->tileID = tileID;

    return entity;
}

static bool collisionTileUpdate(void *entity, float dt){
    return false;
}

static void collisionTileDraw(void *entity){
    ;
}

static void collisionTileDestroy(void *entity){
    ;
}
















