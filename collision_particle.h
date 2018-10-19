#ifndef COLLISION_PARTICLE_
#define COLLISION_PARTICLE_

#include "particle.h"
#include "res_textures.h"

void cParticleCreate(ParticleLayer **head, float id, int blendMode, int maxCount, float gravity,
                     bool randomColor, Color defaultColor, TextureName textureName);

#endif // COLLISION_PARTICLE_
