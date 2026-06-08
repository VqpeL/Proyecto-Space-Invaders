#include "raylib.h"
#include "background.h"
#include "jugador.h"
#include "aliens.h"
#include "proyectil.h"
#include "colisiones.h"
#include "cobertura.h"
#include "archivos.h"
#include <vector>

enum EstadoJuego
{
    PANTALLA_INICIO,
    PANTALLA_JUEGO,
    PANTALLA_VICTORIA,
    PANTALLA_FINAL,
    PANTALLA_HIGHSCORES
};

int main()
{
    Color azulCeleste = {135, 206, 250, 255};
    bool reset = true;
    const int cantidad_estrellas = 80;
    const int anchoPantalla = 800;
    const int altoPantalla = 800;
    int puntuacion = 0;
    float alturaAliens = 0.0f;
    char iniciales[4] = "\0";
    int letrasContadas = 0;
    bool calificaParaTop = false;
    bool recordGuardado = false;

    InitWindow(anchoPantalla, altoPantalla, "Space Invaders");
    SetTargetFPS(60);

    InitAudioDevice();
    Music musicaMenu = LoadMusicStream("assets/sonidos/musicaMenu.mp3");
    Music musicaJuego = LoadMusicStream("assets/sonidos/musicaJuego.mp3");
    Sound sonidoMuerte = LoadSound("assets/sonidos/explosion.wav");
    Sound sonidoLaser = LoadSound("assets/sonidos/laser.wav");
    Sound sonidoLaserAlien = LoadSound("assets/sonidos/laserAlien.wav");
    Sound golpeNave = LoadSound("assets/sonidos/golpeNave.wav");

    SetMusicVolume(musicaMenu, 0.3f);
    SetMusicVolume(musicaJuego, 0.1f);
    SetSoundVolume(sonidoMuerte, 0.2f);
    SetSoundVolume(sonidoLaser, 1.0f);
    SetSoundVolume(sonidoLaserAlien, 0.15f);
    SetSoundVolume(golpeNave, 1.0f);

    PlayMusicStream(musicaMenu);

    ConfigNivel datosNivel;

    AlienGrid AliensGrid;
    AliensGrid.aliens = nullptr;

    Proyectil *balasJugador = nullptr;
    Proyectil *balasAliens = nullptr;

    Nave jugador;
    jugador.textura.id = 0;
    Cobertura coberturas[MAX_COBERTURAS];
    int nivelActual = 0; // Debes de poner el "nivel al que quieres ir" - 1
    EstadoJuego estadoActual = PANTALLA_INICIO;

    Estrella *misEstrellas = InicializarBackground(cantidad_estrellas, anchoPantalla, altoPantalla);

    while (!WindowShouldClose())
    {
        ActualizarBackground(misEstrellas, cantidad_estrellas, anchoPantalla, altoPantalla);

        // Control del stream de audio según la pantalla
        if (estadoActual == PANTALLA_INICIO || estadoActual == PANTALLA_FINAL || estadoActual == PANTALLA_VICTORIA || estadoActual == PANTALLA_HIGHSCORES)
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
            else if (IsKeyPressed(KEY_H))
            {
                estadoActual = PANTALLA_HIGHSCORES;
            }
            break;

        case PANTALLA_HIGHSCORES:

            if (IsKeyPressed(KEY_ENTER))
            {
                estadoActual = PANTALLA_INICIO;
            }
            break;

        case PANTALLA_JUEGO:
        {
            // Reincio en caso de avanzar de nivel o ganar el juego
            if (reset)
            {
                nivelActual++;
                if (nivelActual <= 10)
                {
                    LiberarAlienGrid(&AliensGrid);
                    LiberarProyectiles(&balasJugador);
                    LiberarProyectiles(&balasAliens);

                    if (CargarConfigNivel(nivelActual, &datosNivel))
                    {

                        IniciaAlienGrid(&AliensGrid, datosNivel.filas, datosNivel.columnas, datosNivel.vel_max);
                    }
                    InicializarNave(&jugador, anchoPantalla, altoPantalla);
                    jugador.vidas = 3;
                    InicializarCoberturas(coberturas, MAX_COBERTURAS, anchoPantalla, altoPantalla);
                    jugador.posicion.x = (float)anchoPantalla / 2.0f - (jugador.ancho / 2.0f);

                    reset = false;
                }
                else
                {
                    estadoActual = PANTALLA_VICTORIA;
                    calificaParaTop = EsPuntajeAlto(puntuacion);
                    recordGuardado = false;
                    letrasContadas = 0;
                    iniciales[0] = '\0';
                    reset = false;
                    continue;
                }
            }

            // Validación de derrota o victoria
            if (jugador.vidas <= 0 || AlienSalioPantalla(&AliensGrid, altoPantalla))
            {
                StopMusicStream(musicaJuego);
                PlayMusicStream(musicaMenu);
                estadoActual = PANTALLA_FINAL;
                calificaParaTop = EsPuntajeAlto(puntuacion);
                recordGuardado = false;
                letrasContadas = 0;
                iniciales[0] = '\0';
                continue;
            }
            else if (GridVacia(&AliensGrid))
            {
                reset = true;
                float bono = (600.0f - alturaAliens) / 500.0f;
                if (bono < 0)
                {
                    bono = 0;
                }
                puntuacion += datosNivel.puntos_base + (int)(datosNivel.puntos_base * 5.0f * bono);
                continue;
            }

            if (AliensGrid.aliens != nullptr && AliensGrid.num_filas > 0)
            {
                alturaAliens = AliensGrid.aliens[0][0].y;
            }

            int vidasAntes = jugador.vidas;

            ActualizarNave(&jugador, anchoPantalla);
            ActualizarAlienGrid(&AliensGrid, anchoPantalla);
            ActualizarProyectiles(&balasJugador, altoPantalla);
            ActualizarProyectiles(&balasAliens, altoPantalla);

            // Bloque de colisiones
            ColisionProyectilesCoberturas(&balasJugador, coberturas, MAX_COBERTURAS);
            ColisionProyectilesCoberturas(&balasAliens, coberturas, MAX_COBERTURAS);
            ColisionAlienCobertura(&AliensGrid, coberturas, MAX_COBERTURAS);
            ColisionLaserAlien(&balasJugador, &AliensGrid, sonidoMuerte);
            ColisionAlienNave(&jugador, &AliensGrid);
            ColisionProyectilEnemigoNave(&balasAliens, &jugador);

            // Control de disparo e inserción de audio del jugador
            if (IsKeyPressed(KEY_SPACE))
            {
                Vector2 posDisparo = {jugador.posicion.x + (jugador.ancho / 2.0f), jugador.posicion.y};
                Disparar(&balasJugador, posDisparo, -7.0f);
                PlaySound(sonidoLaser);
            }

            // Disparo aleatorio de los aliens
            if (GetRandomValue(0, 100) < datosNivel.frecuencia_disparo)
            {
                int col_aliens = AliensGrid.num_col;
                int filas_aliens = AliensGrid.num_filas;
                Vector2 posAlien = AlienAleatorio(&AliensGrid, filas_aliens, col_aliens);
                Disparar(&balasAliens, posAlien, 5.0f);
                PlaySound(sonidoLaserAlien);
            }

            // Sonido si el jugador recibe daño
            if (jugador.vidas < vidasAntes)
            {
                PlaySound(golpeNave);
            }
            break;
        }

        case PANTALLA_VICTORIA:
        case PANTALLA_FINAL:
        {

            if (!reset)
            {
                LiberarProyectiles(&balasJugador);
                LiberarProyectiles(&balasAliens);
                LiberarAlienGrid(&AliensGrid);
                LiberarNave(&jugador);
                reset = true;
            }
            if (calificaParaTop && !recordGuardado)
            {
                int tecla = GetCharPressed();

                while (tecla > 0)
                {
                    if ((tecla >= 32) && (tecla <= 125) && (letrasContadas < 3))
                    {
                        iniciales[letrasContadas] = (char)tecla;
                        iniciales[letrasContadas + 1] = '\0';
                        letrasContadas++;
                    }
                    tecla = GetCharPressed();
                }

                if (IsKeyPressed(KEY_BACKSPACE))
                {
                    letrasContadas--;
                    if (letrasContadas < 0)
                        letrasContadas = 0;
                    iniciales[letrasContadas] = '\0';
                }

                if (IsKeyPressed(KEY_ENTER) && letrasContadas == 3)
                {
                    GuardarHighScore(iniciales, puntuacion);
                    recordGuardado = true;
                }
            }
            else
            {
                if (IsKeyPressed(KEY_ENTER))
                {
                    nivelActual = 0;
                    puntuacion = 0;
                    estadoActual = PANTALLA_INICIO;
                }
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
            DrawText("Presiona H para ver HIGHSCORES", anchoPantalla / 2 - MeasureText("Presiona H para ver HIGHSCORES", 18) / 2, 400, 18, GRAY);
            break;

        case PANTALLA_HIGHSCORES:
        {
            DrawText("GALACTIC HALL OF FAME", anchoPantalla / 2 - MeasureText("GALACTIC HALL OF FAME", 40) / 2, 80, 40, GREEN);
            DrawText("TOP 10 MEJORES PILOTOS", anchoPantalla / 2 - MeasureText("TOP 10 MEJORES PILOTOS", 18) / 2, 135, 18, azulCeleste);

            FILE *archivo = fopen("assets/data/highscores.txt", "r");
            if (archivo != NULL)
            {
                char nombreTemp[4];
                int puntosTemp;
                int yPos = 200;
                int puesto = 1;

                while (fscanf(archivo, "%3[^,],%d\n", nombreTemp, &puntosTemp) != EOF && puesto <= 10)
                {

                    const char *renglonScore = TextFormat("%02d.  %-3s   %06d", puesto, nombreTemp, puntosTemp);

                    Color colorTexto = (puesto == 1) ? YELLOW : RAYWHITE;

                    DrawText(renglonScore, anchoPantalla / 2 - 140, yPos, 22, colorTexto);

                    yPos += 35;
                    puesto++;
                }
                fclose(archivo);
            }
            else
            {
                DrawText("No se pudo cargar la base de datos.", anchoPantalla / 2 - MeasureText("No se pudo cargar la base de datos.", 20) / 2, 300, 20, RED);
            }

            DrawText("Presiona ENTER para volver al menú", anchoPantalla / 2 - MeasureText("Presiona ENTER para volver al menú", 18) / 2, 650, 18, GRAY);
            break;
        }

        case PANTALLA_JUEGO:
            if (!reset)
            {

                DibujarAlienGrid(&AliensGrid);
                DibujarCoberturas(coberturas, MAX_COBERTURAS);
                DibujarNave(&jugador);
                DibujarProyectiles(balasJugador, YELLOW);
                DibujarProyectiles(balasAliens, azulCeleste);

                DrawText("SCORE", 40, 15, 20, RAYWHITE);
                DrawText(TextFormat("%06d", puntuacion), 40, 40, 20, GREEN);

                DrawText(TextFormat("NIVEL: %d", nivelActual), 350, 40, 20, azulCeleste);

                DrawText(TextFormat("VIDAS: %d", jugador.vidas), anchoPantalla - 150, 40, 20, RED);
            }
            break;

        case PANTALLA_VICTORIA:
            DrawText("¡VICTORIA!", anchoPantalla / 2 - MeasureText("¡VICTORIA!", 50) / 2, 120, 50, GREEN);
            DrawText("Has salvado la galaxia", anchoPantalla / 2 - MeasureText("Has salvado la galaxia", 20) / 2, 190, 20, RAYWHITE);

            DrawText(TextFormat("PUNTUACIÓN FINAL: %06d", puntuacion), anchoPantalla / 2 - MeasureText(TextFormat("PUNTUACIÓN FINAL: %06d", puntuacion), 25) / 2, 250, 25, YELLOW);

            if (calificaParaTop && !recordGuardado)
            {
                DrawText("¡NUEVO RÉCORD LOCAL!", anchoPantalla / 2 - MeasureText("¡NUEVO RÉCORD LOCAL!", 20) / 2, 340, 20, RED);
                DrawText("Escribe tus 3 iniciales y presiona ENTER:", anchoPantalla / 2 - MeasureText("Escribe tus 3 iniciales y presiona ENTER:", 20) / 2, 380, 20, RAYWHITE);

                DrawText(iniciales, anchoPantalla / 2 - MeasureText(iniciales, 40) / 2, 440, 40, GREEN);
                DrawRectangleLines(anchoPantalla / 2 - 60, 435, 120, 50, GRAY);
            }
            else
            {
                if (recordGuardado)
                    DrawText("¡PUNTUACIÓN GUARDADA CON ÉXITO!", anchoPantalla / 2 - MeasureText("¡PUNTUACIÓN GUARDADA CON ÉXITO!", 18) / 2, 360, 18, GREEN);
                DrawText("Presiona ENTER para volver al menú", anchoPantalla / 2 - MeasureText("Presiona ENTER para volver al menú", 20) / 2, 450, 20, GRAY);
            }
            break;

        case PANTALLA_FINAL:
            DrawText("GAME OVER", anchoPantalla / 2 - MeasureText("GAME OVER", 50) / 2, 120, 50, RED);

            DrawText(TextFormat("PUNTUACIÓN FINAL: %06d", puntuacion), anchoPantalla / 2 - MeasureText(TextFormat("PUNTUACIÓN FINAL: %06d", puntuacion), 25) / 2, 220, 25, YELLOW);

            if (calificaParaTop && !recordGuardado)
            {
                DrawText("¡A PESAR DE PERDER ENTRASTE AL TOP 10!", anchoPantalla / 2 - MeasureText("¡A PESAR DE PERDER ENTRASTE AL TOP 10!", 18) / 2, 320, 18, GREEN);
                DrawText("Escribe tus iniciales y presiona ENTER:", anchoPantalla / 2 - MeasureText("Escribe tus iniciales y presiona ENTER:", 20) / 2, 360, 20, RAYWHITE);

                DrawText(iniciales, anchoPantalla / 2 - MeasureText(iniciales, 40) / 2, 420, 40, GREEN);
                DrawRectangleLines(anchoPantalla / 2 - 60, 415, 120, 50, GRAY);
            }
            else
            {
                if (recordGuardado)
                    DrawText("¡PUNTUACIÓN GUARDADA CON ÉXITO!", anchoPantalla / 2 - MeasureText("¡PUNTUACIÓN GUARDADA CON ÉXITO!", 18) / 2, 340, 18, GREEN);
                DrawText("Presiona ENTER para volver al menú", anchoPantalla / 2 - MeasureText("Presiona ENTER para volver al menú", 20) / 2, 450, 20, RAYWHITE);
            }
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

    if (!reset)
    {
        LiberarProyectiles(&balasJugador);
        LiberarProyectiles(&balasAliens);
        LiberarAlienGrid(&AliensGrid);
        LiberarNave(&jugador);
    }
    LiberarBackground(misEstrellas);
    CloseWindow();
    return 0;
}