#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tile_map.h"
#include "utils.h"

#define TILE01_SIZE 12

static void parseTileMapInfoFile(char *buffer, TileMap *tileMap);
static bool checkString(const char *buffer, const char *text);
static int find(int data, int size, int *array);

static int tile01CollisionTiles[TILE01_SIZE] = {24, 25, 26, 27, 28, 32, 34, 35, 36, 40, 41, 42};

TileMap* loadTileMap(const char *luaFile){
    FILE *file = NULL;
    char *filePtr = NULL;
    long int fileSize = 0;

    file = fopen(luaFile, "r");
    if(file == NULL){
        fprintf(stderr, "fopen err...\n");
        return NULL;
    }
    else
        printf("file %s is open...\n", luaFile);

    // calc file size
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    if(fileSize <= 0){
        fprintf(stderr, "file size err...\n");
        fclose(file);
        return NULL;
    }
    else
        printf("file size %d bytes...\n", (int)fileSize);

    // copy file to mem
    filePtr = malloc(fileSize);
    memset(filePtr, 0, fileSize);
    fread(filePtr, fileSize, 1, file);
    fclose(file);

    TileMap *tileMap = NULL;
    tileMap = malloc(sizeof(TileMap));

    // parse file
    parseTileMapInfoFile(filePtr, tileMap);

    free(filePtr);

    return tileMap;
}

void freeTileMap(TileMap *tileMap){
    if(tileMap != NULL){
        if(tileMap->tileSet->tile != NULL)
            free(tileMap->tileSet->tile);
        if(tileMap->tileSet != NULL)
            free(tileMap->tileSet);
        if(tileMap->layer->layerData != NULL)
            free(tileMap->layer->layerData);
        if(tileMap->layer != NULL)
            free(tileMap->layer);
        if(tileMap->object != NULL)
            free(tileMap->object);
        free(tileMap);
    }
}

static void parseTileMapInfoFile(char *buffer, TileMap *tileMap){
    unsigned int width = 0;
    unsigned int height = 0;
    char *t = strtok(buffer, ",");

    // global info
    while(1){
        // width
        if(checkString(t, " width ") == true){
            tileMap->width = getUIntValue(t, ',');
        }
        // height
        if(checkString(t, " height ") == true){
            tileMap->height = getUIntValue(t, ',');
        }

        t = strtok(0, ",");
        // break on tilesets token
        if(checkString(t, "tilesets") == true)
            break;
    }

    // tilesets
    tileMap->tileSet = malloc(sizeof(TileSet));
    while(1){
        // name
        if(checkString(t, " name ") == true){
            getStringValue(t, tileMap->tileSet->name);
        }
        // firstGid
        if(checkString(t, "firstgid") == true){
            tileMap->tileSet->firstGid = getUIntValue(t, ',');
        }
        // tileWidth
        if(checkString(t, "tilewidth") == true){
            tileMap->tileSet->tileWidth = getUIntValue(t, ',');
        }
        // tileHeight
        if(checkString(t, "tileheight") == true){
            tileMap->tileSet->tileHeight = getUIntValue(t, ',');
        }
        // image
        if(checkString(t, " image ") == true){
            getStringValue(t, tileMap->tileSet->image);
        }
        // imageWidth
        if(checkString(t, "imagewidth") == true){
            tileMap->tileSet->imageWidth = getUIntValue(t, ',');
        }
        // imageHeight
        if(checkString(t, "imageheight") == true){
            tileMap->tileSet->imageHeight = getUIntValue(t, ',');
        }
        // tileCount
        if(checkString(t, "tilecount") == true){
            tileMap->tileSet->tileCount = getUIntValue(t, ',');
        }

        t = strtok(0, ",");

        // break on layers token
        if(checkString(t, "layers"))
            break;
    }
    tileMap->tileSet->tile = malloc(sizeof(Tile) * tileMap->tileSet->tileCount);
    width = tileMap->tileSet->imageWidth / tileMap->tileSet->tileWidth;
    height = tileMap->tileSet->imageHeight / tileMap->tileSet->tileHeight;

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            tileMap->tileSet->tile[x + (y * width)].index = x + (y * width);
            tileMap->tileSet->tile[x + (y * width)].rec.x = x * tileMap->tileSet->tileWidth;
            tileMap->tileSet->tile[x + (y * width)].rec.y = y * tileMap->tileSet->tileHeight;
            tileMap->tileSet->tile[x + (y * width)].rec.width = tileMap->tileSet->tileWidth;
            tileMap->tileSet->tile[x + (y * width)].rec.height = tileMap->tileSet->tileHeight;
            // is tile collision tile
            if(find(tileMap->tileSet->tile[x + (y * width)].index, TILE01_SIZE, tile01CollisionTiles) >= 0)
                tileMap->tileSet->tile[x + (y * width)].isCollisionTile = true;
            else
                tileMap->tileSet->tile[x + (y * width)].isCollisionTile = false;
        }
    }

    // layers
    int ii = 0;
    char p[256] = {0};
    tileMap->layerCount = 0;
    tileMap->layer = malloc(sizeof(MapLayer));
    while(1){
        // type
        if(checkString(t, " type ") == true){
            getStringValue(t , p);
            // break on "objectgroup" type
            if(checkString(p, "objectgroup") == true){
                break;
            }
            else{
                // more then one layer ?
                if(tileMap->layerCount > 0){
                    tileMap->layer = realloc(tileMap->layer,sizeof(MapLayer)*(tileMap->layerCount + 1));
                    //
                    tileMap->layer[1].collTiles = malloc(sizeof(CollisionTile) * 468);    // HARD CODED <<<<<<<<<<<<<-------------------------
                    //
                }
                // "tilelayer"
                strcpy(tileMap->layer[tileMap->layerCount].type, p);
            }
        }
        // name
        if(checkString(t, " name ") == true){
            getStringValue(t, tileMap->layer[tileMap->layerCount].name);
        }
        // x
        if(checkString(t, " x ") == true){
            tileMap->layer[tileMap->layerCount].x = getUIntValue(t, ',');
        }
        // y
        if(checkString(t, " y ") == true){
            tileMap->layer[tileMap->layerCount].y = getUIntValue(t, ',');
        }
        // width
        if(checkString(t, " width ") == true){
            tileMap->layer[tileMap->layerCount].width = getUIntValue(t, ',');
        }
        // height
        if(checkString(t, " height ") == true){
            tileMap->layer[tileMap->layerCount].height = getUIntValue(t, ',');
        }
        // data
        if(checkString(t, " data ") == true){
            int index = 0;
            char c = ',';
            width = tileMap->layer[tileMap->layerCount].width;
            height = tileMap->layer[tileMap->layerCount].height;
            tileMap->layer[tileMap->layerCount].layerData = malloc(sizeof(LayerData) * width * height);

            for(int y = 0; y < height; y++){
                for(int x = 0; x < width; x++){
                    index = getUIntValue(t, c);
                    if(index == 0)
                        tileMap->tileSet->tile[index].color = BLANK;
                    else{
                        index -= tileMap->tileSet->firstGid;
                        tileMap->tileSet->tile[index].color = WHITE;
                    }

                    if(tileMap->layerCount == 1 && tileMap->tileSet->tile[index].isCollisionTile == true){
                        tileMap->layer[1].collTiles[ii].id = x + (y * width);
                        tileMap->layer[1].collTiles[ii].index = index;
                        ii++;
                    }

                    tileMap->layer[tileMap->layerCount].layerData[x + (y * width)].index = index;
                    tileMap->layer[tileMap->layerCount].layerData[x + (y * width)].position.x = x * tileMap->tileSet->tileWidth;
                    tileMap->layer[tileMap->layerCount].layerData[x + (y * width)].position.y = y * tileMap->tileSet->tileHeight;
                    if((x == (width - 2)) && (y == (height - 1))){
                        t = strtok(0, "\n");
                        c = '\n';
                    }
                    else{
                        t = strtok(0, ",");
                        c = ',';
                    }
                }
            }
            tileMap->layerCount++;
        }
        t = strtok(0, ",");
    }
    // objectgroup
    // find the start of the list of objects
    while(1){
        t = strtok(0, "\n");
        if(checkString(t, " objects ") == true)
            break;
    }
    tileMap->objectCount = 0;
    tileMap->object = malloc(sizeof(MapObject));
    // start of the list
    while(1){
        // id
        if(checkString(t, " id ") == true){
            tileMap->object[tileMap->objectCount].id = getUIntValue(t,',');
        }
        // name
        if(checkString(t, " name ") == true){
            getStringValue(t, tileMap->object[tileMap->objectCount].name);
        }
        // type
        if(checkString(t, " type ") == true){
            getStringValue(t, tileMap->object[tileMap->objectCount].type);
        }
        // shape
        if(checkString(t, " shape ") == true){
            getStringValue(t, tileMap->object[tileMap->objectCount].shape);
        }
        // x
        if(checkString(t, " x ") == true){
            tileMap->object[tileMap->objectCount].x = getUIntValue(t, ',');
        }
        // y
        if(checkString(t, " y ") == true){
            tileMap->object[tileMap->objectCount].y = getUIntValue(t, ',');
        }
        // width
        if(checkString(t, " width ") == true){
            tileMap->object[tileMap->objectCount].width = getUIntValue(t, ',');
        }
        // height
        if(checkString(t, " height ") == true){
            tileMap->object[tileMap->objectCount].height = getUIntValue(t, ',');
        }
        // properties
        if(checkString(t, " properties ") == true){
           tileMap->objectCount++;
           tileMap->object = realloc(tileMap->object, sizeof(MapObject) * (tileMap->objectCount + 1));
        }

        t = strtok(0, ",");
        if(t == NULL){
            break;
        }
    }
}

static bool checkString(const char *buffer, const char *text){
    if(strstr(buffer, text) != NULL)
        return true;

    return false;
}

// binary search
static int find(int data, int size, int *array){
    int lowerBound = 0;
    int upperBound = size - 1;
    int midPoint = 0;
    int index = -1;

    while(lowerBound <= upperBound){
        midPoint = lowerBound + (upperBound - lowerBound) / 2;

        if(array[midPoint] == data){
            index = midPoint;
            break;
        }
        else{
            if(array[midPoint] < data)
                lowerBound = midPoint + 1;
            else
                upperBound = midPoint - 1;
        }
    }

    return index;
}
























































