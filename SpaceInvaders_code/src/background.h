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

Estrella *InicializarBackground(int cantidad_estrellas, int ancho, int alto);

void ActualizarBackground(Estrella *estrellas, int cantidad_estrellas, int ancho, int alto);
void DibujarBackground(Estrella *estrellas, int cantidad_estrellas);

void LiberarBackground(Estrella *estrellas);

#endif