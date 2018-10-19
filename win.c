#include <stdio.h>
#include <raylib.h>

#include "win.h"
#include "game_stage.h"
#include "player.h"
#include "level.h"

extern GameStage currentStage;
extern Player *player;
extern Level *level;

static Rectangle goBack = {250, 450, 260, 100};
static Rectangle quit = {250, 560, 260, 100};

void winUpdate(void){
    if(CheckCollisionPointRec(GetMousePosition(), goBack) && (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_B))){
        currentStage = MENU_STAGE;

        playerDestroy(player);
        player = NULL;

        levelDestroy(level);
        level = NULL;
    }

    if(CheckCollisionPointRec(GetMousePosition(), quit) && (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_Q))){
        currentStage = QUIT_STAGE;
    }
}

void winDraw(void){
    DrawTexture(level->boss->screenshotTexture, 0, 0, RAYWHITE);

    DrawText("V I C T O R Y", 170, 300, 64, RED);

    DrawRectangleLines(goBack.x, goBack.y, goBack.width, goBack.height, RED); DrawText("GO BACK", 285, 480, 48, RED);
    DrawRectangleLines(quit.x, quit.y, quit.width, quit.height, RED);         DrawText("QUIT", 285, 590, 48, RED);
}




