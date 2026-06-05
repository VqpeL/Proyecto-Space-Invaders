#include "Background.h"
#include <stdlib.h> // OBLIGATORIO para usar malloc y free

// 1. Reservamos memoria dinámica y llenamos las estrellas
Estrella *InicializarBackground(int cantidad_estrellas, int ancho, int alto)
{
    // REQUISITO: Asignación dinámica de memoria
    Estrella *estrellas = (Estrella *)malloc(cantidad_estrellas * sizeof(Estrella));

    // Buenas prácticas de C: Verificar si la computadora nos dio la memoria
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

    return estrellas; // Devolvemos el bloque de memoria dinámico listo
}

// 2. Movemos las estrellas (el acceso con estrellas[i] funciona exactamente igual)
void ActualizarBackground(Estrella *estrellas, int cantidad_estrellas, int ancho, int alto)
{
    // Si por alguna razón el puntero está vacío, no hacemos nada para evitar un crash
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

// 3. Dibujamos las estrellas
void DibujarBackground(Estrella *estrellas, int cantidad_estrellas)
{
    if (estrellas == NULL)
        return;

    for (int i = 0; i < cantidad_estrellas; i++)
    {
        DrawCircleV(estrellas[i].posicion, estrellas[i].radio, estrellas[i].color);
    }
}

// 4. Liberamos la memoria de la pila dinámica (Heap)
void LiberarBackground(Estrella *estrellas)
{
    if (estrellas != NULL)
    {
        free(estrellas); // Devuelve la memoria al sistema operativo
    }
}