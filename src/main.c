#include <stdio.h>
#include <stdlib.h>
#include <SFML/Audio.h>
#include <SFML/Graphics.h>
#include <SFML/System/Vector2.h>
#include <SFML/Window/VideoMode.h>

/* Prepare textures */
sfSprite* initializeSprite(const char* texture);


// Player movement and position
sfVector2f playerDefaultPosition = {400.f, 300.f};
float velocity = 500.f;

sfBool isPlayerMovingLeft = sfFalse;
sfBool isPlayerMovingRight = sfFalse;
sfBool isPlayerMovingUp = sfFalse;
sfBool isPlayerMovingDown = sfFalse;


sfColor ext_sfGrey;


int main() {
    // SFML variables
    sfRenderWindow* window;
    sfEvent event;
    sfVideoMode mode = {800, 600, 32};

    sfClock* clock = sfClock_create();

    window = sfRenderWindow_create(mode, "Asteroid destoryer", sfResize | sfClose, NULL);
    if (!window) return 1;
    sfWindow_setFramerateLimit(window, 60);

    // Colors
    initColors();


    // Player sprite
    sfSprite* player = initializeSprite("./sprites/player.png");
    sfSprite_setPosition(player, playerDefaultPosition);


    // Start the game loop
    while (sfRenderWindow_isOpen(window)) {
        sfTime deltaTime = sfClock_restart(clock);
        float dt = sfTime_asSeconds(deltaTime);
        // Process events
        processEvents(window, event);
        
        if (event.type == sfEvtKeyPressed){
            sfKeyCode keyCode = event.key.code;
            if (keyCode == sfKeyRight){
                isPlayerMovingRight = sfTrue;
            }
        }
        if (event.type == sfEvtKeyReleased){
            sfKeyCode keyCode = event.key.code;
            if (keyCode == sfKeyRight){
                isPlayerMovingRight = sfFalse;
            }
        }

        sfVector2f playerMovement = {0.0f, 0.0f};
        if (isPlayerMovingRight){
            playerMovement.x += velocity * dt; 
        }
        sfVector2f newPlayer1Position = {playerDefaultPosition.x + playerMovement.x, playerDefaultPosition.y};
    
        if (newPlayer1Position.x <= mode.width && newPlayer1Position.x >= 0.0f)
        {
            playerDefaultPosition = newPlayer1Position;
            sfSprite_setPosition(player, playerDefaultPosition);
        }

        update(player, window);   
    }
    
    /* Cleanup resources */
    sfRenderWindow_destroy(window);
    sfSprite_destroy(player);

    return 0;
}

void update(sfSprite* player, sfWindow* window)
{
    /* Clear the screen */
    sfRenderWindow_clear(window, ext_sfGrey);

    /* Draw the sprites */
    sfRenderWindow_drawSprite(window, player, NULL);

    /* Update the window */
    sfRenderWindow_display(window);
}

// Takes texture and returns it's sprite
sfSprite* initializeSprite(const char* texture) {
    /* Create Texture */
    sfTexture* spriteTexture = sfTexture_createFromFile(texture, NULL);
    if (!spriteTexture)
        return NULL;

    /* Create sprite */
    sfSprite* sprite = sfSprite_create();
    sfSprite_setTexture(sprite, spriteTexture, sfTrue);
    return sprite;
}

void initColors(){
    ext_sfGrey = sfColor_fromRGBA(149, 165, 166, 255);
}

void processEvents(sfWindow *window, sfEvent event){
    while (sfRenderWindow_pollEvent(window, &event)) { if (event.type == sfEvtClosed) sfRenderWindow_close(window); }
    return;
}