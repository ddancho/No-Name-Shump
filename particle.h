#ifndef PARTICLE_
#define PARTICLE_

#include <raylib.h>

typedef bool (*pUpdate)(void*, Vector2, float);
typedef void (*pDraw)(void*);
typedef void (*pDestroy)(void*);

typedef struct{
    bool isActive;
    float alpha;
    float rotation;
    Vector2 position;
    Color color;
} ParticleData;

typedef struct{
    float id;
    //
    float gravity;
    int blendMode;
    int maxCount;
    //
    float drawRate;
    bool isDraw;
    //
    pUpdate update;
    pDraw draw;
    pDestroy destroy;
    //
    Texture2D texture;
    Color defaultColor;
    //
    ParticleData *particleData;
} Particle;

typedef struct p{
    Particle particle;
    struct p *next;
} ParticleLayer;

void particleLayerUpdate(ParticleLayer *head, Vector2 position, float dt);
void particleLayerDraw(ParticleLayer *head);
void particleLayerDestroy(ParticleLayer **head);
//
Particle * particleCreate(ParticleLayer **head, float id);
void particleDestroy(ParticleLayer **head, float id);
//
Particle* getParticleByID(ParticleLayer *head, float id);

#endif // PARTICLE_





















