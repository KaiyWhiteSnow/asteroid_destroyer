#include <SFML/Graphics.h>
#include <SFML/Audio.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../../headers/entities/sprites.h"
#include "../../headers/graphics/window.h"
#include "../../headers/core/movement.h"


#define MAX_BULLETS 500


// Globals
int maxBullets = 500;
float bulletVelocity = 400.f;
float fireDelay = 0.2f;   // seconds between shots
float fireTimer = 0.f;    // counts down


// Bullet struct
typedef struct {
    sfSprite* sprite;
    sfVector2f velocity;
    sfBool alive;
} Bullet;


Bullet bullets[MAX_BULLETS];


void fireBullet(sfSprite* player, sfVector2i mousePos, sfRenderWindow* window) {
    if (!player || !window || !getBulletTexture()) return;

    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].alive) {
            sfSprite* bulletSprite = createSpriteFromTexture(getBulletTexture());
            if (!bulletSprite) {
                fprintf(stderr, "Couldn't make sprite for bullet\n");
                return;
            }
            bullets[i].sprite = bulletSprite;
            sfVector2f playerPos = sfSprite_getPosition(player);
            sfSprite_setPosition(bulletSprite, playerPos);
            sfFloatRect bounds = sfSprite_getLocalBounds(bulletSprite);
            sfSprite_setOrigin(bulletSprite, (sfVector2f){bounds.width / 2.f, bounds.height / 2.f});
            // Direction to mouse
            float dx = (float)mousePos.x - playerPos.x;
            float dy = (float)mousePos.y - playerPos.y;
            float length = sqrtf(dx*dx + dy*dy);
            if (length == 0.f) length = 1.f;
            bullets[i].velocity.x = (dx / length) * bulletVelocity;
            bullets[i].velocity.y = (dy / length) * bulletVelocity;

            float angle = atan2f(dy, dx) * 180.f / getPI();
            sfSprite_setRotation(bulletSprite, angle - 90.f);

            bullets[i].alive = sfTrue;
            break;
        }
    }
}


void shootBullets(sfSound *shootfx, sfRenderWindow *window, sfSprite *player){
    if (sfKeyboard_isKeyPressed(sfKeySpace) && fireTimer <= 0.f) {
        sfSound_play(shootfx);
        sfVector2i mousePos = sfMouse_getPositionRenderWindow(window);
        fireBullet(player, mousePos, window);
        fireTimer = fireDelay;  // reset cooldown
    }
}


void updateBullets(float deltaTime){
    // Update bullets
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].alive && bullets[i].sprite) {
            sfVector2f bpos = sfSprite_getPosition(bullets[i].sprite);
            bpos.x += bullets[i].velocity.x * deltaTime;  
            bpos.y += bullets[i].velocity.y * deltaTime;
            sfSprite_setPosition(bullets[i].sprite, bpos);

            if (bpos.x < 0 || bpos.x > getWidth() || bpos.y < 0 || bpos.y > getHeight()) {
                sfSprite_destroy(bullets[i].sprite);
                bullets[i].alive = sfFalse;
            }
        }
    }
}


Bullet* getBullets(){
    return bullets;
}


void updateFireTimer(float deltaTime){
    fireTimer -= deltaTime;
    if (fireTimer < 0.f) fireTimer = 0.f;
}


int getMaxBullets(){ return maxBullets; }