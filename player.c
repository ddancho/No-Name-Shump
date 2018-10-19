#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "player.h"
#include "bullet.h"
#include "game_stage.h"
#include "level.h"
#include "level_one.h"
#include "res_manager.h"

extern GameStage currentStage;

Player* playerCreate(void){
    Player *player = NULL;

    player = malloc(sizeof(Player));
    if(player == NULL){
        fprintf(stderr, "player malloc err...\n");
        return NULL;
    }

    player->playerDamage = PLAYER_DAMAGE;
    player->currentHealth = PLAYER_HEALTH;
    player->maxHealth = PLAYER_HEALTH;
    //
    player->bulletDamage = PLAYER_BULLET_DAMAGE;
    player->bulletHealth = PLAYER_BULLET_HEALTH;
    //
    player->bulletsLayer = NULL;
    player->collisionParticlesLayer = NULL;
    player->screenshotFileName = NULL;
    player->collisionTile = NULL;
    player->isCollision = false;
    player->particleID = 1.0;
    player->speed = 200.0;
    player->texture = getTexture(AIRPLANE_01);
    player->rec.x = 0;
    player->rec.y = 0;
    player->rec.width = player->texture.width;
    player->rec.height = player->texture.height;
    player->position.x = 300;
    player->position.y = 800;
    player->collisionPosition = (Vector2){0, 0};

    cParticleCreate(&player->collisionParticlesLayer, player->particleID, BLEND_ALPHA, 200, 7.0, false, RAYWHITE, CLOUD_GREY_SMOKE);

    return player;
}

void playerUpdate(Player *player, float dt){
    static float id = 0.000001;
    static float fireRate = 0;
    static float ssRate = 0;
    static int width = 0;
    static int height = 0;

    if(player->currentHealth <= 0){
        ssRate += dt;
        if(ssRate >= 1.0){
            ssRate = 0;
            player->screenshotFileName = "playerDeath.png";
            TakeScreenshot(player->screenshotFileName);
            player->screenshotTexture = LoadTexture(player->screenshotFileName);
            currentStage = LOST_STAGE;
            return;
        }
    }

    if(player->currentHealth > 0){

        if(IsKeyDown(KEY_W) == true)
            player->position.y -= player->speed * dt;
        else if(IsKeyDown(KEY_S) == true)
            player->position.y += player->speed * dt;

        if(IsKeyDown(KEY_A) == true)
            player->position.x -= player->speed * dt;
        else if(IsKeyDown(KEY_D) == true)
            player->position.x += player->speed * dt;

        if(IsKeyPressed(KEY_SPACE) == true){
            bulletCreate(&player->bulletsLayer, id, PLAYER_BULLET, player->position.x + player->rec.width / 2, player->position.y,
                          player->bulletDamage, player->bulletHealth);
            PlaySound(getSound(SOUND_03));
            id += 0.000001;
        }
        else if(IsKeyDown(KEY_SPACE) == true){
            fireRate += dt;
            if(fireRate > 0.25){
                fireRate = 0;
                bulletCreate(&player->bulletsLayer, id, PLAYER_BULLET, player->position.x + player->rec.width / 2, player->position.y,
                              player->bulletDamage, player->bulletHealth);
                PlaySound(getSound(SOUND_03));
                id += 0.000001;
            }
        }
        else if(IsKeyUp(KEY_SPACE) == true)
            fireRate = 0;

        if(player->bulletsLayer != NULL)
            layerUpdate(player->bulletsLayer, dt);

        if(player->isCollision == true && player->collisionTile != NULL){

            if(player->collisionTile->tileID == 42 || player->collisionTile->tileID == 34 || player->collisionTile->tileID == 26){
                player->position.x += player->speed * dt;
                width = 0;
                height = 0;
            }

            else if(player->collisionTile->tileID == 40 || player->collisionTile->tileID == 32 || player->collisionTile->tileID == 24){
                player->position.x -= player->speed * dt;
                width = player->rec.width;
                height = 0;
            }

            if(player->collisionTile->tileID == 42 || player->collisionTile->tileID == 41 || player->collisionTile->tileID == 40){
                player->position.y += player->speed * dt;
                width = player->rec.width / 2;
                height = 0;
            }

            else if(player->collisionTile->tileID == 26 || player->collisionTile->tileID == 25 || player->collisionTile->tileID == 24){
                player->position.y -= player->speed * dt;
                width = player->rec.width / 2;
                height = player->rec.height;
            }

            Particle *p = getParticleByID(player->collisionParticlesLayer, player->particleID);
            p->drawRate += dt * 1.5;

            player->isCollision = false;
            player->collisionTile = NULL;
        }

        player->collisionPosition.x = player->position.x + width;
        player->collisionPosition.y = player->position.y + height;

        particleLayerUpdate(player->collisionParticlesLayer, player->collisionPosition, dt);

    }
}

void playerDraw(Player *player){
    if(player->currentHealth > 0){
        DrawTextureRec(player->texture, player->rec, player->position, RAYWHITE);

        if(player->bulletsLayer != NULL)
            layerDraw(player->bulletsLayer);

        particleLayerDraw(player->collisionParticlesLayer);
    }
}

void playerDestroy(Player *player){
    if(player != NULL){
        if(player->bulletsLayer != NULL)
            layerDestroy(&player->bulletsLayer);

        if(player->collisionParticlesLayer != NULL)
            particleLayerDestroy(&player->collisionParticlesLayer);

        if(player->screenshotFileName != NULL){
            remove(player->screenshotFileName);
            player->screenshotFileName = NULL;
            UnloadTexture(player->screenshotTexture);
        }

        free(player);
    }
}
























