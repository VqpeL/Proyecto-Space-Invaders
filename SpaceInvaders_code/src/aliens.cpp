#include "aliens.h"
#include <stdlib.h>

const float ancho_alien = 30.0f;
const float alto_alien = 20.0f;

void IniciaAlienGrid(AlienGrid *grid, int filas, int col, int vel)
{

    grid->num_col = col;
    grid->num_filas = filas;
    grid->dirX = 1.0f;
    grid->velNivel = vel;

    grid->textura[0] = LoadTexture("assets/graficos/alien_1.png");
    grid->textura[1] = LoadTexture("assets/graficos/alien_2.png");
    grid->textura[2] = LoadTexture("assets/graficos/alien_3.png");

    grid->aliens = (Alien **)malloc(filas * sizeof(Alien *));

    for (int i = 0; i < filas; i++)
    {
        grid->aliens[i] = (Alien *)malloc(col * sizeof(Alien));
        for (int j = 0; j < col; j++)
        {

            grid->aliens[i][j].ancho = ancho_alien;
            grid->aliens[i][j].alto = alto_alien;

            grid->aliens[i][j].x = (float)(j * (ancho_alien + 30) + 50);
            grid->aliens[i][j].y = (float)(i * (alto_alien + 30) + 50);
            grid->aliens[i][j].activo = true;

            if (i == 0)
            {
                grid->aliens[i][j].tipo = 0;
            }
            else if (i == 1 || i == 2)
            {
                grid->aliens[i][j].tipo = 1;
            }
            else
            {
                grid->aliens[i][j].tipo = 2;
            }
        }
    }
}

void ActualizarAlienGrid(AlienGrid *grid, int ancho_pantalla)
{
    bool borde = false;
    int aliens_muertos = 0;
    int total_aliens = grid->num_col * grid->num_filas;

    for (int i = 0; i < grid->num_filas; i++)
    {
        for (int j = 0; j < grid->num_col; j++)
        {
            if (!grid->aliens[i][j].activo)
            {
                aliens_muertos += 1;
            }
        }
    }

    grid->vel = 1.0f + (grid->velNivel - 1.0f) * ((float)aliens_muertos / total_aliens);

    for (int i = 0; i < grid->num_filas; i++)
    {
        for (int j = 0; j < grid->num_col; j++)
        {
            if (grid->aliens[i][j].activo)
            {
                grid->aliens[i][j].x += grid->dirX * 2.0f * grid->vel;

                if (grid->aliens[i][j].x < 20 || (grid->aliens[i][j].x + grid->aliens[i][j].ancho + 20) > ancho_pantalla)
                {
                    borde = true;
                }
            }
        }
    }

    if (borde)
    {
        grid->dirX *= -1;
        for (int i = 0; i < grid->num_filas; i++)
        {
            for (int j = 0; j < grid->num_col; j++)
            {
                grid->aliens[i][j].y += 9.0f;
            }
        }
    }
}

void DibujarAlienGrid(AlienGrid *grid)
{
    for (int i = 0; i < grid->num_filas; i++)
    {
        for (int j = 0; j < grid->num_col; j++)
        {
            if (grid->aliens[i][j].activo)
            {
                int tipoAlien = grid->aliens[i][j].tipo;

                Color colorTinte = WHITE;
                if (tipoAlien == 0)
                    colorTinte = RED;
                else if (tipoAlien == 2)
                    colorTinte = PURPLE;

                DrawTexture(
                    grid->textura[tipoAlien],
                    (int)grid->aliens[i][j].x,
                    (int)grid->aliens[i][j].y,
                    colorTinte);
            }
        }
    }
}

Vector2 AlienAleatorio(AlienGrid *grid, int filas, int col)
{

    if (grid->aliens == nullptr || filas <= 0 || col <= 0)
        return (Vector2){-1.0f, -1.0f};

    int pos_fila = 0;
    int pos_col = 0;
    Vector2 pos_alien = {-1.0f, -1.0f};

    for (int intentos = 0; intentos < 40; intentos++)
    {
        pos_fila = GetRandomValue(0, filas - 1);
        pos_col = GetRandomValue(0, col - 1);

        if (grid->aliens[pos_fila][pos_col].activo)
        {
            pos_alien.x = grid->aliens[pos_fila][pos_col].x + 20.0f;
            pos_alien.y = grid->aliens[pos_fila][pos_col].y + 19.0f;
            return pos_alien;
        }
    }

    for (int f = 0; f < filas; f++)
    {
        for (int c = 0; c < col; c++)
        {
            if (grid->aliens[f][c].activo)
            {
                pos_alien.x = grid->aliens[f][c].x + 20.0f;
                pos_alien.y = grid->aliens[f][c].y + 19.0f;
                return pos_alien;
            }
        }
    }

    return pos_alien;
}

void LiberarAlienGrid(AlienGrid *grid)
{
    if (grid->aliens != NULL)
    {
        for (int i = 0; i < grid->num_filas; i++)
        {
            free(grid->aliens[i]);
        }
        free(grid->aliens);
        grid->aliens = NULL;
    }

    for (int i = 0; i < 3; i++)
    {
        UnloadTexture(grid->textura[i]);
    }
}

bool GridVacia(AlienGrid *grid)
{

    if (grid->aliens == NULL)
        return true;

    for (int i = 0; i < grid->num_filas; i++)
    {
        for (int j = 0; j < grid->num_col; j++)
        {
            if (grid->aliens[i][j].activo)
            {
                return false;
            }
        }
    }
    return true;
}

bool AlienSalioPantalla(AlienGrid *grid, int altoPantalla)
{
    if (grid->aliens == NULL)
    {
        return false;
    }

    for (int fila = 0; fila < grid->num_filas; fila++)
    {
        for (int col = 0; col < grid->num_col; col++)
        {

            if (grid->aliens[fila][col].activo)
            {

                float parteBajaAlien = grid->aliens[fila][col].y + grid->aliens[fila][col].alto;

                if (parteBajaAlien >= altoPantalla)
                {
                    return true;
                }
            }
        }
    }

    return false;
}