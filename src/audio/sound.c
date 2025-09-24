#include <SFML/Audio.h>

// Create sfSound* object
sfSound* createSFX(char path[], int volume){
    sfSoundBuffer *shootSFX = sfSoundBuffer_createFromFile(path);
    sfSound *sound = sfSound_create();
    if (volume > 100) volume = 100;
    if (volume < 0) volume = 0;
    sfSound_setVolume(sound, volume);
    sfSound_setBuffer(sound, shootSFX);
    return sound;
}

// Create sfMusic* object
sfMusic* createMusic(char path[], int volume){
    sfMusic* music = sfMusic_createFromFile(path);
    if (volume > 100) volume = 100;
    if (volume < 0) volume = 0;
    sfMusic_setVolume(music, volume);
    return music;
}