#include "raylib.h"
#include "background.h"
#include "jugador.h"
#include "aliens.h"
#include "proyectil.h"
#include "colisiones.h"
#include "cobertura.h"
#include <vector>

enum EstadoJuego
{
    PANTALLA_INICIO,
    PANTALLA_JUEGO,
    PANTALLA_VICTORIA,
    PANTALLA_FINAL
};

int main()
{
    Color azulCeleste = {135, 206, 250, 255};
    bool reset = true;
    const int cantidad_estrellas = 80;
    const int anchoPantalla = 800;
    const int altoPantalla = 800;

    InitWindow(anchoPantalla, altoPantalla, "Space Invaders - Estructura de Pantallas");
    SetTargetFPS(60);

    InitAudioDevice();
    Music musicaMenu = LoadMusicStream("assets/sonidos/musicaMenu.mp3");
    Music musicaJuego = LoadMusicStream("assets/sonidos/musicaJuego.mp3");
    Sound sonidoMuerte = LoadSound("assets/sonidos/explosion.wav");
    Sound sonidoLaser = LoadSound("assets/sonidos/laser.wav");
    Sound sonidoLaserAlien = LoadSound("assets/sonidos/laserAlien.wav");
    Sound golpeNave = LoadSound("assets/sonidos/golpeNave.wav");

    SetMusicVolume(musicaMenu, 0.03f);
    SetMusicVolume(musicaJuego, 0.01f);
    SetSoundVolume(sonidoMuerte, 0.01f);
    SetSoundVolume(sonidoLaser, 0.1f);
    SetSoundVolume(sonidoLaserAlien, 0.015f);
    SetSoundVolume(golpeNave, 0.08f);

    PlayMusicStream(musicaMenu);

    AlienGrid miGrid;
    miGrid.aliens = nullptr; 

    Proyectil *balasJugador = nullptr;
    Proyectil *balasAliens = nullptr;

    Nave jugador;
    jugador.textura.id = 0; 
    Cobertura coberturas[MAX_COBERTURAS];
    int nivelActual = 1;
    EstadoJuego estadoActual = PANTALLA_INICIO;

    Estrella *misEstrellas = InicializarBackground(cantidad_estrellas, anchoPantalla, altoPantalla);

    while (!WindowShouldClose())
    {
        ActualizarBackground(misEstrellas, cantidad_estrellas, anchoPantalla, altoPantalla);

        // Control del stream de audio según la pantalla
        if (estadoActual == PANTALLA_INICIO || estadoActual == PANTALLA_FINAL || estadoActual == PANTALLA_VICTORIA)
        {
            UpdateMusicStream(musicaMenu);
        }
        else if (estadoActual == PANTALLA_JUEGO)
        {
            UpdateMusicStream(musicaJuego);
        }

        // Lógica y actualización del estado actual
        switch (estadoActual)
        {
        case PANTALLA_INICIO:
            if (IsKeyPressed(KEY_ENTER))
            {
                StopMusicStream(musicaMenu);
                PlayMusicStream(musicaJuego);
                estadoActual = PANTALLA_JUEGO;
                reset = true; 
            }
            break;

        case PANTALLA_JUEGO:
        {
            if (reset)
            {
                IniciaAlienGrid(&miGrid, 5, 10, nivelActual);
                InicializarNave(&jugador, anchoPantalla, altoPantalla);
                jugador.vidas = 3;
                InicializarCoberturas(coberturas, MAX_COBERTURAS, anchoPantalla, altoPantalla);
                reset = false;
            }

            ActualizarNave(&jugador, anchoPantalla);
            ActualizarAlienGrid(&miGrid, anchoPantalla);
            ActualizarProyectiles(&balasJugador, altoPantalla);
            ActualizarProyectiles(&balasAliens, altoPantalla);

            int vidasAntes = jugador.vidas;

            // Bloque de colisiones
            ColisionProyectilesCoberturas(&balasJugador, coberturas, MAX_COBERTURAS);
            ColisionProyectilesCoberturas(&balasAliens, coberturas, MAX_COBERTURAS);
            ColisionAlienCobertura(&miGrid, coberturas, MAX_COBERTURAS);
            ColisionLaserAlien(&balasJugador, &miGrid, sonidoMuerte); 
            ColisionAlienNave(&jugador, &miGrid);
            ColisionProyectilEnemigoNave(&balasAliens, &jugador);

            // Sonido si el jugador recibe daño
            if (jugador.vidas < vidasAntes)
            {
                PlaySound(golpeNave);
            }

            // Validación de derrota
            if (jugador.vidas <= 0 || AlienSalioPantalla(&miGrid, altoPantalla))
            {
                StopMusicStream(musicaJuego);
                PlayMusicStream(musicaMenu);
                estadoActual = PANTALLA_FINAL;
                reset = false; 
            }

            // Pasar de nivel al limpiar la pantalla
            if (GridVacia(&miGrid))
            {
                nivelActual++;
                LiberarAlienGrid(&miGrid);
                LiberarProyectiles(&balasJugador);
                LiberarProyectiles(&balasAliens);
                
                IniciaAlienGrid(&miGrid, 5, 10, nivelActual);
                InicializarCoberturas(coberturas, MAX_COBERTURAS, anchoPantalla, altoPantalla);
                jugador.vidas = 3;
                jugador.posicion.x = (float)anchoPantalla / 2.0f - (jugador.ancho / 2.0f);
            }

            // Control de disparo e inserción de audio del jugador
            if (IsKeyPressed(KEY_SPACE))
            {
                Vector2 posDisparo = { jugador.posicion.x + (jugador.ancho / 2.0f), jugador.posicion.y };
                Disparar(&balasJugador, posDisparo, -7.0f); 
                PlaySound(sonidoLaser);
            }

            // Disparo aleatorio de los aliens
            if (GetRandomValue(0, 100) < 4)
            { 
                int col_aliens = miGrid.num_col;
                int filas_aliens = miGrid.num_filas;
                Vector2 posAlien = AlienAleatorio(&miGrid, filas_aliens, col_aliens);
                Disparar(&balasAliens, posAlien, 5.0f); 
                PlaySound(sonidoLaserAlien);
            }

            if (IsKeyPressed(KEY_G)) 
            {
                StopMusicStream(musicaJuego);
                PlayMusicStream(musicaMenu);
                estadoActual = PANTALLA_FINAL;
                reset = false;
            }
            break;
        }

        case PANTALLA_VICTORIA:
        {
            if (!reset)
            {
                LiberarAlienGrid(&miGrid);
                LiberarNave(&jugador);
                LiberarProyectiles(&balasJugador);
                LiberarProyectiles(&balasAliens);
                reset = true;
            }

            if (IsKeyPressed(KEY_ENTER))
            {
                estadoActual = PANTALLA_INICIO;
            }
            break;
        }

        case PANTALLA_FINAL:
        {
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
                nivelActual = 1;
                estadoActual = PANTALLA_INICIO;
            }
            break;
        }
        }

        // Renderizado de gráficos
        BeginDrawing();
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
                DrawText(TextFormat("VIDAS: %d", jugador.vidas), anchoPantalla - 140, 20, 20, RED);
                DrawText(TextFormat("NIVEL: %d", nivelActual), anchoPantalla - 140, 50, 20, GREEN);

                DibujarAlienGrid(&miGrid);
                DibujarCoberturas(coberturas, MAX_COBERTURAS);
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
    }

    // Liberación de recursos al cerrar la ventana
    UnloadMusicStream(musicaMenu);
    UnloadMusicStream(musicaJuego);
    UnloadSound(sonidoMuerte);
    UnloadSound(sonidoLaser);
    UnloadSound(sonidoLaserAlien);
    UnloadSound(golpeNave);
    CloseAudioDevice();

    if (!reset) {
        LiberarProyectiles(&balasJugador);
        LiberarProyectiles(&balasAliens);
        LiberarAlienGrid(&miGrid);
        LiberarNave(&jugador);
    }
    LiberarBackground(misEstrellas);
    CloseWindow();
    return 0;
}