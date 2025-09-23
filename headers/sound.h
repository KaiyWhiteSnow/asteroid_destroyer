#ifndef SOUNDFX_H
#define SOUNDFX_H


#include <SFML/Audio.h>


sfSound* createSFX(char path[], int volume);
sfMusic* createMusic(char path[], int volume);


#endif