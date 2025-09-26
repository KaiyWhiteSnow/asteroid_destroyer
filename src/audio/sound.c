#include <SFML/Audio.h>
#include <math.h>
#include <stdlib.h>


char *songs[50] = {
    "./assets/music/sabaton_acesinexile.mp3", 
    "./assets/music/sabaton_attackofthedeadmen.mp3", 
    "./assets/music/sabaton_wehrmacht.mp3"
};

int lastSongIndex = 0;


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
    sfMusic* music = sfMusic_createFromFile(songs[lastSongIndex]);
    if (volume > 100) volume = 100;
    if (volume < 0) volume = 0;
    sfMusic_setVolume(music, volume);
    lastSongIndex++;
    if (lastSongIndex == 3) lastSongIndex = 0;
    return music;
}


sfBool hasMusicStoppedPlaying(sfMusic* music){
    if (sfMusic_getStatus(music) == sfStopped){
        return sfTrue;
    } return sfFalse;
}