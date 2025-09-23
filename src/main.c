#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SFML/Graphics.h>
#include <SFML/System/Vector2.h>
#include <SFML/Window/VideoMode.h>
#include <SFML/Audio.h>

#include "../headers/color.h"
#include "../headers/window.h"
#include "../headers/time.h"
#include "../headers/movement.h"
#include "../headers/sprites.h"
#include "../headers/sound.h"

#define MAX_BULLETS 500
#define MAX_ASTEROIDS 30


const unsigned short int WIDTH = 800;
const unsigned short int HEIGHT = 600;

// Bullet struct
typedef struct {
    sfSprite* sprite;
    sfVector2f velocity;
    sfBool alive;
} Bullet;

typedef struct {
    sfSprite* sprite;
    sfVector2f velocity;
    sfBool alive;
} Asteroid;


Bullet bullets[MAX_BULLETS];
Asteroid asteroids[MAX_ASTEROIDS];


// Globals
float bulletVelocity = 400.f;
float asteroidStartingVelocity = 200.f;
float fireDelay = 0.2f;   // seconds between shots
float fireTimer = 0.f;    // counts down



// --- Prototypes --- Will move into proper files later
void update(sfSprite* player, sfRenderWindow* window);
void fireBullet(sfSprite* player, sfVector2i mousePos, sfRenderWindow* window);
void rainAsteroids(sfSprite* player, sfRenderWindow* window);
sfVector2f randomAsteroidPosition();
sfSprite* getRandomAsteroid();
int getRandomNumberInRange(int min, int max);


float distance(sfVector2f a, sfVector2f b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return sqrtf(dx * dx + dy * dy);
}


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
    
    if (!player) {
        fprintf(stderr, "Failed to create player sprite\n");
        freeResources();
        sfRenderWindow_destroy(window);
        return 1;
    } 
    if (!asteroid1) {
        fprintf(stderr, "ast1 not workin"); 
        freeResources();
        sfRenderWindow_destroy(window);
        return 1;
    }
    if (!asteroid2) {
        fprintf(stderr, "ast2 not workin"); 
        freeResources();
        sfRenderWindow_destroy(window);
        return 1;
    }
    if (!asteroid3) {
        fprintf(stderr, "ast3 not workin"); 
        freeResources();
        sfRenderWindow_destroy(window);
        return 1;
    }
    if (!asteroid4) {
        fprintf(stderr, "ast4 not workin"); 
        freeResources();
        sfRenderWindow_destroy(window);
        return 1;
    }


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
    fireTimer -= deltaTime;
    if (fireTimer < 0.f) fireTimer = 0.f;   

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
        if (sfKeyboard_isKeyPressed(sfKeySpace) && fireTimer <= 0.f) {
            sfSound_play(shootfx);
            sfVector2i mousePos = sfMouse_getPositionRenderWindow(window);
            fireBullet(player, mousePos, window);
            fireTimer = fireDelay;  // reset cooldown
        }



        // Update bullets
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].alive && bullets[i].sprite) {
                sfVector2f bpos = sfSprite_getPosition(bullets[i].sprite);
                bpos.x += bullets[i].velocity.x * deltaTime;  
                bpos.y += bullets[i].velocity.y * deltaTime;
                sfSprite_setPosition(bullets[i].sprite, bpos);

                if (bpos.x < 0 || bpos.x > WIDTH || bpos.y < 0 || bpos.y > HEIGHT) {
                    sfSprite_destroy(bullets[i].sprite);
                    bullets[i].sprite = NULL;
                    bullets[i].alive = sfFalse;
                }
            }
        }
        
        // Update asteroids
        for (int i = 0; i < MAX_ASTEROIDS; i++) {
            if (asteroids[i].alive && asteroids[i].sprite) {
                sfVector2f apos = sfSprite_getPosition(asteroids[i].sprite);
                apos.x += asteroids[i].velocity.x * deltaTime;  
                apos.y += asteroids[i].velocity.y * deltaTime;
                sfSprite_setPosition(asteroids[i].sprite, apos);

                if (apos.x < -200 || apos.x > WIDTH + 200 || apos.y < -200 || apos.y > HEIGHT + 200) {
                    if (asteroids[i].sprite) {
                        sfSprite_destroy(asteroids[i].sprite);
                        asteroids[i].sprite = NULL;
                    }
                    asteroids[i].alive = sfFalse;
                }
            }
        }


        // Bullet–Asteroid Collision
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].alive && bullets[i].sprite) {
                sfVector2f bpos = sfSprite_getPosition(bullets[i].sprite);

                
                float br = 4.f; 

                for (int j = 0; j < MAX_ASTEROIDS; j++) {
                    if (asteroids[j].alive && asteroids[j].sprite) {
                        sfVector2f apos = sfSprite_getPosition(asteroids[j].sprite);

                        // Approximate asteroid hit radius (about half width of sprite)
                        float ar = sfSprite_getGlobalBounds(asteroids[j].sprite).width * 0.3f;

                        if (distance(bpos, apos) < br + ar) {
                            // --- Collision! ---
                            sfSound_play(destroyAsteroidfx);
                            sfSprite_destroy(bullets[i].sprite);
                            bullets[i].sprite = NULL;
                            bullets[i].alive = sfFalse;

                            sfSprite_destroy(asteroids[j].sprite);
                            asteroids[j].sprite = NULL;
                            asteroids[j].alive = sfFalse;

                            break; // stop checking this bullet
                        }
                    }
                }
            }
        }

        // Update window
        update(player, window);
    }

    
    // Cleanup
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].alive && bullets[i].sprite) {
            sfSprite_destroy(bullets[i].sprite);
            bullets[i].sprite = NULL;
            bullets[i].alive = sfFalse;
        }
    }
    
    sfSprite_destroy(player);
    freeResources();
    sfRenderWindow_destroy(window);
    sfClock_destroy(getClock());

    return 0;
}

// --- Functions ---
void fireBullet(sfSprite* player, sfVector2i mousePos, sfRenderWindow* window) {
    if (!player || !window || !getBulletTexture()) return;

    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].alive) {
            sfSprite* bulletSprite = createSpriteFromTexture(getBulletTexture());
            if (!bulletSprite) {
                fprintf(stderr, "Couldn't make sprite for bullet\n");
                return;
            }
            bullets[i].sprite = bulletSprite;
            sfVector2f playerPos = sfSprite_getPosition(player);
            sfSprite_setPosition(bulletSprite, playerPos);
            sfFloatRect bounds = sfSprite_getLocalBounds(bulletSprite);
            sfSprite_setOrigin(bulletSprite, (sfVector2f){bounds.width / 2.f, bounds.height / 2.f});
            // Direction to mouse
            float dx = (float)mousePos.x - playerPos.x;
            float dy = (float)mousePos.y - playerPos.y;
            float length = sqrtf(dx*dx + dy*dy);
            if (length == 0.f) length = 1.f;
            bullets[i].velocity.x = (dx / length) * bulletVelocity;
            bullets[i].velocity.y = (dy / length) * bulletVelocity;

            float angle = atan2f(dy, dx) * 180.f / getPI();
            sfSprite_setRotation(bulletSprite, angle - 90.f);

            bullets[i].alive = sfTrue;
            break;
        }
    }
}


void rainAsteroids(sfSprite* player, sfRenderWindow* window){
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
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

            float dx = playerPos.x - apos.x + getRandomNumberInRange(-100, 600);
            float dy = playerPos.y - apos.y + getRandomNumberInRange(-100, 600);
            float length = sqrtf(dx*dx + dy*dy);
            if (length == 0.f) length = 1.f;

            asteroids[i].velocity.x = (dx / length) * asteroidStartingVelocity;
            asteroids[i].velocity.y = (dy / length) * asteroidStartingVelocity;

            float angle = atan2f(dy, dx) * 180.f / getPI();
            sfSprite_setRotation(asteroidSprite, angle - 90.f);

            asteroids[i].alive = sfTrue;
            break;
        }
    }
}


int getRandomNumberInRange(int min, int max){
    return (int) rand() % max - min + 1;
    
}


sfSprite* getRandomAsteroid(){
    int side = rand() % 4;
    switch (side) {
        case 0: { return createSpriteFromTexture(getAsteroidTexture1()); };
        case 1: { return createSpriteFromTexture(getAsteroidTexture2()); };  
        case 2: { return createSpriteFromTexture(getAsteroidTexture3()); }; 
        case 3: { return createSpriteFromTexture(getAsteroidTexture4()); }; 
    }
}


sfVector2f randomAsteroidPosition() {
    int side = rand() % 4; // 0=top, 1=right, 2=bottom, 3=left
    switch (side) {
        case 0: return (sfVector2f){rand() % WIDTH, -50};           // top
        case 1: return (sfVector2f){WIDTH + 50, rand() % HEIGHT};   // right
        case 2: return (sfVector2f){rand() % WIDTH, HEIGHT + 50};   // bottom
        case 3: return (sfVector2f){-50, rand() % HEIGHT};          // left
    }
    return (sfVector2f){0, 0}; // fallback in case weird stuff happens
}


void update(sfSprite* player, sfRenderWindow* window) {
    sfRenderWindow_clear(window, sfGray());
    if (player)
        sfRenderWindow_drawSprite(window, player, NULL);
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].alive && bullets[i].sprite)
            sfRenderWindow_drawSprite(window, bullets[i].sprite, NULL);
    }
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (asteroids[i].alive && asteroids[i].sprite)
        sfRenderWindow_drawSprite(window, asteroids[i].sprite, NULL);
    }

    sfRenderWindow_display(window);
}
