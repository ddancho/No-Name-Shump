#ifndef LAYER_
#define LAYER_

#include "entity.h"

typedef struct layer{
    Entity entity;
    struct layer *next;
} Layer;

void layerUpdate(Layer *head, float dt);
void layerDraw(Layer *head);
void layerDestroy(Layer **head);

unsigned int getLayerSize(Layer *head);

Entity* getEntityByIndex(Layer *head, unsigned int index);
Entity* getEntityById(Layer *head, float id);

Entity * entityCreate(Layer **head, float id, EntityType entityType);
void entityDestroy(Layer **head, float id);

#endif // LAYER_
