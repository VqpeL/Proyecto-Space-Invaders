#ifndef ARCHIVOS_H
#define ARCHIVOS_H

#include <stdio.h>
#include <stdbool.h>

typedef struct
{
    int nivel;
    int filas;
    int columnas;
    float vel_max;
    int frecuencia_disparo;
    int puntos_base;
} ConfigNivel;

typedef struct
{
    char nombre[4];
    int puntos;
} HighScore;

bool CargarConfigNivel(int nivelObjetivo, ConfigNivel *config);
void GuardarHighScore(const char *nombreJugador, int puntajeFinal);
bool EsPuntajeAlto(int puntuacionJugador);

#endif