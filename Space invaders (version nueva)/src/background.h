#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "raylib.h"

typedef struct
{
    Vector2 posicion;
    float velocidad;
    float radio;
    Color color;
} Estrella;

// Ahora InicializarBackground CREA y devuelve el puntero con la memoria dinámica
Estrella *InicializarBackground(int cantidad_estrellas, int ancho, int alto);

// Estas funciones ahora reciben el puntero de la memoria dinámica
void ActualizarBackground(Estrella *estrellas, int cantidad_estrellas, int ancho, int alto);
void DibujarBackground(Estrella *estrellas, int cantidad_estrellas);

// Función crucial para evitar fugas de memoria (Memory Leaks)
void LiberarBackground(Estrella *estrellas);

#endif