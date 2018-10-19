#ifndef COLLISION_TILE_
#define COLLISION_TILE_

#include "layer.h"

Entity* collisionTileCreate(Layer **head, float id, EntityType entityType, unsigned int width, unsigned int height, int tileID, bool isDraw);

#endif // COLLISION_TILE_
