#include <raylib.h>
#include "game.h"

int main(){
    Color gris{29, 29, 27, 255};

    InitWindow(ANCHO_VENT, ALTO_VENT, "Space Invaders");
    SetTargetFPS(FPS);
    while(WindowShouldClose() == false){

        BeginDrawing();
        ClearBackground(gris);
        DrawText("SPACE INVADERS", 100, 250, 40, WHITE);

        EndDrawing();
    }
    CloseWindow();
    
    return 0;
}