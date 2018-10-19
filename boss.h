#ifndef BOSS_
#define BOSS_

#include <raylib.h>

#define GO_FORTH    1
#define GO_BACK     (-1)

typedef struct path{
    Vector2 *nodes;
} Path;

typedef struct shield{

    float health;
    float damage;

    float rotation;
    float rotationSpeed;
    float rotationRate;

    Vector2 position;
    Texture2D texture;
    Rectangle rec;
} Shield;

typedef struct boss{

    Path *path;
    int currentNode;
    int maxNode;
    int pathDirection;

    Shield *shield;

    float fireRate;
    float rFireRate;

    float health;
    float damage;

    float speed;

    char *screenshotFileName;
    Texture2D screenshotTexture;

    Vector2 position;

    Texture2D texture;
    Rectangle rec;
} Boss;

Boss* bossCreate(void);
void bossUpdate(Boss *boss, float dt);
void bossDraw(Boss *boss);
void bossDestroy(Boss *boss);

#endif // BOSS_
