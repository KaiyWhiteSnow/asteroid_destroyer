#ifndef SPRITES_H
#define SPRITES_H


#include <SFML/Graphics.h>
#include <SFML/System/Vector2.h>

int loadResources(void);
void freeResources(void);
sfTexture* getPlayerTexture();
sfTexture* getBulletTexture();
sfTexture* getAsteroidTexture();
sfSprite* createSpriteFromTexture(sfTexture* tex);

#endif