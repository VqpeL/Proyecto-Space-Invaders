#include "proyectil.h"
#include <stdlib.h>

void Disparar(Proyectil **cabeza, Vector2 pos, float velY)
{
    Proyectil *nuevo = (Proyectil *)malloc(sizeof(Proyectil));
    if (nuevo == NULL)
        return;

    nuevo->posicion = pos;
    nuevo->velocidadY = velY;
    nuevo->ancho = 4.0f;
    nuevo->alto = 12.0f;
    nuevo->siguiente = *cabeza;
    *cabeza = nuevo;
}

void ActualizarProyectiles(Proyectil **cabeza, int altoPantalla)
{
    Proyectil *actual = *cabeza;
    Proyectil *anterior = NULL;

    while (actual != NULL)
    {
        actual->posicion.y += actual->velocidadY;

        if (actual->posicion.y < 0 || actual->posicion.y > altoPantalla)
        {
            Proyectil *aEliminar = actual;
            if (anterior == NULL)
                *cabeza = actual->siguiente;
            else
                anterior->siguiente = actual->siguiente;

            actual = actual->siguiente;
            free(aEliminar);
        }
        else
        {
            anterior = actual;
            actual = actual->siguiente;
        }
    }
}

void DibujarProyectiles(Proyectil *cabeza, Color color)
{
    Proyectil *actual = cabeza;
    while (actual != NULL)
    {
        DrawRectangleV(actual->posicion, (Vector2){actual->ancho, actual->alto}, color);
        actual = actual->siguiente;
    }
}

void LiberarProyectiles(Proyectil **cabeza)
{
    Proyectil *actual = *cabeza;
    while (actual != NULL)
    {
        Proyectil *siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
    *cabeza = NULL;
}