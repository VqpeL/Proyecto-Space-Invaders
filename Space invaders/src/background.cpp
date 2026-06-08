#include "Background.h"
#include <stdlib.h>

Estrella *InicializarBackground(int cantidad_estrellas, int ancho, int alto)
{

    Estrella *estrellas = (Estrella *)malloc(cantidad_estrellas * sizeof(Estrella));

    if (estrellas == NULL)
    {
        return NULL;
    }

    for (int i = 0; i < cantidad_estrellas; i++)
    {
        estrellas[i].posicion.x = (float)GetRandomValue(0, ancho);
        estrellas[i].posicion.y = (float)GetRandomValue(0, alto);

        estrellas[i].radio = (float)GetRandomValue(1, 3);
        estrellas[i].velocidad = estrellas[i].radio * 0.5f;

        if (estrellas[i].radio == 1.0f)
        {
            estrellas[i].color = GRAY;
        }
        else if (estrellas[i].radio == 2.0f)
        {
            estrellas[i].color = LIGHTGRAY;
        }
        else
        {
            estrellas[i].color = WHITE;
        }
    }

    return estrellas;
}

void ActualizarBackground(Estrella *estrellas, int cantidad_estrellas, int ancho, int alto)
{

    if (estrellas == NULL)
        return;

    for (int i = 0; i < cantidad_estrellas; i++)
    {
        estrellas[i].posicion.y += estrellas[i].velocidad;

        if (estrellas[i].posicion.y > alto)
        {
            estrellas[i].posicion.y = 0;
            estrellas[i].posicion.x = (float)GetRandomValue(0, ancho);
        }
    }
}

void DibujarBackground(Estrella *estrellas, int cantidad_estrellas)
{
    if (estrellas == NULL)
        return;

    for (int i = 0; i < cantidad_estrellas; i++)
    {
        DrawCircleV(estrellas[i].posicion, estrellas[i].radio, estrellas[i].color);
    }
}

void LiberarBackground(Estrella *estrellas)
{
    if (estrellas != NULL)
    {
        free(estrellas);
    }
}