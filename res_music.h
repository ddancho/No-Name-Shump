#ifndef RES_MUSIC_
#define RES_MUSIC_

// MUSICS
// music_01_Orbital Colossus
// music_01_Space_Idea

typedef enum{
    MUSIC_01 = 0,
    MUSIC_02
} MusicName;

typedef struct{
    Music music;
    MusicName musicName;
    const char *fileName;
} MusicInfo;

#endif // RES_MUSIC_

