#include <SFML/System.h>
#include <SFML/Graphics.h>
#include "../headers/window.h"
#include "../headers/physics.h"

// Player movement and position
sfVector2f playerDefaultPosition = { 400.f, 300.f };
float playerVelocity = 500.f;

sfBool isPlayerMovingLeft = sfFalse;
sfBool isPlayerMovingRight = sfFalse;
sfBool isPlayerMovingUp = sfFalse;
sfBool isPlayerMovingDown = sfFalse;

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

sfVector2f getDefaultPlayerPosition(){ return playerDefaultPosition; }