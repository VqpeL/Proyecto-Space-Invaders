#include "archivos.h"
#include <string.h>

bool CargarConfigNivel(int nivelObjetivo, ConfigNivel *config)
{

    FILE *archivo = fopen("assets/data/niveles.txt", "r");

    if (archivo == NULL)
    {
        printf("Error: No se pudo abrir el archivo de niveles.\n");
        return false;
    }

    int n, f, c, freq, pts;
    float vel;

    while (fscanf(archivo, "%d,%d,%d,%f,%d,%d\n", &n, &f, &c, &vel, &freq, &pts) != EOF)
    {

        if (n == nivelObjetivo)
        {
            config->nivel = n;
            config->filas = f;
            config->columnas = c;
            config->vel_max = vel;
            config->frecuencia_disparo = freq;
            config->puntos_base = pts;

            fclose(archivo);
            return true;
        }
    }

    fclose(archivo);
    return false;
}

void GuardarHighScore(const char *nombreJugador, int puntajeFinal)
{
    HighScore lista[11];
    int total_scores = 0;

    FILE *archivoLectura = fopen("assets/data/highscores.txt", "r");
    if (archivoLectura != NULL)
    {

        while (fscanf(archivoLectura, "%3[^,],%d\n", lista[total_scores].nombre, &lista[total_scores].puntos) != EOF)
        {
            total_scores++;
            if (total_scores >= 10)
                break;
        }
        fclose(archivoLectura);
    }

    strncpy(lista[total_scores].nombre, nombreJugador, 3);
    lista[total_scores].nombre[3] = '\0';
    lista[total_scores].puntos = puntajeFinal;
    total_scores++;

    for (int i = 0; i < total_scores - 1; i++)
    {
        for (int j = 0; j < total_scores - i - 1; j++)
        {
            if (lista[j].puntos < lista[j + 1].puntos)
            {

                HighScore temp = lista[j];
                lista[j] = lista[j + 1];
                lista[j + 1] = temp;
            }
        }
    }

    if (total_scores > 10)
        total_scores = 10;

    FILE *archivoEscritura = fopen("assets/data/highscores.txt", "w");
    if (archivoEscritura == NULL)
    {
        printf("Error: No se pudo escribir el archivo de puntajes.\n");
        return;
    }

    for (int i = 0; i < total_scores; i++)
    {

        fprintf(archivoEscritura, "%s,%d\n", lista[i].nombre, lista[i].puntos);
    }

    fclose(archivoEscritura);
}

bool EsPuntajeAlto(int puntuacionJugador)
{
    FILE *archivo = fopen("assets/data/highscores.txt", "r");
    if (archivo == NULL)
    {

        return true;
    }

    char nombreTemp[4];
    int puntosTemp;
    int conteo = 0;
    int peorPuntaje = 0;

    while (fscanf(archivo, "%3[^,],%d\n", nombreTemp, &puntosTemp) != EOF)
    {
        conteo++;
        peorPuntaje = puntosTemp;
    }
    fclose(archivo);

    if (conteo < 10)
        return true;
    return (puntuacionJugador > peorPuntaje);
}