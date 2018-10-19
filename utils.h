#ifndef UTILS_
#define UTILS_

#include <raylib.h>

Vector2 * allocMemForTexCoords(unsigned int cols, unsigned int rows, unsigned int width, unsigned int height);
void getStringValue(const char *buffer, char *dst);
unsigned int getUIntValue(const char *buffer, const char delimiter);

#endif // UTILS_
