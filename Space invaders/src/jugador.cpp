#include "jugador.h"

void InicializarNave(Nave *nave, int anchoPantalla, int altoPantalla) {
    nave->ancho = 50.0f;
    nave->alto = 25.0f;
    nave->velocidad = 6.0f;
    
    // Centrada abajo
    nave->posicion.x = (float)anchoPantalla / 2.0f - (nave->ancho / 2.0f);
    nave->posicion.y = (float)altoPantalla - 50.0f;

    nave->textura = LoadTexture("assets/graficos/nave.png");

}

void ActualizarNave(Nave *nave, int anchoPantalla) {
    // Movimiento con A y D
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        nave->posicion.x -= nave->velocidad;
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))  {
        nave->posicion.x += nave->velocidad;
    }
    



    // Límites de pantalla
    if (nave->posicion.x < 0) {
        nave->posicion.x = 0;
    }
    if (nave->posicion.x > anchoPantalla - nave->ancho) {
        nave->posicion.x = anchoPantalla - nave->ancho;
    }
}

void DibujarNave(Nave *nave) {

    DrawTexture(
        nave->textura,
        nave->posicion.x,
        nave->posicion.y, 
        ORANGE
    );
}

void LiberarNave(Nave *nave) {
    UnloadTexture(nave->textura); 
}