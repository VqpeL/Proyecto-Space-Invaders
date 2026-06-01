#pragma once
#include <raylib.h>
#include <stdbool.h>

#define ALTO_VENT 800
#define ANCHO_VENT 600
#define FPS 60
#define MAX_LASERS 50
#define MAX_SCORES 10

struct Nave{
    float x;
    float y;
    int vidas;

    float vel;
    float ancho;
    float alto;

    bool activo;
    Texture2D textura;
};

struct Alien{
    float x;
    float y;
    
    int puntos;
    int tipo;

    float vel;
    float ancho;
    float alto;

    bool activo;
};

struct Laser{
    float x;
    float y;

    float vel;

    bool activo;
};

struct AlienGrid{
    Alien** aliens;
    int num_col;
    int num_filas;

    float dirX;
    float vel;
    Texture2D textura[3];
};

struct ListaLaser{
    Laser lasers [MAX_LASERS];
    
    int cont;
};

struct EstadoJuego{
    int nivel;
    int vidas;
    int puntaje;

    bool perder;
    bool ganar;
};

struct HighScore{
    char nombre[50];
    char fecha[20];

    int puntos;
};

extern Nave Jugador;
extern Laser JugadorLaser, AlienLaser;
extern ListaLaser lista;
extern AlienGrid grid;
extern EstadoJuego estadoJuego;
extern Sound laserSound, explosionSound;
extern Music bgmusic;