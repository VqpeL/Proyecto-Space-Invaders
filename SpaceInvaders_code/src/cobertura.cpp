#include "cobertura.h"
#include <stdlib.h>

const float ANCHO_BLOQUE_COBERTURA = 20.0f;
const float ALTO_BLOQUE_COBERTURA = 10.0f;

const float ESPACIO_BLOQUES_COBERTURA = 0.0f;

void InicializarCoberturas(Cobertura coberturas[], int cantidad, int anchoPantalla, int altoPantalla)
{
    float espacioEntreCoberturas = anchoPantalla / (float)(cantidad + 1);

    float yBase = altoPantalla - 200.0f;

    for (int c = 0; c < cantidad; c++)
    {

        float anchoTotalCobertura =
            (COLUMNAS_COBERTURA * ANCHO_BLOQUE_COBERTURA) +
            ((COLUMNAS_COBERTURA - 1) * ESPACIO_BLOQUES_COBERTURA);

        float xBase = espacioEntreCoberturas * (c + 1) - (anchoTotalCobertura / 2.0f);

        for (int fila = 0; fila < FILAS_COBERTURA; fila++)
        {
            for (int col = 0; col < COLUMNAS_COBERTURA; col++)
            {

                coberturas[c].bloques[fila][col].rect = {
                    xBase + col * (ANCHO_BLOQUE_COBERTURA + ESPACIO_BLOQUES_COBERTURA),
                    yBase + fila * (ALTO_BLOQUE_COBERTURA + ESPACIO_BLOQUES_COBERTURA),
                    ANCHO_BLOQUE_COBERTURA,
                    ALTO_BLOQUE_COBERTURA};

                coberturas[c].bloques[fila][col].vida = 1;
                coberturas[c].bloques[fila][col].activo = true;
            }
        }
    }
}

void DibujarCoberturas(Cobertura coberturas[], int cantidad)
{
    for (int c = 0; c < cantidad; c++)
    {
        for (int fila = 0; fila < FILAS_COBERTURA; fila++)
        {
            for (int col = 0; col < COLUMNAS_COBERTURA; col++)
            {

                BloqueCobertura *bloque = &coberturas[c].bloques[fila][col];

                if (bloque->activo)
                {

                    DrawRectangleRec(bloque->rect, ORANGE);
                }
            }
        }
    }
}

void ColisionProyectilesCoberturas(Proyectil **cabezaProyectiles, Cobertura coberturas[], int cantidad)
{
    Proyectil *actual = *cabezaProyectiles;
    Proyectil *anterior = NULL;

    while (actual != NULL)
    {

        Rectangle rectProyectil = {
            actual->posicion.x,
            actual->posicion.y,
            actual->ancho,
            actual->alto};

        bool proyectilEliminado = false;

        for (int c = 0; c < cantidad && !proyectilEliminado; c++)
        {
            for (int fila = 0; fila < FILAS_COBERTURA && !proyectilEliminado; fila++)
            {
                for (int col = 0; col < COLUMNAS_COBERTURA; col++)
                {

                    BloqueCobertura *bloque = &coberturas[c].bloques[fila][col];

                    if (bloque->activo && CheckCollisionRecs(rectProyectil, bloque->rect))
                    {

                        bloque->vida--;

                        if (bloque->vida <= 0)
                        {
                            bloque->activo = false;
                        }

                        Proyectil *aEliminar = actual;

                        if (anterior == NULL)
                        {
                            *cabezaProyectiles = actual->siguiente;
                        }
                        else
                        {
                            anterior->siguiente = actual->siguiente;
                        }

                        actual = actual->siguiente;
                        free(aEliminar);

                        proyectilEliminado = true;
                        break;
                    }
                }
            }
        }

        if (!proyectilEliminado)
        {
            anterior = actual;
            actual = actual->siguiente;
        }
    }
}

void ColisionAlienCobertura(AlienGrid *grid, Cobertura coberturas[], int cantidad)
{
    for (int filaAlien = 0; filaAlien < grid->num_filas; filaAlien++)
    {
        for (int colAlien = 0; colAlien < grid->num_col; colAlien++)
        {

            if (grid->aliens[filaAlien][colAlien].activo)
            {

                Rectangle rectAlien = {
                    grid->aliens[filaAlien][colAlien].x,
                    grid->aliens[filaAlien][colAlien].y,
                    grid->aliens[filaAlien][colAlien].ancho,
                    grid->aliens[filaAlien][colAlien].alto};

                for (int c = 0; c < cantidad; c++)
                {
                    for (int fila = 0; fila < FILAS_COBERTURA; fila++)
                    {
                        for (int col = 0; col < COLUMNAS_COBERTURA; col++)
                        {

                            BloqueCobertura *bloque = &coberturas[c].bloques[fila][col];

                            if (bloque->activo && CheckCollisionRecs(rectAlien, bloque->rect))
                            {

                                grid->aliens[filaAlien][colAlien].activo = false;
                                bloque->activo = false;

                                return;
                            }
                        }
                    }
                }
            }
        }
    }
}