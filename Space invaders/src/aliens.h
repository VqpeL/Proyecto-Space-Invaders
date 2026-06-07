#ifndef ALIENS_H
#define ALIENS_H

#include "raylib.h"
#include <stdbool.h>

typedef struct {
    float x;
    float y;
    int puntos;
    int tipo;
    float vel;
    float ancho;
    float alto;
    bool activo;
} Alien;

typedef struct {
    Alien** aliens;
    int num_col;
    int num_filas;
    float dirX;
    float vel;
    float velNivel;
    Texture2D textura[3];
} AlienGrid;

// Funciones corregidas con punteros de C puro (*)
void IniciaAlienGrid(AlienGrid *grid, int filas, int col, int nivel);
void ActualizarAlienGrid(AlienGrid *grid, int ancho_pantalla);
void DibujarAlienGrid(AlienGrid *grid);
Vector2 AlienAleatorio(AlienGrid *grid, int filas, int col);
void LiberarAlienGrid(AlienGrid *grid); // <-- ¡La pieza que les faltaba!
bool GridVacia(AlienGrid *grid);
bool AlienSalioPantalla(AlienGrid *grid, int altoPantalla);
#endif