#ifndef TILE_MAP_
#define TILE_MAP_

#include <raylib.h>

typedef struct{
    float id;
    unsigned int index;
} CollisionTile;

typedef struct{
    unsigned int index;
    bool isCollisionTile;
    Rectangle rec;
    Color color;
} Tile;

typedef struct{
    char name[256];
    char image[256];
    unsigned int firstGid;
    unsigned int tileCount;
    unsigned int tileWidth;
    unsigned int tileHeight;
    unsigned int imageWidth;
    unsigned int imageHeight;
    Tile *tile;                     // free
} TileSet;

typedef struct{
    unsigned int index;
    Vector2 position;
} LayerData;

typedef struct{
    char name[256];
    char type[256];
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
    CollisionTile *collTiles;        // free
    LayerData *layerData;            // free
} MapLayer;

typedef struct{
    unsigned int id;
    char name[256];
    char type[256];
    char shape[256];
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
} MapObject;

typedef struct{
    unsigned int width;
    unsigned int height;
    unsigned int layerCount;
    unsigned int objectCount;
    TileSet *tileSet;       // free
    MapLayer *layer;           // free
    MapObject *object;         // free
} TileMap;                  // free

TileMap * loadTileMap(const char * luaFile);
void freeTileMap(TileMap * tileMap);

#endif // TILE_MAP_


























