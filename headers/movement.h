#ifndef MOVEMENT_H
#define MOVEMENT_H


#include <SFML/Graphics.h>
#include <SFML/System/Vector2.h>

void checkMovementInput(sfEvent event);
void handleMovement(sfSprite* player, float deltaTime);

sfVector2f getDefaultPlayerPosition();


#endif