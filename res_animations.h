#ifndef RES_ANIMATIONS_
#define RES_ANIMATIONS_

// ANIMATIONS
// explosion_01_238x36
// explosion_02_20x200
// explosion_03_640x960
// explosion_04_96x96
// explosion_05_128x128
// explosion_06_48x48
// explosion_07_16x16
// explosion_08_48x48
// explosion_09_24x24
// explosion_10_160x160
// rotator_01_240x48
// bullet_hit_01_32x32

typedef enum {
    EXPLOSION_01 = 0,
    EXPLOSION_02,
    EXPLOSION_03,
    EXPLOSION_04,
    EXPLOSION_05,
    EXPLOSION_06,
    EXPLOSION_07,
    EXPLOSION_08,
    EXPLOSION_09,
    EXPLOSION_10,
    ROTATOR_01,
    BULLET_HIT_01
} AnimationName;

typedef struct{
    Texture2D texture;
    AnimationName animationName;
    const char *fileName;
} AnimationInfo;

#endif // RES_ANIMATIONS_
