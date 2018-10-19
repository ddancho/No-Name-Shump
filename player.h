#ifndef PLAYER_
#define PLAYER_

#include <raylib.h>

#include "layer.h"
#include "collision_particle.h"
#include "entity.h"

typedef struct{
    float currentHealth;
    float maxHealth;
    float playerDamage;
    //
    float bulletHealth;
    float bulletDamage;
    //
    float speed;
    //
    Layer *bulletsLayer;
    //
    bool isCollision;
    Entity *collisionTile;
    //
    float particleID;
    ParticleLayer *collisionParticlesLayer;
    Vector2 collisionPosition;
    //
    char *screenshotFileName;
    Texture2D screenshotTexture;
    //
    Vector2 position;
    Texture2D texture;
    Rectangle rec;
} Player;

Player* playerCreate(void);
void playerUpdate(Player *player, float dt);
void playerDraw(Player *player);
void playerDestroy(Player *player);

#endif // PLAYER_

