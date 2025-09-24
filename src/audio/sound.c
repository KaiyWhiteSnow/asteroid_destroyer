#include <SFML/Audio.h>
#include <math.h>
#include <stdlib.h>

char *song[50];

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

// TODO: Make sure a song can't play again if it already played before

// Create sfMusic* object
sfMusic* createMusic(int volume){
    int randomSong = rand() % 3;
    switch (randomSong) {
        case 0: {   *song = "./assets/music/sabaton_acesinexile.mp3";         };
        case 1: {   *song = "./assets/music/sabaton_attackofthedeadmen.mp3";  };  
        case 2: {   *song = "./assets/music/sabaton_wehrmacht.mp3"            ;}
    }
    sfMusic* music = sfMusic_createFromFile(*song);
    if (volume > 100) volume = 100;
    if (volume < 0) volume = 0;
    sfMusic_setVolume(music, volume);
    return music;
}


sfBool hasMusicStoppedPlaying(sfMusic* music){
    if (sfMusic_getStatus(music) == sfStopped){
        return sfTrue;
    } return sfFalse;
}