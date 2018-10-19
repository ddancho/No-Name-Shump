#ifndef RES_MANAGER_
#define RES_MANAGER_

#include "res_textures.h"
#include "res_animations.h"
#include "res_music.h"
#include "res_sound.h"

bool resourceCreate(void);
void resourceDestroy(void);
Texture2D getTexture(TextureName textureName);
Texture2D getAnimation(AnimationName animationName);
Music getMusic(MusicName musicName);
Sound getSound(SoundName soundName);

#endif // RES_MANAGER_
