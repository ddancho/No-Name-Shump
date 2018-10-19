#ifndef RES_SOUND_
#define RES_SOUND_

// SOUNDS
// sound_01_laser_01
// sound_02_laser_09
// sound_03_laser_shoot_01
// sound_04_explosion_06_hit
// sound_05_explosion_18

typedef enum{
    SOUND_01 = 0,
    SOUND_02,
    SOUND_03,
    SOUND_04,
    SOUND_05
} SoundName;

typedef struct{
    Sound sound;
    SoundName soundName;
    const char *fileName;
} SoundInfo;

#endif // RES_SOUND_
