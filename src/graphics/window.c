#include <SFML/Window.h>

// Constants

const unsigned short int WIDTH = 800;
const unsigned short int HEIGHT = 600;


// Getters

sfVideoMode getMode() { return (sfVideoMode){WIDTH, HEIGHT, 32}; }
int getWidth(){ return WIDTH; }
int getHeight(){ return HEIGHT; }