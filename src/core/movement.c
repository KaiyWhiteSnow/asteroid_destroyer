#include <SFML/System.h>
#include <SFML/Graphics.h>
#include <math.h>

#include "../../headers/graphics/window.h"
#include "../../headers/core/time.h"


#define M_PI 3.14159265358979323846264338327950288


// Player movement and position
sfVector2f playerDefaultPosition = { 400.f, 300.f };
float playerVelocity = 500.f;

sfBool isPlayerMovingLeft = sfFalse;
sfBool isPlayerMovingRight = sfFalse;
sfBool isPlayerMovingUp = sfFalse;
sfBool isPlayerMovingDown = sfFalse;


// Handles movement according to bools set in check movement input
void handleMovement(sfSprite* player, float deltaTime) {
    sfVector2f playerMovement = {0.0f, 0.0f};
    if (isPlayerMovingRight) { playerMovement.x += playerVelocity * deltaTime; }
    if (isPlayerMovingLeft)  { playerMovement.x -= playerVelocity * deltaTime; }
    if (isPlayerMovingDown)  { playerMovement.y += playerVelocity * deltaTime; }
    if (isPlayerMovingUp)    { playerMovement.y -= playerVelocity * deltaTime; }

    sfVector2f pos = sfSprite_getPosition(player);
    if (pos.x + playerMovement.x >= 0.f &&
        pos.x + playerMovement.x <= getMode().width &&
        pos.y + playerMovement.y >= 0.f &&
        pos.y + playerMovement.y <= getMode().height)
        sfSprite_move(player, playerMovement);
}


// Rotates player around to face mouse
void handleRotation(sfSprite* player, sfRenderWindow* window){
    sfVector2i mousePos = sfMouse_getPositionRenderWindow(window);
    sfVector2f playerPos = sfSprite_getPosition(player);  
    float dx = mousePos.x - playerPos.x;
    float dy = mousePos.y - playerPos.y;
    float angle = atan2f(dy, dx) * 180.f / M_PI;
    sfSprite_setRotation(player, angle);
}

// Check movement input sets bools that handle movement
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

// Getters

// Returns starting position of a player
sfVector2f getDefaultPlayerPosition(){ return playerDefaultPosition; }
// Returns PI value
double getPI(){ return M_PI; }