#include <stdio.h>
#include <stdlib.h>
#include <SFML/Audio.h>
#include <SFML/Graphics.h>
#include <SFML/System/Vector2.h>
#include <SFML/Window/VideoMode.h>
#include <math.h>

/* Prepare textures */
sfSprite* initializeSprite(const char* texture);

// Player movement and position
sfVector2f playerDefaultPosition = { 400.f, 300.f };
float playerVelocity = 500.f;
float bulletVelocity = 800.f;

sfBool isPlayerMovingLeft = sfFalse;
sfBool isPlayerMovingRight = sfFalse;
sfBool isPlayerMovingUp = sfFalse;
sfBool isPlayerMovingDown = sfFalse;

sfColor ext_sfGrey;

// Bullet struct
typedef struct {
    sfSprite* sprite;
    sfVector2f velocity;
    sfBool alive;
} Bullet;

#define MAX_BULLETS 500
Bullet bullets[MAX_BULLETS];

// Function prototypes
void update(sfSprite* player, sfRenderWindow* window);
void initColors();
void checkMovementInput(sfEvent event);
void fireBullet(sfSprite* player, sfVector2i mousePos, sfWindow* window);

int main() {
    // SFML variables
    sfRenderWindow* window;
    sfEvent event;
    sfVideoMode mode = {800, 600, 32};

    sfClock* clock = sfClock_create();

    window = sfRenderWindow_create(mode, "Asteroid Destroyer", sfResize | sfClose, NULL);
    if (!window) return 1;
    sfRenderWindow_setFramerateLimit(window, 60);

    // Colors
    initColors();

    // Player sprite
    sfSprite* player = initializeSprite("./sprites/player.png");
    sfSprite_setPosition(player, playerDefaultPosition);
    sfFloatRect bounds = sfSprite_getLocalBounds(player);
    sfSprite_setOrigin(player, (sfVector2f){bounds.width / 2, bounds.height / 2});

    // Start the game loop
    while (sfRenderWindow_isOpen(window)) {
        sfTime deltaTime = sfClock_restart(clock);
        float dt = sfTime_asSeconds(deltaTime);

        // Process events
        while (sfRenderWindow_pollEvent(window, &event)) { 
            if (event.type == sfEvtClosed) sfRenderWindow_close(window); 
            checkMovementInput(event);
        }

        // Player rotation
        sfVector2i mousePos = sfMouse_getPositionRenderWindow(window);
        float dx = mousePos.x - playerDefaultPosition.x;
        float dy = mousePos.y - playerDefaultPosition.y;
        float angle = atan2f(dy, dx) * 180.f / 3.14159265f;
        sfSprite_setRotation(player, angle);

        // Movement
        sfVector2f playerMovement = {0.0f, 0.0f};
        if (isPlayerMovingRight) { playerMovement.x += playerVelocity * dt; }
        if (isPlayerMovingLeft)  { playerMovement.x -= playerVelocity * dt; }
        if (isPlayerMovingDown)  { playerMovement.y += playerVelocity * dt; }
        if (isPlayerMovingUp)    { playerMovement.y -= playerVelocity * dt; }  

        sfVector2f pos = sfSprite_getPosition(player);
        if (pos.x + playerMovement.x >= 0.f &&
            pos.x + playerMovement.x <= mode.width &&
            pos.y + playerMovement.y >= 0.f &&
            pos.y + playerMovement.y <= mode.height)
            sfSprite_move(player, playerMovement);

        // Fire bullet
        if (sfKeyboard_isKeyPressed(sfKeySpace)) {
            fireBullet(player, mousePos, window);
        }

        // Update bullets
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].alive) {
                sfVector2f bpos = sfSprite_getPosition(bullets[i].sprite);
                bpos.x += bullets[i].velocity.x * dt;
                bpos.y += bullets[i].velocity.y * dt;
                sfSprite_setPosition(bullets[i].sprite, bpos);

                // Destroy if out of bounds
                if (bpos.x < 0 || bpos.x > 800 || bpos.y < 0 || bpos.y > 600) {
                    bullets[i].alive = sfFalse;
                    sfSprite_destroy(bullets[i].sprite);
                    bullets[i].sprite = NULL;
                }
            }
        }

        // Update window
        update(player, window);
    }

    // Cleanup
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].alive)
            sfSprite_destroy(bullets[i].sprite);
    }
    sfRenderWindow_destroy(window);
    sfSprite_destroy(player);
    sfClock_destroy(clock);

    return 0;
}

void fireBullet(sfSprite* player, sfVector2i mousePos, sfWindow* window) {
    // Find inactive bullet
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].alive) {
            bullets[i].sprite = initializeSprite("./sprites/bullet.png");
            sfVector2f playerPos = sfSprite_getPosition(player);
            sfSprite_setPosition(bullets[i].sprite, playerPos);

            sfSprite_setScale(bullets[i].sprite, (sfVector2f){0.1,0.1});

            sfFloatRect bounds = sfSprite_getLocalBounds(bullets[i].sprite);
            sfSprite_setOrigin(bullets[i].sprite, (sfVector2f){bounds.width / 2, bounds.height / 2});


            sfVector2i mousePos = sfMouse_getPositionRenderWindow(window);
            float bx = mousePos.x - playerDefaultPosition.x;
            float by = mousePos.y - playerDefaultPosition.y;
            float angle = atan2f(by, bx) * 180.f / 3.14159265f;
            sfSprite_setRotation(bullets[i].sprite, angle-90);


            // Compute direction
            float dx = mousePos.x - playerPos.x;
            float dy = mousePos.y - playerPos.y;
            float length = sqrtf(dx*dx + dy*dy);
            bullets[i].velocity.x = (dx / length) * bulletVelocity;
            bullets[i].velocity.y = (dy / length) * bulletVelocity;

            bullets[i].alive = sfTrue;
            break;
        }
    }
}

void update(sfSprite* player, sfRenderWindow* window) {
    sfRenderWindow_clear(window, ext_sfGrey);
    sfRenderWindow_drawSprite(window, player, NULL);
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].alive)
            sfRenderWindow_drawSprite(window, bullets[i].sprite, NULL);
    }
    sfRenderWindow_display(window);
}

void initColors(){
    ext_sfGrey = sfColor_fromRGBA(149, 165, 166, 255);
}

sfSprite* initializeSprite(const char* texture) {
    sfTexture* spriteTexture = sfTexture_createFromFile(texture, NULL);
    if (!spriteTexture) return NULL;
    sfSprite* sprite = sfSprite_create();
    sfSprite_setTexture(sprite, spriteTexture, sfTrue);
    return sprite;
}

void checkMovementInput(sfEvent event) {
    if (event.type == sfEvtKeyPressed) {
        sfKeyCode keyCode = event.key.code;
        if (keyCode == sfKeyRight) { isPlayerMovingRight = sfTrue;  }
        if (keyCode == sfKeyLeft)  { isPlayerMovingLeft = sfTrue;   }
        if (keyCode == sfKeyUp)    { isPlayerMovingUp = sfTrue;     }
        if (keyCode == sfKeyDown)  { isPlayerMovingDown = sfTrue;   }
    }
    if (event.type == sfEvtKeyReleased){
        sfKeyCode keyCode = event.key.code;
        if (keyCode == sfKeyRight) { isPlayerMovingRight = sfFalse; }
        if (keyCode == sfKeyLeft)  { isPlayerMovingLeft = sfFalse;  }
        if (keyCode == sfKeyUp)    { isPlayerMovingUp = sfFalse;    }
        if (keyCode == sfKeyDown)  { isPlayerMovingDown = sfFalse;  }
    }
}
