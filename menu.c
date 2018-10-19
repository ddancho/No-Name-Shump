#include <stdio.h>
#include <raylib.h>

#include "menu.h"
#include "game_stage.h"
#include "player.h"
#include "level.h"
#include "res_manager.h"

extern GameStage currentStage;
extern Player *player;
extern Level *level;

static Rectangle play = {250, 450, 200, 100};
static Rectangle quit = {250, 560, 200, 100};

void menuUpdate(void){
    UpdateMusicStream(getMusic(MUSIC_02));

    if(CheckCollisionPointRec(GetMousePosition(), play) && (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_P))){
        currentStage = PLAY_STAGE;
        player = playerCreate();
        level = levelCreate(player, LEVEL_ONE);
        PlayMusicStream(getMusic(MUSIC_01));

        PauseMusicStream(getMusic(MUSIC_02));
        ResumeMusicStream(getMusic(MUSIC_01));
    }

    if(CheckCollisionPointRec(GetMousePosition(), quit) && (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_Q))){
        currentStage = QUIT_STAGE;
    }
}

void menuDraw(Texture2D menuTexture){
    DrawTexture(menuTexture, 0, 0, RAYWHITE);
    DrawRectangle(play.x, play.y, play.width, play.height, DARKBLUE);DrawText("PLAY", 285, 480, 48, BLACK);
    DrawRectangle(quit.x, quit.y, quit.width, quit.height, DARKBLUE);DrawText("QUIT", 285, 590, 48, BLACK);
}


























































