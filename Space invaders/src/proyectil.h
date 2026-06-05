#ifndef PROYECTIL_H
#define PROYECTIL_H

#include "raylib.h"

typedef struct Proyectil {
    Vector2 posicion;
    float velocidadY; // Ahora guardamos la velocidad con dirección
    float ancho;
    float alto;
    struct Proyectil* siguiente;
} Proyectil;

// Agregamos 'velocidad' y 'color' a las funciones
void Disparar(Proyectil** cabeza, Vector2 pos, float velY);
void ActualizarProyectiles(Proyectil** cabeza, int altoPantalla);
void DibujarProyectiles(Proyectil* cabeza, Color color);
void LiberarProyectiles(Proyectil** cabeza);

#endif