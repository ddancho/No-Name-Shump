#ifndef RES_TEXTURES_
#define RES_TEXTURES_

// TEXTURES
// enemy_03_48x48
// enemy_beam_0
// enemy_beam_90
// enemy_beam_180
// enemy_beam_270
// enemy_large_canon_0
// enemy_large_canon_90
// enemy_large_canon_180
// enemy_large_canon_270
// enemy_block_01_64x64
// enemy_bullet_01_16x16
// enemy_boss_01_448x256
// ring_01_512x512
// cloud_grey_smoke_16x16
// airplane_01_64x64
// bullet_01_32x32
// tile_01
// menu

typedef enum {
    ENEMY_03 = 0,
    ENEMY_BEAM_0,
    ENEMY_BEAM_90,
    ENEMY_BEAM_180,
    ENEMY_BEAM_270,
    ENEMY_LARGE_CANON_0,
    ENEMY_LARGE_CANON_90,
    ENEMY_LARGE_CANON_180,
    ENEMY_LARGE_CANON_270,
    ENEMY_BLOCK_01,
    ENEMY_BULLET_01,
    ENEMY_BOSS_01,
    RING_01,
    CLOUD_GREY_SMOKE,
    AIRPLANE_01,
    BULLET_01,
    TILE_01,
    MENU_TEX
} TextureName;

typedef struct{
    Texture2D texture;
    TextureName textureName;
    const char *fileName;
} TextureInfo;

#endif // RES_TEXTURES_































