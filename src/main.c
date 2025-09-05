#include <stdio.h>
#include <stdlib.h>
#include <SFML/Audio.h>
#include <SFML/Graphics.h>
#include <SFML/System/Vector2.h>
#include <SFML/Window/VideoMode.h>

int main(){
    // SFML variables
    sfRenderWindow* window;
    sfEvent event;

    sfVideoMode mode = {800, 600, 32};
    window = sfRenderWindow_create(mode, "Asteroid destoryer", sfResize | sfClose, NULL);
    if (!window) return 1;
    
        // Start the game loop
    while (sfRenderWindow_isOpen(window)) {
        // Process events
        
        while (sfRenderWindow_pollEvent(window, &event)) {
            // Close window: exit
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }
            /* Clear the screen */
        sfRenderWindow_clear(window, sfBlack);

        /* Update the window */
        sfRenderWindow_display(window);
    }

    return 0;
}