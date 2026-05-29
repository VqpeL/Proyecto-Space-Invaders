#include <raylib.h>
#include "game.h"

Nave Jugador;

void iniciaNave(Nave &Jugador);
void ActualizarNave(Nave &Jugador);
void DibujarNave(Nave const &Jugador);

int main(){
    Color gris{29, 29, 27, 255};

    InitWindow(ANCHO_VENT, ALTO_VENT, "Space Invaders");
    SetTargetFPS(FPS);

    Font fuente = LoadFont("assets/fuentes/Arcade2.otf");

    iniciaNave(Jugador);
    while(WindowShouldClose() == false){

        ActualizarNave(Jugador);

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

        DibujarNave(Jugador);

        EndDrawing();
    }
    UnloadFont(fuente);
    CloseWindow();

    return 0;
}

void iniciaNave(Nave &Jugador){
    Jugador.alto = 20;
    Jugador.ancho = 40;

    Jugador.x = (ANCHO_VENT/2) - (Jugador.ancho/2);
    Jugador.y = ALTO_VENT -50;

    Jugador.vel = 5;
    Jugador.vidas = 3;

    Jugador.activo = true;
}

void ActualizarNave(Nave &Jugador){
    if(IsKeyDown(KEY_A)){
        Jugador.x -= Jugador.vel;
    }
    if(IsKeyDown(KEY_D)){
        Jugador.x += Jugador.vel;
    }
    
    //Limites

    if(Jugador.x < 0){
        Jugador.x = 0;
    }

    if(Jugador.x + Jugador.ancho > ANCHO_VENT){
        Jugador.x = ANCHO_VENT - Jugador.ancho;
    }
}

void DibujarNave(Nave const &Jugador){
    DrawRectangle(
        Jugador.x,
        Jugador.y, 
        Jugador.ancho, 
        Jugador.alto,
        GREEN
    );
}