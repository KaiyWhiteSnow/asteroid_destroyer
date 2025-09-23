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
#include "../headers/entities/asteroid.h"
#include "../headers/audio/sound.h"


#define MAX_BULLETS 500
#define MAX_ASTEROIDS 30

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
float fireDelay = 0.2f;   // seconds between shots
float fireTimer = 0.f;    // counts down



// --- Prototypes --- Will move into proper files later
void update(sfSprite* player, sfRenderWindow* window);
void fireBullet(sfSprite* player, sfVector2i mousePos, sfRenderWindow* window);
void rainAsteroids(sfSprite* player, sfRenderWindow* window);
int getRandomNumberInRange(int min, int max);
int wasSpriteCreated(sfSprite *sprite, sfRenderWindow *window);
void updateAsteroids(float deltaTime);
void shootBullets(sfSound *shootfx, sfRenderWindow *window, sfSprite *player);
float distance(sfVector2f a, sfVector2f b);
void updateBullets(float deltaTime);
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
        shootBullets(shootfx, window, player);


        updateBullets(deltaTime);


        updateAsteroids(deltaTime);


        bulletAsteroidCollisionHandler(destroyAsteroidfx);

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


void bulletAsteroidCollisionHandler(sfSound *destroyAsteroidfx){
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
}


void shootBullets(sfSound *shootfx, sfRenderWindow *window, sfSprite *player){
    if (sfKeyboard_isKeyPressed(sfKeySpace) && fireTimer <= 0.f) {
        sfSound_play(shootfx);
        sfVector2i mousePos = sfMouse_getPositionRenderWindow(window);
        fireBullet(player, mousePos, window);
        fireTimer = fireDelay;  // reset cooldown
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


int getRandomNumberInRange(int min, int max){
    return (int) rand() % max - min + 1;
    
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

            asteroids[i].velocity.x = (dx / length) * getAsteroidBaseVelocity();
            asteroids[i].velocity.y = (dy / length) * getAsteroidBaseVelocity();

            float angle = atan2f(dy, dx) * 180.f / getPI();
            sfSprite_setRotation(asteroidSprite, angle - 90.f);

            asteroids[i].alive = sfTrue;
            break;
        }
    }
}

void updateAsteroids(float deltaTime){
    // Update asteroids
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
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


float distance(sfVector2f a, sfVector2f b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return sqrtf(dx * dx + dy * dy);
}


void updateBullets(float deltaTime){
    // Update bullets
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].alive && bullets[i].sprite) {
            sfVector2f bpos = sfSprite_getPosition(bullets[i].sprite);
            bpos.x += bullets[i].velocity.x * deltaTime;  
            bpos.y += bullets[i].velocity.y * deltaTime;
            sfSprite_setPosition(bullets[i].sprite, bpos);

            if (bpos.x < 0 || bpos.x > getWidth() || bpos.y < 0 || bpos.y > getHeight()) {
                sfSprite_destroy(bullets[i].sprite);
                bullets[i].sprite = NULL;
                bullets[i].alive = sfFalse;
            }
        }
    }
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
