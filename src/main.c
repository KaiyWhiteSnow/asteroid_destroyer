#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SFML/Audio.h>
#include <SFML/Graphics.h>
#include <SFML/System/Vector2.h>
#include <SFML/Window/VideoMode.h>
#include "../headers/color.h"
#include "../headers/window.h"
#include "../headers/time.h"
#include "../headers/movement.h"


#define MAX_BULLETS 500


const unsigned short int WIDTH = 800;
const unsigned short int HEIGHT = 400;

// Bullet struct
typedef struct {
    sfSprite* sprite;
    sfVector2f velocity;
    sfBool alive;
} Bullet;


Bullet bullets[MAX_BULLETS];


// Globals for shared textures
sfTexture* g_playerTexture = NULL;
sfTexture* g_bulletTexture = NULL;


// Globals
float bulletVelocity = 800.f;


// --- Resource management ---
int loadResources(void) {
    g_playerTexture = sfTexture_createFromFile("./sprites/player.png", NULL);
    if (!g_playerTexture) {
        fprintf(stderr, "Failed to load player texture\n");
        return 0;
    }
    g_bulletTexture = sfTexture_createFromFile("./sprites/bullet.png", NULL);
    if (!g_bulletTexture) {
        fprintf(stderr, "Failed to load bullet texture\n");
        return 0;
    }
    return 1;
}


void freeResources(void) {
    if (g_playerTexture) { sfTexture_destroy(g_playerTexture); g_playerTexture = NULL; }
    if (g_bulletTexture) { sfTexture_destroy(g_bulletTexture); g_bulletTexture = NULL; }
}


// Create a sprite from an already-loaded texture (returns NULL on failure)
sfSprite* createSpriteFromTexture(sfTexture* tex) {
    if (!tex) return NULL;
    sfSprite* s = sfSprite_create();
    if (!s) return NULL;
    sfSprite_setTexture(s, tex, sfTrue); // sprite holds a reference
    return s;
}


// --- Prototypes --- Will move into proper files later
void update(sfSprite* player, sfRenderWindow* window);
void fireBullet(sfSprite* player, sfVector2i mousePos, sfRenderWindow* window);


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
    sfSprite* player = createSpriteFromTexture(g_playerTexture);
    if (!player) {
        fprintf(stderr, "Failed to create player sprite\n");
        freeResources();
        sfRenderWindow_destroy(window);
        return 1;
    }

    sfSprite_setPosition(player, getDefaultPlayerPosition());
    sfFloatRect pbounds = sfSprite_getLocalBounds(player);
    sfSprite_setOrigin(player, (sfVector2f){pbounds.width / 2, pbounds.height / 2});

    // Game loop
    while (sfRenderWindow_isOpen(window)) {
    float deltaTime = restartDeltaTime();   

        // Process events
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) sfRenderWindow_close(window);
            checkMovementInput(event);            
        }

        // Player movement
        handleMovement(player, deltaTime);

        // Player rotation (towards mouse)
        handleRotation(player, window);

        // Fire bullet
        if (sfKeyboard_isKeyPressed(sfKeySpace)) {
            sfVector2i mousePos = sfMouse_getPositionRenderWindow(window);
            fireBullet(player, mousePos, window);
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
    if (!player || !window || !g_bulletTexture) return;

    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].alive) {
            sfSprite* bs = createSpriteFromTexture(g_bulletTexture);
            if (!bs) return;
            bullets[i].sprite = bs;
            sfVector2f playerPos = sfSprite_getPosition(player);
            sfSprite_setPosition(bs, playerPos);
            sfFloatRect bounds = sfSprite_getLocalBounds(bs);
            sfSprite_setOrigin(bs, (sfVector2f){bounds.width / 2.f, bounds.height / 2.f});
            // Direction to mouse
            float dx = (float)mousePos.x - playerPos.x;
            float dy = (float)mousePos.y - playerPos.y;
            float length = sqrtf(dx*dx + dy*dy);
            if (length == 0.f) length = 1.f;
            bullets[i].velocity.x = (dx / length) * bulletVelocity;
            bullets[i].velocity.y = (dy / length) * bulletVelocity;

            float angle = atan2f(dy, dx) * 180.f / getPI();
            sfSprite_setRotation(bs, angle - 90.f);

            bullets[i].alive = sfTrue;
            break;
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
    sfRenderWindow_display(window);
}
