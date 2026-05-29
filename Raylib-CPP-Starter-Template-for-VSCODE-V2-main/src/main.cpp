#include <raylib.h>
#include <cstdlib>
#include "game.h"


void iniciaNave(Nave &Jugador);
void actualizarNave(Nave &Jugador);
void dibujarNave(Nave const &Jugador);
void iniciaAlienGrid(AlienGrid &grid, int , int);
void actualizarAlienGrid(AlienGrid &grid);
void dibujarAlienGrid(AlienGrid &grid);
void iniciaLasers(ListaLaser &lasers);
void disparoLaser(ListaLaser &lista, float x, float y);
void disparoNave(Nave &Jugador, ListaLaser &lista);
void actualizarLaser(ListaLaser &lista);
void dibujarLasers(const ListaLaser &lista);

int main(){
    Color gris{29, 29, 27, 255};
    Nave Jugador;
    AlienGrid grid;
    ListaLaser lista;

    InitWindow(ANCHO_VENT, ALTO_VENT, "Space Invaders");
    SetTargetFPS(FPS);

    Font fuente = LoadFont("assets/fuentes/Arcade2.otf");

    iniciaNave(Jugador);
    iniciaAlienGrid(grid, 5 ,11);
    iniciaLasers(lista);
    while(WindowShouldClose() == false){

        actualizarNave(Jugador);
        disparoNave(Jugador, lista);
        actualizarLaser(lista);
        actualizarAlienGrid(grid);

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

        dibujarNave(Jugador);
        dibujarAlienGrid(grid);
        dibujarLasers(lista);

        EndDrawing();
    }
    UnloadFont(fuente); // Descargar fuente
    UnloadTexture(Jugador.textura);// Descargar texturas de nave
    for(int i =0; i < 3; i++){
        UnloadTexture(grid.textura[i]); // Descargar texturas de aliens
    }
    for(int i = 0; i < grid.num_filas; i++)free(grid.aliens[i]);
    free(grid.aliens); // Libera la memoria del malloc que se hace en iniciaAlienGrid
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
    Jugador.textura = LoadTexture("assets/graficos/nave.png");
}

void actualizarNave(Nave &Jugador){
    if(IsKeyDown(KEY_A)){
        Jugador.x -= Jugador.vel; //Moviemiento a la izquierda
    }
    if(IsKeyDown(KEY_D)){
        Jugador.x += Jugador.vel; //Moviemiento a la derecha
    }
    
    //Limites

    if(Jugador.x < 0){
        Jugador.x = 0;
    }

    if(Jugador.x + Jugador.ancho > ANCHO_VENT){
        Jugador.x = ANCHO_VENT - Jugador.ancho;
    }
}

void dibujarNave(Nave const &Jugador){//const porque solo se necesita la lectura de los daros del struct
    DrawTexture(
        Jugador.textura,
        Jugador.x,
        Jugador.y, 
        ORANGE
    );
}

void iniciaAlienGrid(AlienGrid &grid, int filas , int col){
    grid.num_col = col;
    grid.num_filas = filas;
    grid.dirX = 1; // Si direccion esta en 1 se mueve a la derecha. Si esta en -1 se mueve a la izquierda
    grid.vel = 2.0f;

    grid.textura[0] = LoadTexture("assets/graficos/alien_1.png");
    grid.textura[1] = LoadTexture("assets/graficos/alien_2.png");
    grid.textura[2] = LoadTexture("assets/graficos/alien_3.png");


    grid.aliens = (Alien**)malloc(filas * (sizeof(Alien*)));

    for(int i = 0; i< filas; i++){
        grid.aliens[i] = (Alien*)malloc(col * (sizeof(Alien)));
        for(int j =0; j<col; j++){
            grid.aliens[i][j].ancho = 30;
            grid.aliens[i][j].alto = 20;

            grid.aliens[i][j].x = j * (30 + 15) + 50;
            grid.aliens[i][j].y = i * (20 + 15) + 50;
            grid.aliens[i][j].activo = true;

            if(i == 0){
                grid.aliens[i][j].tipo = 0;
            }
            else if(i == 1 || i == 2){
                grid.aliens[i][j].tipo = 1;
            }else{
                grid.aliens[i][j].tipo = 2;
            }
        }
    }
}

void actualizarAlienGrid(AlienGrid &grid){
    bool borde = false;
    for(int i = 0; i < grid.num_filas; i++){
        for(int j = 0; j < grid.num_col; j++){
            if(grid.aliens[i][j].activo == true){
                grid.aliens[i][j].x += grid.vel * grid.dirX; // Movimiento horizaontal
                
                if(grid.aliens[i][j].x < 0 || (grid.aliens[i][j].x + grid.aliens[i][j].ancho) > ANCHO_VENT){
                    borde = true; // True = Si toca el borde de la pantalla para cambiar direccion
                }
            }
        }
    }

    if(borde == true){
        grid.dirX *= -1; // Camabia direccion
        for(int i = 0; i < grid.num_filas; i++){
            for(int j = 0; j < grid.num_col; j++){
                grid.aliens[i][j].y += 15; // Aliens bajan
            }
        }
    }
}

void dibujarAlienGrid(AlienGrid &grid){
    for(int i =0; i < grid.num_filas; i++){
        for(int j = 0; j < grid.num_col; j++){
            if(grid.aliens[i][j].activo == true){
                int tipoAlien = grid.aliens[i][j].tipo;
                if(tipoAlien == 0){
                DrawTexture(
                        grid.textura[tipoAlien],
                        grid.aliens[i][j].x,
                        grid.aliens[i][j].y,
                        RED
                    );
                }else if(tipoAlien == 1){
                    DrawTexture(
                        grid.textura[tipoAlien],
                        grid.aliens[i][j].x,
                        grid.aliens[i][j].y,
                        WHITE
                    );
                }else{
                    DrawTexture(
                        grid.textura[tipoAlien],
                        grid.aliens[i][j].x,
                        grid.aliens[i][j].y,
                        PURPLE
                    );
                }
            }
        }
    }
}

void iniciaLasers(ListaLaser &lista){
    for(int i = 0; i < MAX_LASERS; i++){
        lista.lasers[i].activo = false;
    }
}

void disparoLaser(ListaLaser &lista, float x, float y){
    for(int i = 0; i < MAX_LASERS; i++){
        if(lista.lasers[i].activo == false){
            lista.lasers[i].x = x;
            lista.lasers[i].y = y;
            lista.lasers[i].vel = -7.0f; // Negativo para que suba en la pantalla
            lista.lasers[i].activo = true;
            break; // Para hacer solo un laser por disparo
        }
    }
}

void disparoNave(Nave &Jugador, ListaLaser &lista){
    if(IsKeyPressed(KEY_SPACE) == true){
        float posX = Jugador.x + (Jugador.ancho/2) - 2; // Calcula el centro de la nave para disparar desde ahi
        float posY = Jugador.y;

        disparoLaser(lista, posX, posY); 
    }
}


void actualizarLaser(ListaLaser &lista){
    for(int i = 0; i < MAX_LASERS; i++){
        if(lista.lasers[i].activo == true){
            lista.lasers[i].y += lista.lasers[i].vel;

            if(lista.lasers[i].y < 0){
                lista.lasers[i].activo = false; // Si llega a la altura 0 (Final de la ventana) desaparece
            }
        }
    }
}

void dibujarLasers(const ListaLaser &lista){
    for(int i = 0; i < MAX_LASERS; i++){
        if(lista.lasers[i].activo == true){
            DrawRectangle(
                lista.lasers[i].x,
                lista.lasers[i].y,
                4, 
                16,
                BLUE
            );
        }
    }
}