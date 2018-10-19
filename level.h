#ifndef LEVEL_
#define LEVEL_

#include "tile_map.h"
#include "layer.h"
#include "player.h"
#include "boss.h"

#define LEVEL_ONE 1

typedef struct{
    unsigned int levelNo;
    //
    unsigned int hiScore;
    unsigned int score;
    //
    float start;
    float y;
    float lowBorder;
    float highBorder;
    float infoBorder;
    float scrollSpeed;
    //
    Player *player;
    //
    Boss *boss;
    //
    Layer *enemiesLayer;
    Layer *eBulletsLayer;
    Layer *collisionTilesVsCannonsLayer;
    Layer *collisionTilesVsBlocksLayer;
    Layer *collisionTilesVsPlayer;
    Layer *animationLayer;
    //
    TileMap *tileMap;
    Texture2D texture;
} Level;

Level* levelCreate(Player *player, unsigned int levelNo);
void levelUpdate(Level *level, float dt);
void levelDraw(Level *level);
void levelDestroy(Level *level);

#endif // LEVEL_
