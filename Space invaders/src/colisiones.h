#ifndef COLISIONES_H
#define COLISIONES_H

#include "raylib.h"
#include "jugador.h"
#include "aliens.h"
#include "proyectil.h"

void ColisionLaserAlien(Proyectil** cabezaLasers, AlienGrid *grid);
void ColisionAlienNave(Nave *jugador, AlienGrid *grid);
void ColisionProyectilEnemigoNave(Proyectil** cabezaLasersAliens, Nave *jugador);

#endif