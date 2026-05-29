#include <raylib.h>

int main(){
    Color gris{29, 29, 27, 255};
    int alturaVent = 800;
    int anchoVent = 600;

    InitWindow(alturaVent, anchoVent, "Space Invaders");
    SetTargetFPS(60);
    while(WindowShouldClose() == false){

        BeginDrawing();
        ClearBackground(gris);
        EndDrawing();
    }
    CloseWindow();
}