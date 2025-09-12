#define _USE_MATH_DEFINES

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
    sfFloatRect bounds = sfSprite_getLocalBounds(player);
    sfSprite_setOrigin(player, (sfVector2f){bounds.width / 2, bounds.height / 2});

    // Start the game loop
    while (sfRenderWindow_isOpen(window)) {
        sfTime deltaTime = sfClock_restart(clock);
        float dt = sfTime_asSeconds(deltaTime);
        
        
        // Process events
        while (sfRenderWindow_pollEvent(window, &event)) { 
            if (event.type == sfEvtClosed) sfRenderWindow_close(window); 
            
            // Checkin for keyboard input
            checkMovementInput(event);
        }
        

        // Player rotation
        // Get mouse position relative to window
        sfVector2i mousePos = sfMouse_getPositionRenderWindow(window);

        // Compute direction vector
        float dx = mousePos.x - playerDefaultPosition.x;
        float dy = mousePos.y - playerDefaultPosition.y;

        // Compute angle with atan2 (radians → degrees)
        float angle = atan2f(dy, dx) * 180.f / 3.14159265f;

        // Apply rotation 
        sfSprite_setRotation(player, angle);

        // Movement - Create velocity vector
        sfVector2f playerMovement = {0.0f, 0.0f};
        if (isPlayerMovingRight)       { playerMovement.x += velocity * dt; }
        if (isPlayerMovingLeft)        { playerMovement.x -= velocity * dt; }
        if (isPlayerMovingDown)        { playerMovement.y += velocity * dt; }
        if (isPlayerMovingUp)          { playerMovement.y -= velocity * dt; }  
              
        sfVector2f pos = sfSprite_getPosition(player);

        if (
            pos.x + playerMovement.x >= 0.f &&
            pos.x + playerMovement.x <= mode.width &&
            pos.y + playerMovement.y >= 0.f &&
            pos.y + playerMovement.y <= mode.height
        ) sfSprite_move(player, playerMovement);
        

        // Update window 
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


sfSprite* initializeSprite(const char* texture) {
    // Takes texture and returns it's sprite
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

void checkMovementInput(sfEvent event){
    // Movement - Check input
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