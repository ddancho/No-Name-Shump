#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include "game.h"
#include "game_stage.h"

GameStage currentStage = MENU_STAGE;
bool running = true;

int main(int argc, char *argv[]){

    //int targetFPS = 0;

    //targetFPS = atoi(argv[1]);

    if(gameCreate(60) == -1)
        return -1;

    //printf("\ntarget FPS = %d \n", targetFPS);

    while(!WindowShouldClose() && running == true){

        // update
        gameUpdate(GetFrameTime());

        BeginDrawing();

            ClearBackground(RAYWHITE);

            // draw
            gameDraw(GetFrameTime());

        EndDrawing();
    }

    gameDestroy();

    return 0;
}



























