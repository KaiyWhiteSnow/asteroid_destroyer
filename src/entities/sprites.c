#include <SFML/Graphics.h>
#include <stdio.h>


// Textures init

sfTexture*  playerTexture       = NULL;
sfTexture*  bulletTexture       = NULL;
sfTexture*  asteroidTexture1    = NULL;
sfTexture*  asteroidTexture2    = NULL;
sfTexture*  asteroidTexture3    = NULL;
sfTexture*  asteroidTexture4    = NULL;


// --- Resource management ---

// Load all resources
// TODO: Consider loading with automated system
int loadResources(void) {
    playerTexture = sfTexture_createFromFile("./assets/sprites/player.png", NULL);
    if (!playerTexture) {
        fprintf(stderr, "Failed to load player texture\n");
        return 0;
    }
    bulletTexture = sfTexture_createFromFile("./assets/sprites/bullet.png", NULL);
    if (!bulletTexture) {
        fprintf(stderr, "Failed to load bullet texture\n");
        return 0;
    }
    asteroidTexture1 = sfTexture_createFromFile("./assets/sprites/asteroid1.png", NULL);
    if (!asteroidTexture1){
        fprintf(stderr, "Failed to load asteroid1 texture\n");
        return 0;
    }
    asteroidTexture2 = sfTexture_createFromFile("./assets/sprites/asteroid2.png", NULL);
    if (!asteroidTexture2){
        fprintf(stderr, "Failed to load asteroid2 texture\n");
        return 0;
    }
    asteroidTexture3 = sfTexture_createFromFile("./assets/sprites/asteroid3.png", NULL);
    if (!asteroidTexture3){
        fprintf(stderr, "Failed to load asteroid3 texture\n");
        return 0;
    }
    asteroidTexture4 = sfTexture_createFromFile("./assets/sprites/asteroid4.png", NULL);
    if (!asteroidTexture4){
        fprintf(stderr, "Failed to load asteroid4 texture\n");
        return 0;
    }

    return 1;
}


// Free on application finish
void freeResources(void) {
    if (playerTexture)      { sfTexture_destroy(playerTexture);      playerTexture       = NULL; }
    if (bulletTexture)      { sfTexture_destroy(bulletTexture);      bulletTexture       = NULL; }
    if (asteroidTexture1)   { sfTexture_destroy(asteroidTexture1);   asteroidTexture1    = NULL; }
    if (asteroidTexture2)   { sfTexture_destroy(asteroidTexture2);   asteroidTexture2    = NULL; }
    if (asteroidTexture3)   { sfTexture_destroy(asteroidTexture3);   asteroidTexture3    = NULL; }
    if (asteroidTexture4)   { sfTexture_destroy(asteroidTexture4);   asteroidTexture4    = NULL; }
}


// Create a sprite from an already-loaded texture (returns NULL on failure)
sfSprite* createSpriteFromTexture(sfTexture* tex) {
    if (!tex) return NULL;
    sfSprite* s = sfSprite_create();
    if (!s) return NULL;
    sfSprite_setTexture(s, tex, sfTrue); 
    return s;
}


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

// Getters

sfTexture* getPlayerTexture()      {   return playerTexture;         }
sfTexture* getBulletTexture()      {   return bulletTexture;         }
sfTexture* getAsteroidTexture1()   {   return asteroidTexture1;      }
sfTexture* getAsteroidTexture2()   {   return asteroidTexture2;      }
sfTexture* getAsteroidTexture3()   {   return asteroidTexture3;      }
sfTexture* getAsteroidTexture4()   {   return asteroidTexture4;      }
