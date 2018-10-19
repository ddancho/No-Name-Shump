#ifndef ENEMY_BULLET_
#define ENEMY_BULLET_

#include "layer.h"
#include "res_textures.h"

void eBulletCreate(Layer **head, float id, Rectangle entityRec, EntityPosition entityPos, EntityType entityType,
                   Vector2 vSpeed, Vector2 position, float speed, float damage, float health, TextureName textureName);

#endif // ENEMY_BULLET_
