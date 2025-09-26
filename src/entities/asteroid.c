#include <SFML/Graphics.h>
#include <SFML/Audio.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../../headers/entities/sprites.h"
#include "../../headers/graphics/window.h"
#include "../../headers/core/movement.h"
#include "../../headers/entities/asteroid.h"


// Constants
int MAX_ASTEROIDS = 50;


// Arrays
Asteroid asteroids[50];


// non-const Variables
float asteroidStartingVelocity = 200.f;

// Returns a random asteroid (decided on compile, not runtime)
sfSprite* getRandomAsteroid() {
    int side = rand() % 4;
    switch (side) {
        case 0: { return createSpriteFromTexture(getAsteroidTexture1()); };
        case 1: { return createSpriteFromTexture(getAsteroidTexture2()); };  
        case 2: { return createSpriteFromTexture(getAsteroidTexture3()); }; 
        case 3: { return createSpriteFromTexture(getAsteroidTexture4()); }; 
    }
}


// Get a random position behind the border of the screen
sfVector2f randomAsteroidPosition() {
    int side = rand() % 4; // 0=top, 1=right, 2=bottom, 3=left
    switch (side) {
        case 0: return (sfVector2f){rand() % getWidth(), -50};           // top
        case 1: return (sfVector2f){getWidth() + 50, rand() % getHeight()};   // right
        case 2: return (sfVector2f){rand() % getWidth(), getHeight() + 50};   // bottom
        case 3: return (sfVector2f){-50, rand() % getHeight()};          // left
    }
    return (sfVector2f){0, 0}; // fallback in case weird stuff happens
}


// Update asteroid position
void updateAsteroids(float deltaTime){
    for (int i = 0; i < getMaxAsteroids(); i++) {
        if (asteroids[i].alive && asteroids[i].sprite) {
            sfVector2f apos = sfSprite_getPosition(asteroids[i].sprite);
            apos.x += asteroids[i].velocity.x * deltaTime;  
            apos.y += asteroids[i].velocity.y * deltaTime;
            sfSprite_setPosition(asteroids[i].sprite, apos);

            if (apos.x < -200 || apos.x > getWidth() + 200 || apos.y < -200 || apos.y > getHeight() + 200) {
                if (asteroids[i].sprite) {
                    sfSprite_destroy(asteroids[i].sprite);
                    asteroids[i].sprite = NULL;
                }
                asteroids[i].alive = sfFalse;
            }
        }
    }
}

// Asteroid handler
void rainAsteroids(sfSprite* player, sfRenderWindow* window){
    for (int i = 0; i < getMaxAsteroids(); i++) {
        if (!asteroids[i].alive) {
            sfSprite* asteroidSprite = getRandomAsteroid();
            if (!asteroidSprite) {
                fprintf(stderr, "Couldn't make sprite for asteroid\n");
                return;
            }
            sfSprite_setPosition(asteroidSprite, randomAsteroidPosition());
            sfSprite_setScale(asteroidSprite, (sfVector2f){0.5f, 0.5f});

            asteroids[i].sprite = asteroidSprite;
            sfVector2f playerPos = sfSprite_getPosition(player);
            sfVector2f apos = sfSprite_getPosition(asteroidSprite);

            float dx = playerPos.x - apos.x + getRandomNumberInRange(-50, 50);
            float dy = playerPos.y - apos.y + getRandomNumberInRange(-50, 50);
            float length = sqrtf(dx*dx + dy*dy);
            if (length == 0.f) length = 1.f;

            asteroids[i].velocity.x = (dx / length) * getAsteroidBaseVelocity();
            asteroids[i].velocity.y = (dy / length) * getAsteroidBaseVelocity();

            float angle = atan2f(dy, dx) * 180.f / getPI();
            sfSprite_setRotation(asteroidSprite, angle - 90.f);

            asteroids[i].alive = sfTrue;
            break;
        }
    }
}

// Calculate distance between 2 points
float distance(sfVector2f a, sfVector2f b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return sqrtf(dx * dx + dy * dy);
}

// Getters

int getMaxAsteroids(){ return MAX_ASTEROIDS; }
int getRandomNumberInRange(int min, int max){ return (int) rand() % max - min + 1; }
float getAsteroidBaseVelocity(){ return asteroidStartingVelocity; }
Asteroid* getAsteroids(){ return asteroids; }
