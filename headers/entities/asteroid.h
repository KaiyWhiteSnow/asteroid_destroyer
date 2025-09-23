#ifndef ASTEROID_H
#define ASTEROID_H


#include <SFML/Graphics.h>
#include <SFML/Audio.h>

sfSprite* getRandomAsteroid();
sfVector2f randomAsteroidPosition();
float getAsteroidBaseVelocity();

#endif