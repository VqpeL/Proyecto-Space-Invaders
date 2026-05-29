#include <raylib.h>
#include "game.h"

int main(){
    Color gris{29, 29, 27, 255};

    InitWindow(ANCHO_VENT, ALTO_VENT, "Space Invaders");
    SetTargetFPS(FPS);

    Font fuente = LoadFont("assets/fuentes/Arcade2.otf");
    while(WindowShouldClose() == false){

        BeginDrawing();
        ClearBackground(gris);
        DrawTextEx(
            fuente,
            "SPACE INVADERS",
            Vector2{40,180},
            60,
            2,
            GREEN
        );

        EndDrawing();
    }
    CloseWindow();

    return 0;
}