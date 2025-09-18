#include <SFML/Graphics.h>
#include <stdio.h>

sfTexture*  playerTexture    = NULL;
sfTexture*  bulletTexture    = NULL;
sfTexture*  asteroidTexture  = NULL;

// --- Resource management ---
int loadResources(void) {
    playerTexture = sfTexture_createFromFile("./sprites/player.png", NULL);
    if (!playerTexture) {
        fprintf(stderr, "Failed to load player texture\n");
        return 0;
    }
    bulletTexture = sfTexture_createFromFile("./sprites/bullet.png", NULL);
    if (!bulletTexture) {
        fprintf(stderr, "Failed to load bullet texture\n");
        return 0;
    }
    asteroidTexture = sfTexture_createFromFile("./sprites/asteroid.png", NULL);
    if (!asteroidTexture){
        fprintf(stderr, "Failed to load asteroid texture\n");
    }
    return 1;
}

void freeResources(void) {
    if (playerTexture)      { sfTexture_destroy(playerTexture);     playerTexture   = NULL; }
    if (bulletTexture)      { sfTexture_destroy(bulletTexture);     bulletTexture   = NULL; }
    if (asteroidTexture)    { sfTexture_destroy(asteroidTexture);   asteroidTexture = NULL; }
}


// Create a sprite from an already-loaded texture (returns NULL on failure)
sfSprite* createSpriteFromTexture(sfTexture* tex) {
    if (!tex) return NULL;
    sfSprite* s = sfSprite_create();
    if (!s) return NULL;
    sfSprite_setTexture(s, tex, sfTrue); 
    return s;
}



sfTexture* getPlayerTexture()   { return playerTexture;     }
sfTexture* getBulletTexture()   { return bulletTexture;     }
sfTexture* getAsteroidTexture() { return asteroidTexture;   }
