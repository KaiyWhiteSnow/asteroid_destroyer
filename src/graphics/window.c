#include <SFML/Window.h>


const unsigned short int WIDTH = 800;
const unsigned short int HEIGHT = 600;


sfVideoMode getMode() {
    return (sfVideoMode){WIDTH, HEIGHT, 32};
}

int getWidth(){ return WIDTH; }
int getHeight(){ return HEIGHT; }