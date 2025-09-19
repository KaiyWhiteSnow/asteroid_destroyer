#ifndef SPRITES_H
#define SPRITES_H


#include <SFML/Graphics.h>
#include <SFML/System/Vector2.h>

int loadResources(void);
void freeResources(void);
sfTexture* getPlayerTexture();
sfTexture* getBulletTexture();
sfTexture* getAsteroidTexture1();
sfTexture* getAsteroidTexture2();
sfTexture* getAsteroidTexture3();
sfTexture* getAsteroidTexture4();
sfSprite* createSpriteFromTexture(sfTexture* tex);

#endif