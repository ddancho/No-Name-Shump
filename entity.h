#ifndef ENTITY_
#define ENTITY_

#include <raylib.h>

typedef enum{
    PLAYER_BULLET = 0,
    PLAYER_BULLET_HIT_FX,
    ENEMY_BULLET_CANNON,
    ENEMY_BULLET_CANNON_HIT_FX,
    ENEMY_BULLET_ZAKO,
    ENEMY_BULLET_ZAKO_HIT_FX,
    ENEMY_BULLET_BOSS_ROTATOR_FX,
    ENEMY_BULLET_BOSS_ROTATOR_HIT_FX,
    ENEMY_BULLET_BOSS_ROTATOR_DESTROYED_FX,
    ENEMY_BULLET_BOSS,
    ENEMY_BULLET_BOSS_HIT_FX,
    ENEMY_BULLET_BOSS_DESTROYED_FX,
    ENEMY_ZAKO,
    ENEMY_ZAKO_DESTROYED_FX,
    ENEMY_BLOCK,
    ENEMY_CANNON,
    ENEMY_CANNON_DESTROYED_FX,
    COLLISION_TILE,
    PLAYER_DESTROYED_FX,
    BOSS_DESTROYED_FX
} EntityType;

typedef enum{
    UP_POSITION = 0,
    DOWN_POSITION,
    LEFT_POSITION,
    RIGHT_POSITION,
    CENTER_POSITION
} EntityPosition;

typedef bool (*Update)(void*, float);
typedef void (*Draw)(void*);
typedef void (*Destroy)(void*);

typedef struct entity{
    float id;
    //
    EntityType entityType;
    EntityPosition entityPos;
    //
    unsigned int tileID;    // for collision direction
    //
    bool isDraw;
    //
    Update update;
    Draw draw;
    Destroy destroy;
    //
    float animationIndex;
    float animationSpeed;
    bool isAnimation;
    Vector2 *texCoords;     // free
    //
    unsigned int cols;
    unsigned int rows;
    //
    float damage;
    float currentHealth;
    float maxHealth;
    float score;
    //
    float fireRate;
    float speed;
    float rotation;
    float scale;
    Vector2 vSpeed;
    //
    Vector2 position;
    Rectangle rec;
    Texture2D texture;
} Entity;

#endif // ENTITY_
































