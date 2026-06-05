#include "colisiones.h"
#include <stdlib.h>

void ColisionLaserAlien(Proyectil** cabezaLasers, AlienGrid *grid) {
    Proyectil* actual = *cabezaLasers;
    Proyectil* anterior = NULL;
    while (actual != NULL) {
        Rectangle rectLaser = {
            actual->posicion.x,
            actual->posicion.y,
            actual->ancho,
            actual->alto
        };

        bool balaEliminada = false;
        for (int fila = 0; fila < grid->num_filas; fila++) {
            for (int col = 0; col < grid->num_col; col++) {
                if (grid->aliens[fila][col].activo) {

                    Rectangle rectAlien = {
                        grid->aliens[fila][col].x,
                        grid->aliens[fila][col].y,
                        grid->aliens[fila][col].ancho,
                        grid->aliens[fila][col].alto
                    };

                    if (CheckCollisionRecs(rectLaser, rectAlien)) {
                        grid->aliens[fila][col].activo = false; 
                        Proyectil* aEliminar = actual;
                        if (anterior == NULL) {
                            *cabezaLasers = actual->siguiente;
                        } else {
                            anterior->siguiente = actual->siguiente;
                        }
                        
                        actual = actual->siguiente;
                        free(aEliminar); 
                        
                        balaEliminada = true;
                        break; 
                    }
                }
            }
            if (balaEliminada) break; 
        }

        if (!balaEliminada) {
            anterior = actual;
            actual = actual->siguiente;
        }
    }
}

void ColisionAlienNave(Nave *jugador, AlienGrid *grid) {
    Rectangle rectNave = {
        jugador->posicion.x,
        jugador->posicion.y,
        jugador->ancho,
        jugador->alto
    };

    for (int fila = 0; fila < grid->num_filas; fila++) {
        for (int col = 0; col < grid->num_col; col++) {
            if (grid->aliens[fila][col].activo) {
                
                Rectangle rectAlien = {
                    grid->aliens[fila][col].x,
                    grid->aliens[fila][col].y,
                    grid->aliens[fila][col].ancho,
                    grid->aliens[fila][col].alto
                };

                if (CheckCollisionRecs(rectNave, rectAlien)) {
                    grid->aliens[fila][col].activo = false;
                    return; 
                }
            }
        }
    }
}

void ColisionProyectilEnemigoNave(Proyectil** cabezaLasersAliens, Nave *jugador) {
    Proyectil* actual = *cabezaLasersAliens;
    Proyectil* anterior = NULL;

    Rectangle rectNave = {
        jugador->posicion.x,
        jugador->posicion.y,
        jugador->ancho,
        jugador->alto
    };

    while (actual != NULL) {
        Rectangle rectLaser = {
            actual->posicion.x,
            actual->posicion.y,
            actual->ancho,
            actual->alto
        };

        if (CheckCollisionRecs(rectLaser, rectNave)) {
            jugador->vidas--;
            Proyectil* aEliminar = actual;
            if (anterior == NULL) {
                *cabezaLasersAliens = actual->siguiente;
            } else {
                anterior->siguiente = actual->siguiente;
            }
            
            actual = actual->siguiente;
            free(aEliminar); 
            
            // Si quieres que solo le afecte un impacto por frame, salimos de la función
            return; 
        } else {
            anterior = actual;
            actual = actual->siguiente;
        }
    }
}