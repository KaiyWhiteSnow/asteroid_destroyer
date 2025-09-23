#ifndef ASTEROID_H
#define ASTEROID_H


#include <SFML/Graphics.h>
#include <SFML/Audio.h>

void rainAsteroids(sfSprite* player, sfRenderWindow* window);
void fireBullet(sfSprite* player, sfVector2i mousePos, sfRenderWindow* window);
sfVector2f randomAsteroidPosition();
sfSprite* getRandomAsteroid();

void checkAsteroidBulletCollision();
void updateAsteroids();
void rainAsteroids(sfSprite* player, sfRenderWindow* window);


int getMaxAsteroids();
int getMaxBullets();


void checkBulletState(sfRenderWindow *window, int i);
void checkAsteroidState(sfRenderWindow *window, int i);


void updateBullets(float deltaTime);
void checkAsteroidBulletCollision(sfSound *asteroid_destructionfx);


void cleanup();

#endif