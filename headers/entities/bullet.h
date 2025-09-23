#ifndef BULLET_H
#define BULLET_H


// Bullet struct
typedef struct {
    sfSprite* sprite;
    sfVector2f velocity;
    sfBool alive;
} Bullet;


#include <SFML/Graphics.h>
#include <SFML/Audio.h>


void fireBullet(sfSprite* player, sfVector2i mousePos, sfRenderWindow* window);
void shootBullets(sfSound *shootfx, sfRenderWindow *window, sfSprite *player);
void updateBullets(float deltaTime);
int getMaxBullets();
void updateFireTimer(float deltaTime);
Bullet* getBullets();

#endif