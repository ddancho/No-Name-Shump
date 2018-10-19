#ifndef ANIMATION_
#define ANIMATION_

#include "layer.h"
#include "res_animations.h"

void animationCreate(Layer **head, float id, Vector2 position, Rectangle entityRec, EntityPosition entityPos,
                      EntityType entityType, float animationSpeed, unsigned int cols, unsigned int rows, AnimationName animationName);

#endif // ANIMATION_
