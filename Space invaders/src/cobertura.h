#ifndef COBERTURA_H
#define COBERTURA_H

#include "raylib.h"
#include "proyectil.h"
#include <stdbool.h>
#include "aliens.h"

#define MAX_COBERTURAS 4
#define FILAS_COBERTURA 4
#define COLUMNAS_COBERTURA 4

typedef struct
{
    Rectangle rect;
    int vida;
    bool activo;
} BloqueCobertura;

typedef struct
{
    BloqueCobertura bloques[FILAS_COBERTURA][COLUMNAS_COBERTURA];
} Cobertura;

void InicializarCoberturas(Cobertura coberturas[], int cantidad, int anchoPantalla, int altoPantalla);
void DibujarCoberturas(Cobertura coberturas[], int cantidad);
void ColisionProyectilesCoberturas(Proyectil **cabezaProyectiles, Cobertura coberturas[], int cantidad);
void ColisionAlienCobertura(AlienGrid *grid, Cobertura coberturas[], int cantidad);

#endif