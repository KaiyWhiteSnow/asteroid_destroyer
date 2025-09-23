#ifndef ASTEROID_H
#define ASTEROID_H


#include <SFML/Graphics.h>
#include <SFML/Audio.h>


typedef struct {
    sfSprite* sprite;
    sfVector2f velocity;
    sfBool alive;
} Asteroid;


sfSprite* getRandomAsteroid();
sfVector2f randomAsteroidPosition();
float getAsteroidBaseVelocity();
Asteroid* getAsteroids();
int getMaxAsteroids();
void rainAsteroids(sfSprite* player, sfRenderWindow* window);
float distance(sfVector2f a, sfVector2f b);
int getRandomNumberInRange(int min, int max);
void updateAsteroids(float deltaTime);

#endif