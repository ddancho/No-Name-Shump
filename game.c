#include <stdio.h>
#include <raylib.h>

#include "game.h"
#include "game_stage.h"
#include "menu.h"
#include "lost.h"
#include "win.h"
#include "player.h"
#include "level.h"
#include "res_manager.h"

extern GameStage currentStage;
extern bool running;

static Texture2D menuTexture;

const unsigned int SCREEN_WIDTH = 720;
const unsigned int SCREEN_HEIGHT = 960;

Player *player = NULL;
Level *level = NULL;

int gameCreate(int targetFPS){
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "NoNameShump 0.38");
    if(targetFPS > 0)
        SetTargetFPS(targetFPS);

    if(resourceCreate() == false){
        fprintf(stderr, "Error resource create func...\n");
        CloseWindow();
        return -1;
    }

    menuTexture = getTexture(MENU_TEX);

    PlayMusicStream(getMusic(MUSIC_02));
    PauseMusicStream(getMusic(MUSIC_01));

    return 0;
}

void gameUpdate(float dt){
    switch(currentStage){
        case INTRO_STAGE:
            break;

        case MENU_STAGE:{
            menuUpdate();
            break;
        }

        case PLAY_STAGE:{
            levelUpdate(level, dt);
            playerUpdate(player, dt);
            break;
        }

        case WIN_STAGE:
            winUpdate();
            break;

        case LOST_STAGE:
            lostUpdate();
            break;

        case QUIT_STAGE:
            running = false;
            break;
    }
}

void gameDraw(float dt){
    switch(currentStage){
        case INTRO_STAGE:
            break;

        case MENU_STAGE:{
            menuDraw(menuTexture);
            break;
        }

        case PLAY_STAGE:{
            levelDraw(level);
            playerDraw(player);
            DrawFPS(610, 920);
            break;
        }

        case WIN_STAGE:
            winDraw();
            break;

        case LOST_STAGE:
            lostDraw();
            break;

        case QUIT_STAGE:
            break;
    }
}

void gameDestroy(void){
    CloseWindow();
    resourceDestroy();
    playerDestroy(player);
    levelDestroy(level);
}


















































