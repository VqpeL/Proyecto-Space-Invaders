#include "raylib.h"
#include "background.h"
#include "jugador.h"
#include "aliens.h"
#include "proyectil.h"
#include "colisiones.h"
#include <vector>

enum EstadoJuego
{
    PANTALLA_INICIO,
    PANTALLA_JUEGO,
    PANTALLA_FINAL,
    PANTALLA_VICTORIA
};

int main()
{
    // Colores

    Color azulCeleste = {135, 206, 250, 255};

    bool reset = true;
    const int cantidad_estrellas = 80;
    const int anchoPantalla = 800;
    const int altoPantalla = 800;
    InitWindow(anchoPantalla, altoPantalla, "Space Invaders - Estructura de Pantallas");
    SetTargetFPS(60);

    // Crear structs o arreglos ----------------

    AlienGrid miGrid;

    Proyectil *balasJugador = nullptr;
    Proyectil *balasAliens = nullptr;

    Nave jugador;

    EstadoJuego estadoActual = PANTALLA_INICIO;

    // Crear structs o arreglos ----------------

    // Inicializar cosas globales ----------------

    Estrella *misEstrellas = InicializarBackground(cantidad_estrellas, anchoPantalla, altoPantalla);
    IniciaAlienGrid(&miGrid, 5, 10);
    InicializarNave(&jugador, anchoPantalla, altoPantalla);

    // Inicializar cosas globales ----------------

    // Ciclo principal -------------
    while (!WindowShouldClose())
    {

        // Para actualizar el estado de las cosas por cada fotograma ---------------

        ActualizarBackground(misEstrellas, cantidad_estrellas, anchoPantalla, altoPantalla);

        // Para actualizar el estado de las cosas por cada fotograma ---------------

        // Para cambiar la pantalla en la que esta el usuario ---------------
        switch (estadoActual)
        {
        case PANTALLA_INICIO:

            if (IsKeyPressed(KEY_ENTER))
            {
                estadoActual = PANTALLA_JUEGO;
            }
            break;

        case PANTALLA_JUEGO:

            if (reset)
            {

                IniciaAlienGrid(&miGrid, 5, 10);
                InicializarNave(&jugador, anchoPantalla, altoPantalla);
                reset = false;
            }

            ActualizarNave(&jugador, anchoPantalla);
            ActualizarAlienGrid(&miGrid, anchoPantalla);
            ActualizarProyectiles(&balasJugador, altoPantalla);
            ActualizarProyectiles(&balasAliens, altoPantalla);

            ColisionLaserAlien(&balasJugador, &miGrid);
            ColisionAlienNave(&jugador, &miGrid);
            ColisionProyectilEnemigoNave(&balasAliens, &jugador);

            if (jugador.vidas <= 0)
            {
                estadoActual = PANTALLA_FINAL;
            }

            if (GridVacia(&miGrid))
            {
                estadoActual = PANTALLA_VICTORIA;
            }

            if (IsKeyPressed(KEY_SPACE))
            {
                Vector2 pos = {jugador.posicion.x + 20, jugador.posicion.y};
                Disparar(&balasJugador, pos, -7.0f); // Velocidad negativa = SUBE
            }

            if (GetRandomValue(0, 100) < 4)
            { // 2% de probabilidad por frame
                // Aquí elegiríamos un alien al azar de la matriz, por ahora un punto fijo:
                int col_aliens = miGrid.num_col;
                int filas_aliens = miGrid.num_filas;
                Vector2 posAlien = AlienAleatorio(&miGrid, filas_aliens, col_aliens);
                Disparar(&balasAliens, posAlien, 5.0f); // Velocidad positiva = BAJA
            }

            if (IsKeyPressed(KEY_G))
            {
                estadoActual = PANTALLA_FINAL;
            }
            break;

        case PANTALLA_VICTORIA:
            if (IsKeyPressed(KEY_ENTER))
            {
                if (!reset)
                {
                    LiberarAlienGrid(&miGrid);
                    LiberarNave(&jugador);
                    LiberarProyectiles(&balasJugador);
                    LiberarProyectiles(&balasAliens);
                    reset = true;
                }
                estadoActual = PANTALLA_INICIO;
            }
            break;

        case PANTALLA_FINAL:

            if (!reset)
            {
                LiberarProyectiles(&balasJugador);
                LiberarProyectiles(&balasAliens);
                LiberarAlienGrid(&miGrid);
                LiberarNave(&jugador);
                reset = true;
            }

            if (IsKeyPressed(KEY_ENTER))
            {
                estadoActual = PANTALLA_INICIO;
            }
            break;
        }

        // Para cambiar la pantalla en la que esta el usuario ---------------

        // Empieza a dibujar los graficos para cada fotograma ----------------------

        BeginDrawing();

        // Fondo animado es estrellas

        ClearBackground(BLACK);
        DibujarBackground(misEstrellas, cantidad_estrellas);

        DrawFPS(10, altoPantalla - 30);

        switch (estadoActual)
        {
        case PANTALLA_INICIO:

            DrawText("SPACE INVADERS", anchoPantalla / 2 - MeasureText("SPACE INVADERS", 50) / 2, 200, 50, GREEN);
            DrawText("Presiona ENTER para comenzar", anchoPantalla / 2 - MeasureText("Presiona ENTER para comenzar", 20) / 2, 350, 20, RAYWHITE);
            break;

        case PANTALLA_JUEGO:
            if (!reset)
            {
                DrawText("¡AQUÍ ESTÁS JUGANDO!", 20, 20, 20, RAYWHITE);

                DrawText(TextFormat("VIDAS: %d", jugador.vidas), anchoPantalla - 120, 20, 20, RED);
                DibujarAlienGrid(&miGrid);

                DibujarNave(&jugador);

                DibujarProyectiles(balasJugador, YELLOW);
                DibujarProyectiles(balasAliens, azulCeleste);
            }

            break;

        case PANTALLA_VICTORIA:
            DrawText("¡VICTORIA!", anchoPantalla / 2 - MeasureText("¡VICTORIA!", 50) / 2, 200, 50, GREEN);
            DrawText("Has salvado la galaxia", anchoPantalla / 2 - MeasureText("Has salvado la galaxia", 20) / 2, 300, 20, RAYWHITE);
            DrawText("Presiona ENTER para volver al menú", anchoPantalla / 2 - MeasureText("Presiona ENTER para volver al menú", 20) / 2, 400, 20, GRAY);
            break;

        case PANTALLA_FINAL:

            DrawText("GAME OVER", anchoPantalla / 2 - MeasureText("GAME OVER", 50) / 2, 200, 50, RED);
            DrawText("Presiona ENTER para volver al menú", anchoPantalla / 2 - MeasureText("Presiona ENTER para volver al menú", 20) / 2, 350, 20, RAYWHITE);

            break;
        }

        EndDrawing();

        // Empieza a dibujar los graficos para cada fotograma ----------------------
    }
    // Ciclo principal -------------

    // Para cerrar el programa ------------

    LiberarProyectiles(&balasJugador);
    LiberarProyectiles(&balasAliens);
    LiberarAlienGrid(&miGrid);
    LiberarNave(&jugador);
    LiberarBackground(misEstrellas);
    CloseWindow();
    return 0;
    // Para cerrar el programa ------------
}
