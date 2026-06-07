#include "aliens.h"
#include <stdlib.h> // Necesario para malloc y free

const float ancho_alien = 30.0f;
const float alto_alien = 20.0f;
const float vel_max = 10.0f;


void IniciaAlienGrid(AlienGrid *grid, int filas, int col) {

    grid->num_col = col;
    grid->num_filas = filas;
    grid->dirX = 1.0f; 

    // Carga de texturas en el arreglo de la estructura
    grid->textura[0] = LoadTexture("assets/graficos/alien_1.png");
    grid->textura[1] = LoadTexture("assets/graficos/alien_2.png");
    grid->textura[2] = LoadTexture("assets/graficos/alien_3.png");

    // Asignación de memoria dinámica (Matriz de punteros)
    grid->aliens = (Alien**)malloc(filas * sizeof(Alien*));

    for (int i = 0; i < filas; i++) {
        grid->aliens[i] = (Alien*)malloc(col * sizeof(Alien));
        for (int j = 0; j < col; j++) {
            // Hitbox de cada alien
            grid->aliens[i][j].ancho = ancho_alien;
            grid->aliens[i][j].alto = alto_alien;

            // Posicionamiento inicial en rejilla y el espacio entre cada alien
            grid->aliens[i][j].x = (float)(j * (ancho_alien + 30) + 50);
            grid->aliens[i][j].y = (float)(i * (alto_alien + 30) + 50);
            grid->aliens[i][j].activo = true;

            // Asignación de tipos por filas
            if (i == 0) {
                grid->aliens[i][j].tipo = 0;
            } else if (i == 1 || i == 2) {
                grid->aliens[i][j].tipo = 1;
            } else {
                grid->aliens[i][j].tipo = 2;
            }
        }
    }
}

void ActualizarAlienGrid(AlienGrid *grid,int ancho_pantalla) {
    bool borde = false;
    int aliens_muertos = 0;
    int total_aliens = grid->num_col * grid->num_filas;


    for (int i = 0; i < grid->num_filas; i++) {
        for (int j = 0; j < grid->num_col; j++) {
            if (!grid->aliens[i][j].activo) {
                aliens_muertos += 1;
            }
        }
    }

    grid->vel = 0.7f + (vel_max - 0.7f) * ((float)aliens_muertos / total_aliens);

    // 1. Mover y verificar colisión con paredes laterales
    for (int i = 0; i < grid->num_filas; i++) {
        for (int j = 0; j < grid->num_col; j++) {
            if (grid->aliens[i][j].activo) {
                grid->aliens[i][j].x += grid->vel * grid->dirX;
                
                if (grid->aliens[i][j].x < 20 || (grid->aliens[i][j].x + grid->aliens[i][j].ancho + 20) > ancho_pantalla) {
                    borde = true; 
                }
            }
        }
    }

    // 2. Si un alien tocó el borde, todos cambian de rumbo y descienden
    if (borde) {
        grid->dirX *= -1; 
        for (int i = 0; i < grid->num_filas; i++) {
            for (int j = 0; j < grid->num_col; j++) {
                grid->aliens[i][j].y += 9.0f; 
            }
        }
    }
}

void DibujarAlienGrid(AlienGrid *grid) {
    for (int i = 0; i < grid->num_filas; i++) {
        for (int j = 0; j < grid->num_col; j++) {
            if (grid->aliens[i][j].activo) {
                int tipoAlien = grid->aliens[i][j].tipo;
                
                // Optimizamos el código quitando ifs repetitivos de DrawTexture
                Color colorTinte = WHITE;
                if (tipoAlien == 0) colorTinte = RED;
                else if (tipoAlien == 2) colorTinte = PURPLE;

                DrawTexture(
                    grid->textura[tipoAlien],
                    (int)grid->aliens[i][j].x,
                    (int)grid->aliens[i][j].y,
                    colorTinte
                );
            }
        }
    }
}


Vector2 AlienAleatorio(AlienGrid *grid, int filas, int col) {
    int pos_fila = 0;
    int pos_col = 0;
    bool alien_encontrado = false;
    Vector2 pos_alien = { 0.0f, 0.0f };

    // Bucle de seguridad: busca un alien al azar hasta encontrar uno que esté ACTIVO
    while (!alien_encontrado) {
        // CORRECCIÓN CRÍTICA: Restamos 1 para no salirnos de los límites del malloc
        pos_fila = GetRandomValue(0, filas - 1);
        pos_col = GetRandomValue(0, col - 1);

        // Si el alien en esa posición está vivo, lo tomamos
        if (grid->aliens[pos_fila][pos_col].activo) {
            pos_alien.x = grid->aliens[pos_fila][pos_col].x + 20.0f;
            pos_alien.y = grid->aliens[pos_fila][pos_col].y + 19.0f;
            alien_encontrado = true; // Rompe el ciclo while
        }
    }

    return pos_alien;
}

// ¡FUNCIÓN ESENCIAL PARA EL PROYECTO!
void LiberarAlienGrid(AlienGrid *grid) {
    // 1. Liberar la memoria dinámica de la matriz (Heap)
    if (grid->aliens != NULL) {
        for (int i = 0; i < grid->num_filas; i++) {
            free(grid->aliens[i]); // Libera cada columna asignada
        }
        free(grid->aliens); // Libera el arreglo de filas
        grid->aliens = NULL;
    }

    // 2. Descargar las texturas de la tarjeta gráfica (VRAM)
    for (int i = 0; i < 3; i++) {
        UnloadTexture(grid->textura[i]);
    }
}

bool GridVacia(AlienGrid *grid) {
    // Si la memoria no está inicializada correctamente, evitamos procesar
    if (grid->aliens == NULL) return true;

    for (int i = 0; i < grid->num_filas; i++) {
        for (int j = 0; j < grid->num_col; j++) {
            if (grid->aliens[i][j].activo) {
                return false; // Se encontró un alien vivo, no hay victoria todavía
            }
        }
    }
    return true; // Todos los aliens están muertos
}

bool AlienSalioPantalla(AlienGrid *grid, int altoPantalla) {
    if (grid->aliens == NULL) {
        return false;
    }

    for (int fila = 0; fila < grid->num_filas; fila++) {
        for (int col = 0; col < grid->num_col; col++) {

            if (grid->aliens[fila][col].activo) {

                float parteBajaAlien = grid->aliens[fila][col].y + grid->aliens[fila][col].alto;

                // CAMBIO:
                // El Game Over ocurre solo cuando el alien ya salió por abajo
                // de la pantalla, no cuando toca la nave.
                if (parteBajaAlien >= altoPantalla) {
                    return true;
                }
            }
        }
    }

    return false;
}