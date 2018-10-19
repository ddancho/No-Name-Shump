#include <stdio.h>
#include <raylib.h>

#include "res_textures.h"
#include "res_animations.h"
#include "res_music.h"
#include "res_manager.h"

#define NUM_OF_TEXTURES     18
#define NUM_OF_ANIMATIONS   12
#define NUM_OF_MUSICS       2
#define NUM_OF_SOUNDS       5

static TextureInfo textures[NUM_OF_TEXTURES] = {    {{}, ENEMY_03,              "resources/enemy/enemy_03_48x48.png"},
                                                    {{}, ENEMY_BEAM_0,          "resources/enemy/enemy_beam_0.png"},
                                                    {{}, ENEMY_BEAM_90,         "resources/enemy/enemy_beam_90.png"},
                                                    {{}, ENEMY_BEAM_180,        "resources/enemy/enemy_beam_180.png"},
                                                    {{}, ENEMY_BEAM_270,        "resources/enemy/enemy_beam_270.png"},
                                                    {{}, ENEMY_LARGE_CANON_0,   "resources/enemy/enemy_large_canon_0.png"},
                                                    {{}, ENEMY_LARGE_CANON_90,  "resources/enemy/enemy_large_canon_90.png"},
                                                    {{}, ENEMY_LARGE_CANON_180, "resources/enemy/enemy_large_canon_180.png"},
                                                    {{}, ENEMY_LARGE_CANON_270, "resources/enemy/enemy_large_canon_270.png"},
                                                    {{}, ENEMY_BLOCK_01,        "resources/enemy/enemy_block_01_64x64.png"},
                                                    {{}, ENEMY_BULLET_01,       "resources/enemy/enemy_bullet_01_16x16.png"},
                                                    {{}, ENEMY_BOSS_01,         "resources/enemy_boss/enemy_boss_01_448x256.png"},
                                                    {{}, RING_01,               "resources/enemy_boss/ring_01_512x512.png"},
                                                    {{}, CLOUD_GREY_SMOKE,      "resources/particle/cloud_grey_smoke_16x16.png"},
                                                    {{}, AIRPLANE_01,           "resources/player/airplane_01_64x64.png"},
                                                    {{}, BULLET_01,             "resources/player/bullet_01_32x32.png"},
                                                    {{}, TILE_01,               "resources/tile_map/tile_01.png"},
                                                    {{}, MENU_TEX,              "resources/menu.png"}
                                               };

static AnimationInfo animations[NUM_OF_ANIMATIONS] = {   {{}, EXPLOSION_01, "resources/animation/explosion_01_238x36.png"},
                                                        {{}, EXPLOSION_02, "resources/animation/explosion_02_20x200.png"},
                                                        {{}, EXPLOSION_03, "resources/animation/explosion_03_640x960.png"},
                                                        {{}, EXPLOSION_04, "resources/animation/explosion_04_96x96.png"},
                                                        {{}, EXPLOSION_05, "resources/animation/explosion_05_128x128.png"},
                                                        {{}, EXPLOSION_06, "resources/animation/explosion_06_48x48.png"},
                                                        {{}, EXPLOSION_07, "resources/animation/explosion_07_16x16.png"},
                                                        {{}, EXPLOSION_08, "resources/animation/explosion_08_48x48.png"},
                                                        {{}, EXPLOSION_09, "resources/animation/explosion_09_24x24.png"},
                                                        {{}, EXPLOSION_10, "resources/animation/explosion_10_160x160.png"},
                                                        {{}, ROTATOR_01,   "resources/animation/rotator_01_240x48.png"},
                                                        {{}, BULLET_HIT_01,"resources/player/bullet_hit_01_32x32.png"}
                                                    };

static MusicInfo musics[NUM_OF_MUSICS] = {   { NULL, MUSIC_01, "resources/music/music_01_orbital_colossus.ogg"},
                                            { NULL, MUSIC_02, "resources/music/music_02_space_idea.ogg"}
                                        };

static SoundInfo sounds[NUM_OF_SOUNDS] = {      {{}, SOUND_01, "resources/sound/sound_01_laser_01.ogg"},
                                                {{}, SOUND_02, "resources/sound/sound_02_laser_09.ogg"},
                                                {{}, SOUND_03, "resources/sound/sound_03_laser_shoot_01.ogg"},
                                                {{}, SOUND_04, "resources/sound/sound_04_explosion_06_hit.ogg"},
                                                {{}, SOUND_05, "resources/sound/sound_05_explosion_18.ogg"}
                                         };

bool resourceCreate(void){
    bool loadAnimations = true;
    bool loadTextures = true;
    bool loadMusics = true;
    bool loadSounds = true;

    InitAudioDevice();

    for(int i = 0; i < NUM_OF_TEXTURES; i++){
        textures[i].texture = LoadTexture(textures[i].fileName);
        if(textures[i].texture.id == 0)
            loadTextures = false;
    }

    for(int i = 0; i < NUM_OF_ANIMATIONS; i++){
        animations[i].texture = LoadTexture(animations[i].fileName);
        if(animations[i].texture.id == 0)
            loadAnimations = false;
    }

    for(int i = 0; i < NUM_OF_MUSICS; i++){
        musics[i].music = LoadMusicStream(musics[i].fileName);
        if(musics[i].music == NULL)
            loadMusics = false;
        else
            SetMusicVolume(musics[i].music, 0.1);
    }

    for(int i = 0; i < NUM_OF_SOUNDS; i++){
        sounds[i].sound = LoadSound(sounds[i].fileName);
        SetSoundVolume(sounds[i].sound, 0.05);
    }

    if(loadTextures == false || loadAnimations == false || loadMusics == false || loadSounds == false)
        return false;

    return true;
}

void resourceDestroy(void){
    for(int i = 0; i < NUM_OF_TEXTURES; i++)
        UnloadTexture(textures[i].texture);

    for(int i = 0; i < NUM_OF_ANIMATIONS; i++)
        UnloadTexture(animations[i].texture);

    for(int i = 0; i < NUM_OF_MUSICS; i++)
        UnloadMusicStream(musics[i].music);

    for(int i = 0; i < NUM_OF_SOUNDS; i++)
        UnloadSound(sounds[i].sound);

    CloseAudioDevice();
}

Texture2D getTexture(TextureName textureName){
    return textures[textureName].texture;
}

Texture2D getAnimation(AnimationName animationName){
    return animations[animationName].texture;
}

Music getMusic(MusicName musicName){
    return musics[musicName].music;
}

Sound getSound(SoundName soundName){
    return sounds[soundName].sound;
}




































