#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include <SFML/Graphics.h>
#include <SFML/System/Vector2.h>
#include <SFML/Window/VideoMode.h>
#include <SFML/Audio.h>


#include "../../headers/graphics/window.h"
#include "../../headers/entities/sprites.h"
#include "../../headers/entities/bullet.h"
#include "../../headers/entities/asteroid.h"
#include "../../headers/audio/sound.h"

// Handles bullet/asteroids collisions
// TODO: Refactor this to make it more resource efficient
void bulletAsteroidCollisionHandler(sfSound *destroyAsteroidfx){
    // Bullet–Asteroid Collision
    for (int i = 0; i < getMaxBullets(); i++) {
        if (!getBullets()[i].alive || !getBullets()[i].sprite) { continue; }
        sfVector2f bpos = sfSprite_getPosition(getBullets()[i].sprite);
        
        float br = 4.f; 

        for (int j = 0; j < getMaxAsteroids(); j++) {
            if (!getAsteroids()[j].alive || !getAsteroids()[j].sprite) { continue; }
            sfVector2f apos = sfSprite_getPosition(getAsteroids()[j].sprite);

            // Approximate asteroid hit radius (about half width of sprite)
            float ar = sfSprite_getGlobalBounds(getAsteroids()[j].sprite).width * 0.3f;

            if (distance(bpos, apos) < br + ar) {
                // --- Collision! ---
                sfSound_play(destroyAsteroidfx);
                sfSprite_destroy(getBullets()[i].sprite);
                getBullets()[i].sprite = NULL;
                getBullets()[i].alive = sfFalse;

                sfSprite_destroy(getAsteroids()[j].sprite);
                getAsteroids()[j].sprite = NULL;
                getAsteroids()[j].alive = sfFalse;

                break; // stop checking this bullet
            }
        }
    }
}