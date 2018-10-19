#ifndef ENEMY_
#define ENEMY_

#include "layer.h"
#include "res_textures.h"

Entity* enemyCreate(Layer **head, float id, EntityType entityType, Vector2 position, float speed, float rotation,
                 float damage, float health, float score, TextureName textureName);

#endif // ENEMY_
