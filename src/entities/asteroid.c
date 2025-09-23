#include <SFML/Graphics.h>
#include <SFML/Audio.h>
#include <stdlib.h>
#include <stdio.h>

#include "../../headers/entities/sprites.h"
#include "../../headers/graphics/window.h"


float asteroidStartingVelocity = 200.f;


sfSprite* getRandomAsteroid() {
    int side = rand() % 4;
    switch (side) {
        case 0: { return createSpriteFromTexture(getAsteroidTexture1()); };
        case 1: { return createSpriteFromTexture(getAsteroidTexture2()); };  
        case 2: { return createSpriteFromTexture(getAsteroidTexture3()); }; 
        case 3: { return createSpriteFromTexture(getAsteroidTexture4()); }; 
    }
}


float getAsteroidBaseVelocity(){ return asteroidStartingVelocity; }


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
