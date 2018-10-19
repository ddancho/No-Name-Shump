#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

Vector2 * allocMemForTexCoords(unsigned int cols, unsigned int rows, unsigned int width, unsigned int height){
    Vector2 *texCoords = NULL;

    texCoords = malloc(cols * rows * sizeof(Vector2));
    if(texCoords == NULL){
        fprintf(stderr, "allocMemForTexCoords err...\n");
        return NULL;
    }

    int i = 0;
    for(int y = 0; y < rows; y++){
        for(int x = 0; x < cols; x++){
            texCoords[i].x = x * width;
            texCoords[i].y = y * height;
            i++;
        }
    }

    return texCoords;
}

void getStringValue(const char *buffer, char *dst){
    unsigned int i = 0;
    char *p = NULL;

    // find last space
    p = strrchr(buffer, 32);
    // go next next chr
    p++;
    p++;
    // copy until '"'
    while((*p) != '"'){
        dst[i] = *p;
        p++;
        i++;
    }

    dst[i] = '\0';
}

unsigned int getUIntValue(const char *buffer, const char delimiter){
    unsigned int i = 0;
    char d[64] = {0};
    char *p = NULL;

    // find last space
    p = strrchr(buffer, 32);
    // go next chr
    p++;
    // copy until delimiter
    while((*p) != delimiter){
        d[i] = (*p);
        p++;
        i++;
    }
    d[i] = '\0';

    return atoi(d);
}

