#ifndef JUGADOR_H
#define JUGADOR_H

#include "raylib.h"

// En C, definimos el struct de esta manera
typedef struct {
    Vector2 posicion;
    float velocidad;
    float ancho;
    float alto;
    Texture2D textura;
} Nave;

// Funciones globales que reciben la nave como parámetro
void InicializarNave(Nave *nave, int anchoPantalla, int altoPantalla);
void ActualizarNave(Nave *nave, int anchoPantalla);
void DibujarNave(Nave *nave);
void LiberarNave(Nave *nave);

#endif