#ifndef MOVEMENT_H
#define MOVEMENT_H


#include <SFML/Graphics.h>
#include <SFML/System/Vector2.h>

void checkMovementInput(sfEvent event);
void handleRotation(sfSprite* player, sfRenderWindow* window);
void handleMovement(sfSprite* player, float deltaTime);

sfVector2f getDefaultPlayerPosition();
double getPI();

#endif