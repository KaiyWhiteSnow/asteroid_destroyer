// System imports
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Library imports
#include <SFML/Graphics.h>
#include <SFML/System/Vector2.h>
#include <SFML/Window/VideoMode.h>
#include <SFML/Audio.h>

// Custom imports
#include "../headers/graphics/color.h"
#include "../headers/graphics/window.h"
#include "../headers/core/time.h"
#include "../headers/core/collisions.h"
#include "../headers/core/movement.h"
#include "../headers/entities/sprites.h"
#include "../headers/entities/bullet.h"
#include "../headers/entities/asteroid.h"
#include "../headers/audio/sound.h"


// --- Prototypes --- Will move into proper files later
void update(sfSprite* player, sfRenderWindow* window);
int wasSpriteCreated(sfSprite *sprite, sfRenderWindow *window);


// --- Main ---
int main() {
    // --- SFML setup ---
    sfRenderWindow* window;
    sfEvent event;

    // --- Create window ---
    window = sfRenderWindow_create(getMode(), "Asteroid Destroyer", sfResize | sfClose, NULL);
    if (!window) return 1;

    if (!loadResources()) {
        sfRenderWindow_destroy(window);
        return 1;
    }

    // Setup window 
    sfRenderWindow_setFramerateLimit(window, 60);

    // --- Sprites ---
    sfSprite* player    = createSpriteFromTexture(getPlayerTexture());
    sfSprite* asteroid1 = createSpriteFromTexture(getAsteroidTexture1());
    sfSprite* asteroid2 = createSpriteFromTexture(getAsteroidTexture2());
    sfSprite* asteroid3 = createSpriteFromTexture(getAsteroidTexture3());
    sfSprite* asteroid4 = createSpriteFromTexture(getAsteroidTexture4());
    
    // Make sure sprites were created properly 
    // TODO: Needs testing
    wasSpriteCreated(player, window);
    wasSpriteCreated(asteroid1, window);
    wasSpriteCreated(asteroid2, window);
    wasSpriteCreated(asteroid3, window);
    wasSpriteCreated(asteroid4, window);
    
    // Player setup
    sfSprite_setPosition(player, getDefaultPlayerPosition());
    sfFloatRect pbounds = sfSprite_getLocalBounds(player);
    sfSprite_setOrigin(player, (sfVector2f){pbounds.width / 2, pbounds.height / 2});

    // Sound and music setup
    // TODO: Add check to make sure these are created
    sfSound *shootfx            = createSFX("./assets/sound/shoot.mp3", 40);
    sfSound *destroyAsteroidfx  = createSFX("./assets/sound/destroy_asteroid.mp3", 60);
    sfMusic *music              = createMusic("./assets/music/sabaton_acesinexile.mp3", 60);
    
    // TODO: Make the music random, and start new song on old song stop
    sfMusic_play(music);


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

        // Update asteroid position
        updateAsteroids(deltaTime);


        // Fire bullet
        shootBullets(shootfx, window, player);

        // Update bullet position
        updateBullets(deltaTime);


        // Check for bullet/asteroid collisions
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

// Check if sprite was created
// Returns 1 on failed to create sprite
// Returns 0 on sprite created
// TODO: Test this properly
int wasSpriteCreated(sfSprite *sprite, sfRenderWindow *window){
    if (!sprite) {
        fprintf(stderr, "Failed to create sprite\n");
        freeResources();
        sfRenderWindow_destroy(window);
        return 1;
    }
    return 0;
}

// Draw, render and handle display
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
