#include <stdio.h>
#include <raylib.h>

#include "lost.h"
#include "game_stage.h"
#include "player.h"
#include "level.h"
#include "res_manager.h"

extern GameStage currentStage;
extern Player *player;
extern Level *level;

static Rectangle goBack = {250, 450, 260, 100};
static Rectangle quit = {250, 560, 260, 100};

void lostUpdate(void){
    UpdateMusicStream(getMusic(MUSIC_01));

    if(CheckCollisionPointRec(GetMousePosition(), goBack) && (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_B))){
        currentStage = MENU_STAGE;

        playerDestroy(player);
        player = NULL;

        levelDestroy(level);
        level = NULL;

        PauseMusicStream(getMusic(MUSIC_01));
        ResumeMusicStream(getMusic(MUSIC_02));
    }

    if(CheckCollisionPointRec(GetMousePosition(), quit) && (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_Q))){
        currentStage = QUIT_STAGE;
    }
}

void lostDraw(void){
    DrawTexture(player->screenshotTexture, 0, 0, RAYWHITE);

    DrawRectangleLines(goBack.x, goBack.y, goBack.width, goBack.height, RED); DrawText("GO BACK", 285, 480, 48, RED);
    DrawRectangleLines(quit.x, quit.y, quit.width, quit.height, RED);         DrawText("QUIT", 285, 590, 48, RED);
}
