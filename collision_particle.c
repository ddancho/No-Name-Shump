#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "collision_particle.h"
#include "res_manager.h"

static bool cParticleUpdate(void *particle, Vector2 position, float dt);
static void cParticleDraw(void *particle);
static void cParticleDestroy(void *particle);

void cParticleCreate(ParticleLayer **head, float id, int blendMode, int maxCount, float gravity,
                     bool randomColor, Color defaultColor, TextureName textureName){
    Particle *particle = particleCreate(head, id);

    particle->update = cParticleUpdate;
    particle->draw = cParticleDraw;
    particle->destroy = cParticleDestroy;

    particle->blendMode = blendMode;
    particle->maxCount = maxCount;
    particle->gravity = gravity;
    particle->texture = getTexture(textureName);
    particle->defaultColor = defaultColor;
    particle->drawRate = 0;
    particle->isDraw = true;
    particle->particleData = malloc(sizeof(ParticleData) * maxCount);

    for(int i = 0; i < maxCount; i++){
        particle->particleData[i].alpha = 1.0;
        particle->particleData[i].rotation = GetRandomValue(0, 360);
        if(randomColor == true)
            particle->particleData[i].color = (Color){GetRandomValue(0, 255), GetRandomValue(0, 255), GetRandomValue(0, 255), 255};
        else
            particle->particleData[i].color = particle->defaultColor;

        particle->particleData[i].isActive = false;
    }

}

static bool cParticleUpdate(void *particle, Vector2 position, float dt){
    Particle *p = (Particle*)particle;

    if(p->drawRate > 0){
        p->drawRate -= dt;

        for(int i = 0; i < p->maxCount; i++){
            if(p->particleData[i].isActive == false){
                p->particleData[i].isActive = true;
                p->particleData[i].alpha = 1.0;
                p->particleData[i].position = position;
                break;
            }
        }

        for(int i = 0; i < p->maxCount; i++){
            if(p->particleData[i].isActive == true){
                p->particleData[i].alpha -= 0.01;
                p->particleData[i].rotation += 5.0;
                p->particleData[i].position.y += p->gravity;

                if(p->particleData[i].alpha <= 0.0)
                    p->particleData[i].isActive = false;
            }
        }
    }

    return false;
}

static void cParticleDraw(void *particle){
    Particle *p = (Particle*)particle;

    if(p->drawRate > 0){
        for(int i = 0; i < p->maxCount; i++){
            if(p->particleData[i].isActive == true){

                BeginBlendMode(p->blendMode);

                DrawTextureEx(p->texture, p->particleData[i].position, p->particleData[i].rotation, 1.5,
                              Fade(p->particleData[i].color, p->particleData[i].alpha));

                EndBlendMode();
            }
        }
    }
    else if(p->drawRate < 0){
        for(int i = 0; i < p->maxCount; i++){
            if(p->particleData[i].isActive == true){
                p->particleData[i].alpha = 0;

                BeginBlendMode(p->blendMode);

                DrawTextureEx(p->texture, p->particleData[i].position, p->particleData[i].rotation, 1.0,
                              Fade(p->particleData[i].color, p->particleData[i].alpha));

                p->particleData[i].isActive = false;

                EndBlendMode();
            }
        }
    }
}

static void cParticleDestroy(void *particle){
    Particle *p = (Particle*)particle;

    free(p->particleData);
}











