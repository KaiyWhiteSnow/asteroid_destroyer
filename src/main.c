#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include <SFML/Graphics.h>
#include <SFML/System/Vector2.h>
#include <SFML/Window/VideoMode.h>
#include <SFML/Audio.h>


#include "../headers/graphics/color.h"
#include "../headers/graphics/window.h"
#include "../headers/core/time.h"
#include "../headers/core/movement.h"
#include "../headers/entities/sprites.h"
#include "../headers/entities/bullet.h"
#include "../headers/entities/asteroid.h"
#include "../headers/audio/sound.h"


// --- Prototypes --- Will move into proper files later
void update(sfSprite* player, sfRenderWindow* window);
int wasSpriteCreated(sfSprite *sprite, sfRenderWindow *window);
void bulletAsteroidCollisionHandler(sfSound *destroyAsteroidfx);


// --- Main ---
int main() {
    sfRenderWindow* window;
    sfEvent event;

    window = sfRenderWindow_create(getMode(), "Asteroid Destroyer", sfResize | sfClose, NULL);
    if (!window) return 1;
    sfRenderWindow_setFramerateLimit(window, 60);

    if (!loadResources()) {
        sfRenderWindow_destroy(window);
        return 1;
    }


    // Player sprite
    sfSprite* player = createSpriteFromTexture(getPlayerTexture());
    sfSprite* asteroid1 = createSpriteFromTexture(getAsteroidTexture1());
    sfSprite* asteroid2 = createSpriteFromTexture(getAsteroidTexture2());
    sfSprite* asteroid3 = createSpriteFromTexture(getAsteroidTexture3());
    sfSprite* asteroid4 = createSpriteFromTexture(getAsteroidTexture4());
    

    wasSpriteCreated(player, window);
    wasSpriteCreated(asteroid1, window);
    wasSpriteCreated(asteroid2, window);
    wasSpriteCreated(asteroid3, window);
    wasSpriteCreated(asteroid4, window);

    
    // TODO: Add check to make sure these are created
    sfSound *shootfx = createSFX("./assets/sound/shoot.mp3", 40);
    sfSound *destroyAsteroidfx = createSFX("./assets/sound/destroy_asteroid.mp3", 40);
    sfMusic *music = createMusic("./assets/music/sabaton_acesinexile.mp3", 60);
    

    sfMusic_play(music);


    sfSprite_setPosition(player, getDefaultPlayerPosition());
    sfFloatRect pbounds = sfSprite_getLocalBounds(player);
    sfSprite_setOrigin(player, (sfVector2f){pbounds.width / 2, pbounds.height / 2});

    // Game loop
    while (sfRenderWindow_isOpen(window)) {
        float deltaTime = restartDeltaTime();   
        
        updateFireTimer(deltaTime);

        // Process events
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) sfRenderWindow_close(window);
            checkMovementInput(event);            
        }

        // Player movement
        handleMovement(player, deltaTime);

        // Player rotation (towards mouse)
        handleRotation(player, window);

        // Handle asteroids
        rainAsteroids(player, window);

        // Fire bullet
        shootBullets(shootfx, window, player);


        updateBullets(deltaTime);


        updateAsteroids(deltaTime);


        bulletAsteroidCollisionHandler(destroyAsteroidfx);

        // Update window
        update(player, window);
    }

    
    // Cleanup
    for (int i = 0; i < getMaxBullets(); i++) {
        if (getBullets()[i].alive && getBullets()[i].sprite) {
            sfSprite_destroy(getBullets()[i].sprite);
            getBullets()[i].sprite = NULL;
            getBullets()[i].alive = sfFalse;
        }
    }
    
    sfSprite_destroy(player);
    freeResources();
    sfRenderWindow_destroy(window);
    sfClock_destroy(getClock());

    return 0;
}

// --- Functions ---
void bulletAsteroidCollisionHandler(sfSound *destroyAsteroidfx){
    // Bullet–Asteroid Collision
    for (int i = 0; i < getMaxBullets(); i++) {
        if (!getBullets()[i].alive && !getBullets()[i].sprite) { continue; }
        sfVector2f bpos = sfSprite_getPosition(getBullets()[i].sprite);
        
        float br = 4.f; 

        for (int j = 0; j < getMaxAsteroids(); j++) {
            if (!getAsteroids()[j].alive && !getAsteroids()[j].sprite) { continue; }
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


int wasSpriteCreated(sfSprite *sprite, sfRenderWindow *window){
    if (!sprite) {
        fprintf(stderr, "Failed to create sprite\n");
        freeResources();
        sfRenderWindow_destroy(window);
        return 1;
    }
    return 0;
}


void update(sfSprite* player, sfRenderWindow* window) {
    sfRenderWindow_clear(window, sfGray());
    if (player)
        sfRenderWindow_drawSprite(window, player, NULL);
    for (int i = 0; i < getMaxBullets(); i++) {
        if (getBullets()[i].alive && getBullets()[i].sprite)
            sfRenderWindow_drawSprite(window, getBullets()[i].sprite, NULL);
    }
    for (int i = 0; i < getMaxAsteroids(); i++) {
        if (getAsteroids()[i].alive && getAsteroids()[i].sprite)
        sfRenderWindow_drawSprite(window, getAsteroids()[i].sprite, NULL);
    }

    sfRenderWindow_display(window);
}
