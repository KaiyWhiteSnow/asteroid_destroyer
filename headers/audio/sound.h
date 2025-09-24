#ifndef SOUNDFX_H
#define SOUNDFX_H


#include <SFML/Audio.h>


sfSound* createSFX(char path[], int volume);
sfMusic* createMusic(int volume);
sfBool hasMusicStoppedPlaying(sfMusic* music);

#endif