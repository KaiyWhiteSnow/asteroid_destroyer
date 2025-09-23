#include <SFML/Graphics.h>
#include <SFML/Audio.h>
#include <stdlib.h>


#include "../../headers/entities/sprites.h"


sfSprite* getRandomAsteroid(){
    int side = rand() % 4;
    switch (side) {
        case 0: { return createSpriteFromTexture(getAsteroidTexture1()); };
        case 1: { return createSpriteFromTexture(getAsteroidTexture2()); };  
        case 2: { return createSpriteFromTexture(getAsteroidTexture3()); }; 
        case 3: { return createSpriteFromTexture(getAsteroidTexture4()); }; 
    }
}